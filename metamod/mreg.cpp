// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mreg.cpp - functions for registered items (classes MRegCmd, MRegCmdList)

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
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

#ifdef linux
// enable extra routines in system header files, like strsignal
#  ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#  endif
#endif /* linux */

#include <cerrno>
#include <cstddef>
#include <cstring>

#include <new>
#include <string>
#include <vector>
#include <algorithm>

#include <extdll.h>         // always

#include "mreg.h"           // me
#include "metamod.h"        // Plugins, etc
#include "mlist.h"          // class MPluginList
#include "mplugin.h"        // class MPlugin
#include "types_meta.h"     // mBOOL
#include "log_meta.h"       // META_LOG, etc
#include "osdep.h"          // os_safe_call, etc

// Width required to printf a Reg*List index number, for show() functions.
// This used to correspond to the number of digits in MAX_REG, which was a
// fixed, compile-time limit.  However, now that the reg lists are grown
// dynamically, this has less strong correspondance to list sizes.  So for
// the moment, it reflects what one might normally expect to be the max
// width needed to print an index number; 4 allows 9999 (which is a damn
// lot, if you ask me).
#define WIDTH_MAX_REG 4


///// class MRegCmd:

// Try to call the function.  Relies on OS-specific routine to attempt
// calling the function without generating a segfault from an unloaded
// plugin DLL.
// meta_errno values:
//  - ME_BADREQ     function disabled/invalid
//  - ME_ARGUMENT   function pointer is null
DLLINTERNAL mBOOL MRegCmd::call() {
    // can we expect to call this function?
    if (status != RG_VALID) RETURN_ERRNO(mFALSE, ME_BADREQ);
    if (! handler) RETURN_ERRNO(mFALSE, ME_ARGUMENT);

    // try to call this function
    mBOOL const result_ = os_safe_call(handler);

    if (! result_) {
        META_DEBUG(4, ("Plugin reg_cmd \"%s\" called after unloaded; removed from list", name));
        status = RG_INVALID;
        handler = NULL;
        // NOTE: we can't free the malloc'd space for the name, as that
        // would just re-introduce the segfault problem..
    }

    // meta_errno (if failed) is set already in os_safe_call()
    return result_;
}

DLLINTERNAL MRegCmd::MRegCmd(): name(), status(RG_INVALID), handler(NULL), plugin_id(0) {}


///// class MRegCmdList:

// Add the given name to the list and return the instance.  This only
// writes the "name" to the new cmd; other fields are writtin by caller
// (meta_AddServerCommand).
// meta_errno values:
//  - ME_NOMEM          couldn't realloc or malloc for various parts
DLLINTERNAL MRegCmd * MRegCmdList::add(char const *name) {
#if false
    // TODO: enable exceptions
    try { items_.push_back(MRegCmd()); }
    catch (::std::bad_alloc const &) {
        META_WARNING("Couldn't add registered command \"%s\" to list: ::std::bad_alloc", name);
        RETURN_ERRNO(NULL, ME_NOMEM);
    }
#else
    items_.push_back(MRegCmd());
#endif

    MRegCmd &item_ = items_.back();

#if false
    // TODO: enable exceptions
    try { item_.name = name; }
    catch(::std::bad_alloc const &) {
        items_.pop_back();
        META_WARNING("Couldn't assign name for new reg cmd \"%s\": ::std::bad_alloc", name);
        RETURN_ERRNO(NULL, ME_NOMEM);
    }
#else
    item_.name = name;
#endif

    size_++;
    return &item_;
}

