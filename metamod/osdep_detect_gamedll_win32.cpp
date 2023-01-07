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

#include <cstddef>

#if defined(__cplusplus) && (201103L <= __cplusplus)
#include <cstdint>
#else
#include "stdint.h"
#endif

#include <extdll.h>         // always

#include "osdep_p.h"        // me
#include "support_meta.h"   // STRNCPY


// check for invalid handle values
inline bool is_invalid_handle(void const *handle) {
    return (0 == handle) || (INVALID_HANDLE_VALUE == handle);
}

// relative virtual address to virtual address
inline static ::size_t rva_to_va(void const *base, ::DWORD address) {
    return reinterpret_cast< ::size_t>(base) + address;
}

// relative virtual address to mapping virtual address
inline static ::size_t rva_to_mva(
    void const *mapping, ::IMAGE_SECTION_HEADER const *sections,
    int sections_count, ::DWORD address
) {
    for (int index_ = 0; index_ < sections_count; index_++) {
        bool const condition_ = (
            (address >= sections[index_].VirtualAddress) &&
            (address < (sections[index_].VirtualAddress + sections[index_].SizeOfRawData))
        );
        if (condition_) return rva_to_va(
            mapping, (address - sections[index_].VirtualAddress + sections[index_].PointerToRawData)
        );
    }

    return 0;
}

// Checks module signatures and return NT headers pointer for valid module
inline static ::IMAGE_NT_HEADERS const * get_nt_headers(void const *mapping) {
    union {
        ::size_t raw_address;
        ::IMAGE_DOS_HEADER const *dos;
        ::IMAGE_NT_HEADERS const *pe;
    } union_;

    // Check if valid dos header
    union_.raw_address = reinterpret_cast< ::size_t>(mapping);
    if (::IsBadReadPtr(union_.dos, sizeof(*union_.dos)) || (IMAGE_DOS_SIGNATURE != union_.dos->e_magic)) return 0;

    // Get and check pe header
    union_.raw_address = rva_to_va(mapping, union_.dos->e_lfanew);
    if (::IsBadReadPtr(union_.pe, sizeof(*union_.pe)) || (IMAGE_NT_SIGNATURE != union_.pe->Signature)) return 0;

    return union_.pe;
}

// Returns export table for valid module
inline static ::IMAGE_EXPORT_DIRECTORY const * get_export_table(
    void const *mapping, ::IMAGE_NT_HEADERS const *nt_headers,
    ::IMAGE_SECTION_HEADER const *sections, int sections_count
) {
    union {
        ::size_t raw_address;
        ::IMAGE_DOS_HEADER const *dos;
        ::IMAGE_NT_HEADERS const *pe;
        ::IMAGE_EXPORT_DIRECTORY const *export_dir;
    } union_;

    union_.pe = nt_headers;

    ::DWORD const virtual_pe_export_address_ = union_.pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    if (! virtual_pe_export_address_) return 0;

    union_.raw_address = rva_to_mva(mapping, sections, sections_count, virtual_pe_export_address_);
    if (::IsBadReadPtr(union_.export_dir, sizeof(*union_.export_dir))) return 0;

    return union_.export_dir;
}

