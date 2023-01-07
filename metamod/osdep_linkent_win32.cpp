/*
 * Copyright (c) 2004-2006 Jussi Kivilinna
 *
 *    This file is part of "Metamod All-Mod-Support"-patch for Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include <cstdlib>

#include <extdll.h>         // always

#include "osdep.h"
#include "log_meta.h"       // META_LOG, etc
#include "support_meta.h"


//
// Win32 code for dynamic linkents
//  -- by Jussi Kivilinna
//

//
// Reads metamod.dll and game.dll function export tables and combines theim to
// single table that replaces metamod.dll's original table.
//

struct sort_names_t_ {
    ::size_t name;
    unsigned short name_ordinal;
};

// relative virtual address to virtual address
inline static ::size_t rva_to_va_(void const *base, ::size_t address) {
    return reinterpret_cast< ::size_t>(base) + address;
}

// virtual address to relative virtual address
inline static ::size_t va_to_rva_(void const *base, ::size_t address) {
    return address - reinterpret_cast< ::size_t>(base);
}

// checks module signatures and return NT headers pointer for valid module
inline static ::IMAGE_NT_HEADERS const * get_nt_headers_(::HMODULE module) {
    union {
        ::size_t raw_address;
        ::IMAGE_DOS_HEADER const *dos;
        ::IMAGE_NT_HEADERS const *pe;
    } union_;

    // check if valid dos header
    union_.raw_address = reinterpret_cast< ::size_t>(module);
    if (::IsBadReadPtr(union_.dos, sizeof(*union_.dos)) || (IMAGE_DOS_SIGNATURE != union_.dos->e_magic)) return 0;

    // get and check pe header
    union_.raw_address = rva_to_va_(module, union_.dos->e_lfanew);
    if (::IsBadReadPtr(union_.pe, sizeof(*union_.pe)) || (IMAGE_NT_SIGNATURE != union_.pe->Signature)) return 0;

    return union_.pe;
}

// returns export table for valid module
inline static ::IMAGE_EXPORT_DIRECTORY * get_export_table_(::HMODULE module) {
    union {
        ::size_t raw_address;
        ::IMAGE_DOS_HEADER const *dos;
        ::IMAGE_NT_HEADERS const *pe;
        ::IMAGE_EXPORT_DIRECTORY *export_dir;
    } union_;

    union_.pe = get_nt_headers_(module);

    ::DWORD const virtual_pe_export_address_ = union_.pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    if (! virtual_pe_export_address_) return 0;

    union_.raw_address = rva_to_va_(module, virtual_pe_export_address_);
    if (::IsBadReadPtr(union_.export_dir, sizeof(*union_.export_dir))) return 0;

    return union_.export_dir;
}

// sort function for qsort
inline static int sort_names_comparator_(sort_names_t_ const *A, sort_names_t_ const *B) {
    char const *str_A = (char const *)A->name;
    char const *str_B = (char const *)B->name;
    return mm_strcmp(str_A, str_B);
}

// combines `meta` and `game` export tables and replaces `meta` table with new one
inline static int combine_export_tables_(::HMODULE meta, ::HMODULE game) {
    // get export tables
    ::IMAGE_EXPORT_DIRECTORY * const meta_export_table_ = get_export_table_(meta);
    ::IMAGE_EXPORT_DIRECTORY const * const game_export_table_ = get_export_table_(game);

    if ((! meta_export_table_) || (! game_export_table_)) {
        META_ERROR("Couldn't initialize dynamic linkents, metamod: %i, gamedll: %i. Exiting...", meta_export_table_, game_export_table_);
        return 0;
    }

    // setup new export table
    ::size_t new_number_of_names_ = meta_export_table_->NumberOfNames + game_export_table_->NumberOfNames;
    ::size_t const new_number_of_functions_ = meta_export_table_->NumberOfFunctions + game_export_table_->NumberOfFunctions;

    // allocating lists
    sort_names_t_ * const sorted_ = reinterpret_cast<sort_names_t_ *>(::std::calloc(1, new_number_of_names_ * sizeof(*sorted_)));
    unsigned long * const new_functions_ = reinterpret_cast<unsigned long *>(::std::calloc(1, new_number_of_functions_ * sizeof(*new_functions_)));

    unsigned long names_count_;

    for (names_count_ = 0; names_count_ < meta_export_table_->NumberOfNames; names_count_++) {
        // fix name address
        sorted_[names_count_].name = rva_to_va_(meta, ((unsigned long *)rva_to_va_(meta, meta_export_table_->AddressOfNames))[names_count_]);
        // ordinal is index to function list
        sorted_[names_count_].name_ordinal = ((unsigned short *)rva_to_va_(meta, meta_export_table_->AddressOfNameOrdinals))[names_count_];
    }

    unsigned long functions_count_;

    // copy meta functions to new export
    for (functions_count_ = 0; functions_count_ < meta_export_table_->NumberOfFunctions; functions_count_++) {
        new_functions_[functions_count_] = rva_to_va_(meta, ((unsigned long *)rva_to_va_(meta, meta_export_table_->AddressOfFunctions))[functions_count_]);
    }

    unsigned long i_;

    // copy game functions to new export
    for (i_ = 0; i_ < game_export_table_->NumberOfFunctions; i_++) {
        new_functions_[functions_count_ + i_] = rva_to_va_(game, ((unsigned long *)rva_to_va_(game, game_export_table_->AddressOfFunctions))[i_]);
    }

    unsigned long u_;
    unsigned long list_fix_;

    for (i_ = 0, list_fix_ = 0; i_ < game_export_table_->NumberOfNames; i_++) {
        char const * const name = (char const *)rva_to_va_(game, ((unsigned long *)rva_to_va_(game, game_export_table_->AddressOfNames))[i_]);

        // check if name already in the list
        for (u_ = 0; u_ < names_count_; u_++) {
            if (! strcasecmp(name, (char const *)sorted_[u_].name)) {
                list_fix_ -= 1;
                break;
            }
        }

        if (u_ < names_count_) continue; // already in the list.. skip

        sorted_[names_count_ + i_ + list_fix_].name = reinterpret_cast< ::size_t>(name);
        sorted_[names_count_ + i_ + list_fix_].name_ordinal = (unsigned short)functions_count_ + ((unsigned short *)rva_to_va_(game, game_export_table_->AddressOfNameOrdinals))[i_];
    }

    // set new number
    new_number_of_names_ = names_count_ + i_ + list_fix_;

    // sort names list
    ::std::qsort(
        sorted_, new_number_of_names_, sizeof(*sorted_),
        reinterpret_cast<int(*)(void const *, void const *)>(&sort_names_comparator_)
    );

    // make new_names_ list (VirtualAlloc so we dont waste heap memory to stuff that isn't freed)
    unsigned long * const new_names_ = reinterpret_cast<unsigned long *>(::VirtualAlloc(
        0, new_number_of_names_ * sizeof(*new_names_), MEM_COMMIT, PAGE_READWRITE
    ));

    // make new_name_ordinals_ list (VirtualAlloc so we dont waste heap memory to stuff that isn't freed)
    unsigned short * const new_name_ordinals_ = reinterpret_cast<unsigned short *>(::VirtualAlloc(
        0, new_number_of_names_ * sizeof(*new_name_ordinals_), MEM_COMMIT, PAGE_READWRITE
    ));

    for (i_ = 0; i_ < new_number_of_names_; i_++) {
        new_names_[i_] = sorted_[i_].name;
        new_name_ordinals_[i_] = sorted_[i_].name_ordinal;
    }

    ::std::free(sorted_);

    // translate VAs to RVAs
    for (i_ = 0; i_ < new_number_of_functions_; i_++) new_functions_[i_] = va_to_rva_(meta, new_functions_[i_]);
    for (i_ = 0; i_ < new_number_of_names_; i_++) {
        new_names_[i_] = va_to_rva_(meta, new_names_[i_]);
        new_name_ordinals_[i_] = (unsigned short)va_to_rva_(meta, new_name_ordinals_[i_]);
    }

    ::DWORD old_meta_export_protection_flags_ = 0;
    if (! ::VirtualProtect(meta_export_table_, sizeof(*meta_export_table_), PAGE_READWRITE, &old_meta_export_protection_flags_)) {
        META_ERROR("Couldn't initialize dynamic linkents, VirtualProtect failed: %i. Exiting...", ::GetLastError());
        return 0;
    }

    meta_export_table_->Base = 1;
    meta_export_table_->NumberOfNames = new_number_of_names_;
    meta_export_table_->NumberOfFunctions = new_number_of_functions_;

    meta_export_table_->AddressOfNames = va_to_rva_(meta, reinterpret_cast< ::size_t>(new_names_));
    meta_export_table_->AddressOfFunctions = va_to_rva_(meta, reinterpret_cast< ::size_t>(new_functions_));
    meta_export_table_->AddressOfNameOrdinals = va_to_rva_(meta, reinterpret_cast< ::size_t>(new_name_ordinals_));

    ::VirtualProtect(meta_export_table_, sizeof(*meta_export_table_), old_meta_export_protection_flags_, &old_meta_export_protection_flags_);
    return 1;
}

int DLLINTERNAL init_linkent_replacement(::DLHANDLE meta, ::DLHANDLE game) {
    return combine_export_tables_(meta, game);
}