// Try to find a registered function with the given name.
// meta_errno values:
//  - ME_NOTFOUND   couldn't find a matching function
DLLINTERNAL MRegCmd * MRegCmdList::find(char const *name) {
    for (::std::list<MRegCmd>::iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegCmd &item_ = *iterator_;
        if (item_.name == name) return &item_;
    }

    RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Try to find a registered function with the given name.
// meta_errno values:
//  - ME_NOTFOUND   couldn't find a matching function
DLLINTERNAL MRegCmd const * MRegCmdList::find(char const *name) const {
    return const_cast<MRegCmdList *>(this)->find(name);
}

// List all the registered commands.
DLLINTERNAL void MRegCmdList::show() const {
    ::std::size_t index_ = 0;
    ::std::size_t available_ = 0;
    ::std::size_t const max_plugin_description_length_ = ::std::min< ::std::size_t>(18, sizeof(MPlugin::desc));

    META_CONS("Registered plugin commands:");
    META_CONS("  %*s  %-*s  %-s", WIDTH_MAX_REG, "", max_plugin_description_length_, "plugin", "command");

    for (::std::list<MRegCmd>::const_iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegCmd const &item_ = *iterator_;
        char const *plugin_description_ = "(unknown)";
        ::std::string plugin_description_buffer_;

        if (RG_VALID == item_.status) {
            available_++;
            MPlugin const * const plugin_ = Plugins->find(item_.plugin_id);
            if (plugin_) {
                plugin_description_buffer_ = ::std::string(plugin_->desc, max_plugin_description_length_);
                if (! plugin_description_buffer_.empty()) plugin_description_ = plugin_description_buffer_.c_str();
            }
        }

        else plugin_description_ = "(unloaded)";

        META_CONS(" [%*d] %-*s  %-s", WIDTH_MAX_REG, ++index_, max_plugin_description_length_, plugin_description_, item_.name);
    }

    META_CONS("%d commands, %d available (%d allocated)", size_, available_, size_);
}

// List all the registered commands for the given plugin id.
DLLINTERNAL void MRegCmdList::show(int plugin_id) const {
#if false
    // If OS doesn't support DLFNAME, then we can't know what the plugin's
    // registered cvars are.
    DLFNAME(NULL);
    if (ME_OSNOTSUP == meta_errno) {
        META_CONS("Registered commands: unknown (can't get info under this OS)");
        return;
    }
#endif

    META_CONS("Registered commands:");

    for (::std::list<MRegCmd>::const_iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegCmd const &item_ = *iterator_;
        if (plugin_id != item_.plugin_id) continue;
        META_CONS("   %s", item_.name);
    }

    META_CONS("%d commands", size_);
}

// Disable any functions belonging to the given plugin (by index id).
DLLINTERNAL void MRegCmdList::disable(int plugin_id) {
    for (::std::list<MRegCmd>::iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegCmd &item_ = *iterator_;
        if (plugin_id == item_.plugin_id) item_.status = RG_INVALID;
    }
}

DLLINTERNAL MRegCmdList::MRegCmdList(): size_(0), items_() {}


///// class MRegCvar:

// Set the cvar, copying values from given cvar.
// meta_errno values:
//  - ME_ARGUMENT   given cvar doesn't match this cvar
DLLINTERNAL mBOOL MRegCvar::set(cvar_t const *src) {
    if (::strcasecmp(src->name, data.name)) {
        META_WARNING("Tried to set cvar with mismatched name; src=%s dst=%s", src->name, data.name);
        RETURN_ERRNO(mFALSE, ME_ARGUMENT);
    }
    // Would like to free() existing string, but can't tell where it was
    // allocated...
    data.string = ::strdup(src->string);
    data.flags = src->flags;
    data.value = src->value;
    data.next = src->next;
    return mTRUE;
}

DLLINTERNAL MRegCvar::MRegCvar(): data(), status(RG_INVALID), plugin_id(0) {}


///// class MRegCvarList:

// Add the given cvar name to the list and return the instance.  This only
// writes the "name" to the new cvar; other fields are written with
// cvar::set().
// meta_errno values:
//  - ME_NOMEM          couldn't alloc or realloc for various parts
DLLINTERNAL MRegCvar * MRegCvarList::add(char const *name) {
#if false
    // TODO: enable exceptions
    try { items_.push_back(MRegCvar()); }
    catch (::std::bad_alloc const &) {
        META_WARNING("Couldn't add registered cvar \"%s\" to list: ::std::bad_alloc", name);
        RETURN_ERRNO(NULL, ME_NOMEM);
    }
#else
    items_.push_back(MRegCvar());
#endif

    MRegCvar &item_ = items_.back();

    item_.data.name = ::strdup(name);
    if (! item_.data.name) {
        items_.pop_back();
        META_WARNING("Couldn't strdup for adding reg cvar name \"%s\": %s", name, strerror(errno));
        RETURN_ERRNO(NULL, ME_NOMEM);
    }

    size_++;
    return &item_;
}

// Try to find a registered cvar with the given name.
// meta_errno values:
//  - ME_NOTFOUND   couldn't find a matching cvar
DLLINTERNAL MRegCvar * MRegCvarList::find(char const *name) {
    for (::std::list<MRegCvar>::iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegCvar &item_ = *iterator_;
        if (! ::strcasecmp(item_.data.name, name)) return &item_;
    }

    RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Try to find a registered cvar with the given name.
// meta_errno values:
//  - ME_NOTFOUND   couldn't find a matching cvar
DLLINTERNAL MRegCvar const * MRegCvarList::find(char const *name) const {
    return const_cast<MRegCvarList *>(this)->find(name);
}

// Disable any cvars belonging to the given plugin (by index id).
DLLINTERNAL void MRegCvarList::disable(int plugin_id) {
    for (::std::list<MRegCvar>::iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegCvar &item_ = *iterator_;
        if (plugin_id == item_.plugin_id) item_.status = RG_INVALID;
    }
}

// List all the registered cvars.
DLLINTERNAL void MRegCvarList::show() const {
    ::std::size_t index_ = 0;
    ::std::size_t available_ = 0;
    ::std::size_t const max_name_length_ = 20;
    ::std::size_t const max_value_length_ = 15;
    ::std::size_t const max_plugin_description_length_ = ::std::min< ::std::size_t>(13, sizeof(MPlugin::desc));

    META_CONS("Registered plugin cvars:");
    META_CONS(
        "  %*s  %-*s  %-*s  %*s  %s",
        WIDTH_MAX_REG, "",
        max_plugin_description_length_, "plugin",
        max_name_length_, "cvar",
        max_value_length_, "float value",
        "string value"
    );

    char const * const unknown_ = "(unknown)";

    for (::std::list<MRegCvar>::const_iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegCvar const &item_ = *iterator_;
        char const *plugin_description_ = unknown_;
        ::std::string plugin_description_buffer_;

        if (RG_VALID == item_.status) {
            available_++;
            MPlugin const * const plugin_ = Plugins->find(item_.plugin_id);
            if (plugin_) {
                plugin_description_buffer_ = ::std::string(plugin_->desc, max_plugin_description_length_);
                if (! plugin_description_buffer_.empty()) plugin_description_ = plugin_description_buffer_.c_str();
            }
        }

        else plugin_description_ = "(unloaded)";

        ::std::string const name_buffer_(item_.data.name, max_name_length_);
        char const *name_ = unknown_;
        if (! name_buffer_.empty()) name_ = name_buffer_.c_str();

        ::std::vector<char> value_buffer_(0, 1 + max_value_length_);
        safevoid_snprintf(value_buffer_.data(), max_value_length_, "%f", item_.data.value);
        char const * const value_ = value_buffer_.data();

        META_CONS(
            " [%*d] %-*s  %-*s  %*s  \"%s\"",
            WIDTH_MAX_REG, ++index_,
            max_plugin_description_length_, plugin_description_,
            max_name_length_, name_,
            max_value_length_, value_,
            item_.data.string
        );
    }

    META_CONS("%d cvars, %d available (%d allocated)", size_, available_, size_);
}

// List the registered cvars for the given plugin id.
DLLINTERNAL void MRegCvarList::show(int plugin_id) const {
#if false
    // If OS doesn't support DLFNAME, then we can't know what the plugin's
    // registered cvars are.
    DLFNAME(NULL);
    if (ME_OSNOTSUP == meta_errno) {
        META_CONS("Registered cvars: unknown (can't get info under this OS)");
        return;
    }
#endif

    ::std::size_t const max_name_length_ = 30;
    ::std::size_t const max_value_length_ = 15;

    META_CONS(
        "%-*s     %*s  %s",
        max_name_length_, "Registered cvars:",
        max_value_length_, "float value",
        "string value"
    );

    char const * const unknown_ = "(unknown)";

    for (::std::list<MRegCvar>::const_iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegCvar const &item_ = *iterator_;
        if (plugin_id != item_.plugin_id) continue;

        ::std::string const name_buffer_(item_.data.name, max_name_length_);
        char const *name_ = unknown_;
        if (! name_buffer_.empty()) name_ = name_buffer_.c_str();

        ::std::vector<char> value_buffer_(0, 1 + max_value_length_);
        safevoid_snprintf(value_buffer_.data(), max_value_length_, "%f", item_.data.value);
        char const * const value_ = value_buffer_.data();

        META_CONS(
            "   %-*s  %*s  %s",
            max_name_length_, name_,
            max_value_length_, value_,
            item_.data.string
        );
    }

    META_CONS("%d cvars", size_);
}

DLLINTERNAL MRegCvarList::MRegCvarList(): size_(0), items_() {}


///// class MRegMsg:

DLLINTERNAL MRegMsg::MRegMsg(): id(0), size(0), name() {}


///// class MRegMsgList:

// Add the given user msg the list and return the instance.
// meta_errno values:
//  - ME_MAXREACHED     reached max number of msgs allowed
DLLINTERNAL MRegMsg * MRegMsgList::add(int id, int size, char const *name) {
    if (! (0 < id)) {
        META_WARNING("Couldn't add registered message \"%s\" to list: invalid id (%d)", id);
        RETURN_ERRNO(NULL, ME_ARGUMENT);
    }

    if (! (0 <= size)) {
        META_WARNING("Couldn't add registered message \"%s\" to list: invalid size (%d)", size);
        RETURN_ERRNO(NULL, ME_ARGUMENT);
    }

    if (! (static_cast<bool>(name) && static_cast<bool>(name[0]))) {
        META_WARNING("Couldn't add registered message \"%s\" to list: invalid name");
        RETURN_ERRNO(NULL, ME_ARGUMENT);
    }

#if false
    // TODO: enable exceptions
    try { items_.push_back(MRegMsg()); }
    catch (::std::bad_alloc const &) {
        META_WARNING("Couldn't add registered message \"%s\" to list: ::std::bad_alloc", name);
        RETURN_ERRNO(NULL, ME_NOMEM);
    }
#else
    items_.push_back(MRegMsg());
#endif

    MRegMsg &item_ = items_.back();

#if false
    // TODO: enable exceptions
    try { item_.name = name; }
    catch(::std::bad_alloc const &) {
        items_.pop_back();
        META_WARNING("Couldn't assign name for new reg cmd \"%s\": ::std::bad_alloc", name);
        RETURN_ERRNO(NULL, ME_NOMEM);
    }
#else
    item_.name = name;
#endif

    item_.id = id;
    item_.size = size;

    size_++;
    return &item_;
}

// Try to find a registered msg with the given msgid.
// meta_errno values:
//  - ME_NOTFOUND   couldn't find a matching cvar
DLLINTERNAL MRegMsg const * MRegMsgList::find(int id) const {
    for (::std::list<MRegMsg>::const_iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegMsg const &item_ = *iterator_;
        if (id == item_.id) return &item_;
    }

    RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// Try to find a registered msg with the given name.
// meta_errno values:
//  - ME_NOTFOUND   couldn't find a matching cvar
DLLINTERNAL MRegMsg const * MRegMsgList::find(char const *name) const {
    for (::std::list<MRegMsg>::const_iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegMsg const &item_ = *iterator_;
        if ((! item_.name.empty()) && (name == item_.name)) return &item_;
    }

    RETURN_ERRNO(NULL, ME_NOTFOUND);
}

// List the registered usermsgs for the gamedll.
DLLINTERNAL void MRegMsgList::show() const {
    ::std::size_t const max_name_length_ = 25;

    META_CONS("%-*s    %5s  %5s", max_name_length_, "Game registered user msgs:", "msgid", "size");

    for (::std::list<MRegMsg>::const_iterator iterator_ = items_.begin(); iterator_ != items_.end(); iterator_++ ) {
        MRegMsg const &item_ = *iterator_;

        char const *name_ = "(unknown)";
        if (! item_.name.empty()) name_ = item_.name.c_str();

        META_CONS(
            "   %-*s   %3d    %3d",
            max_name_length_, name_,
            item_.id, item_.size
        );
    }

    META_CONS("%d game user msgs", size_);
}

DLLINTERNAL MRegMsgList::MRegMsgList(): size_(0), items_() {}