mBOOL DLLINTERNAL is_gamedll(char const *path) {
    // Try open file for read
    ::HANDLE const file_handle_ = ::CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (is_invalid_handle(file_handle_)) {
        META_DEBUG(3, ("is_gamedll(%s): CreateFile() failed.", path));
        return mFALSE;
    }

    ::HANDLE const mapping_handle_ = ::CreateFileMappingA(file_handle_, NULL, PAGE_READONLY, 0, 0, NULL);
    if (is_invalid_handle(mapping_handle_)) {
        META_DEBUG(3, ("is_gamedll(%s): CreateFileMapping() failed.", path));
        ::CloseHandle(file_handle_);
        return mFALSE;
    }

    void const * const mapping_view_ = ::MapViewOfFile(mapping_handle_, FILE_MAP_READ, 0, 0, 0);
    if (! mapping_view_) {
        META_DEBUG(3, ("is_gamedll(%s): MapViewOfFile() failed.", path));
        ::CloseHandle(mapping_handle_);
        ::CloseHandle(file_handle_);
        return mFALSE;
    }

    ::IMAGE_NT_HEADERS const * const nt_headers_ = get_nt_headers(mapping_view_);
    if (! nt_headers_) {
        META_DEBUG(3, ("is_gamedll(%s): get_ntheaders() failed.", path));
        ::UnmapViewOfFile(mapping_view_);
        ::CloseHandle(mapping_handle_);
        ::CloseHandle(file_handle_);
        return mFALSE;
    }

    // Sections for va_to_mapaddr function
    ::IMAGE_SECTION_HEADER const * const sections_ = IMAGE_FIRST_SECTION(nt_headers_);
    ::WORD const sections_count_ = nt_headers_->FileHeader.NumberOfSections;
    if (::IsBadReadPtr(sections_, sections_count_ * sizeof(IMAGE_SECTION_HEADER))) {
        META_DEBUG(3, ("is_gamedll(%s): IMAGE_FIRST_SECTION() failed.", path));
        ::UnmapViewOfFile(mapping_view_);
        ::CloseHandle(mapping_handle_);
        ::CloseHandle(file_handle_);
        return mFALSE;
    }

    ::IMAGE_EXPORT_DIRECTORY const * const exports_ = get_export_table(mapping_view_, nt_headers_, sections_, sections_count_);
    if (! exports_) {
        META_DEBUG(3, ("is_gamedll(%s): get_export_table() failed.", path));
        ::UnmapViewOfFile(mapping_view_);
        ::CloseHandle(mapping_handle_);
        ::CloseHandle(file_handle_);
        return mFALSE;
    }

    ::DWORD const * const names_ = reinterpret_cast< ::DWORD const *>(rva_to_mva(
        mapping_view_, sections_, sections_count_, exports_->AddressOfNames
    ));
    if (::IsBadReadPtr(names_, exports_->NumberOfNames * sizeof(unsigned long))) {
        META_DEBUG(3, ("is_gamedll(%s): Pointer to exported function names is invalid.", path));
        ::UnmapViewOfFile(mapping_view_);
        ::CloseHandle(mapping_handle_);
        ::CloseHandle(file_handle_);
        return mFALSE;
    }

    bool has_GetEntityAPI_ = false;
    bool has_GetEntityAPI2_ = false;
    bool has_GiveFnptrsToDll_ = false;

    for (unsigned int index_ = 0; index_ < exports_->NumberOfNames; index_++) {
        // get function name with valid address
        char const * const function_name_ = reinterpret_cast<char const *>(rva_to_mva(
            mapping_view_, sections_, sections_count_, names_[index_]
        ));
        if (::IsBadStringPtrA(function_name_, 128)) continue;

        // Check
        switch (function_name_[0]) {
            default: break;

            case 'G': if (true) {
                // Check for goldsrc gamedll interface
                if (! has_GetEntityAPI_) has_GetEntityAPI_ = strmatch(function_name_, "GetEntityAPI");
                if (! has_GetEntityAPI2_) has_GetEntityAPI2_ = strmatch(function_name_, "GetEntityAPI2");
                if (! has_GiveFnptrsToDll_) has_GiveFnptrsToDll_ = strmatch(function_name_, "GiveFnptrsToDll");
            }; break;

            case 'M': if (true) {
                // Check if metamod plugin
                if (
                    strmatch(function_name_, "Meta_Init") ||
                    strmatch(function_name_, "Meta_Query") ||
                    strmatch(function_name_, "Meta_Attach") ||
                    strmatch(function_name_, "Meta_Detach")
                ) {
                    // Metamod plugin.. is not gamedll
                    META_DEBUG(5, ("is_gamedll(%s): Detected Metamod plugin, library exports [%s].", path, function_name_));

                    ::UnmapViewOfFile(mapping_view_);
                    ::CloseHandle(mapping_handle_);
                    ::CloseHandle(file_handle_);

                    return mFALSE;
                }
            }; break;
        }
    }

    ::UnmapViewOfFile(mapping_view_);
    ::CloseHandle(mapping_handle_);
    ::CloseHandle(file_handle_);

    // Check if gamedll
    if (has_GiveFnptrsToDll_ && (has_GetEntityAPI2_ || has_GetEntityAPI_)) {
        // This is gamedll!
        META_DEBUG(5, ("is_gamedll(%s): Detected GameDLL.", path));
        return mTRUE;
    }

    META_DEBUG(5, ("is_gamedll(%s): Library isn't GameDLL.", path));
    return mFALSE;
}
