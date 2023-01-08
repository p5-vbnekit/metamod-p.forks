// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mutil.cpp - utility functions to provide to plugins

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

#include <cstdio>       // vsnprintf(), etc
#include <cstdarg>      // vs_start(), etc
#include <cstring>      // ::std::strlen
#include <cstdlib>      // strtol()

#include <extdll.h>     // always

#include "meta_api.h"   //
#include "mutil.h"      // me
#include "linkent.h"    // ENTITY_FN, etc
#include "metamod.h"    // Hooks, etc
#include "types_meta.h" // mBOOL
#include "osdep.h"      // win32 vsnprintf, etc
#include "sdk_util.h"   // ALERT, etc


static hudtextparms_t default_csay_text_parmeters_ = {
    -1, 0.25,           // x, y
    2,                  // effect
    0,  255,    0,  0,  // r, g, b,  a1
    0,  0,      0,  0,  // r2, g2, b2,  a2
    0,  0,      10, 10, // fadein, fadeout, hold, fxtime
    1                   // channel
};

// Log to console; newline added.
static void log_console_(plid_t, char const *format, ...) {
    ::std::va_list payload_;
    va_start(payload_, format);
    char buffer_[MAX_LOGMSG_LEN];
    safevoid_vsnprintf(buffer_, sizeof(buffer_), format, payload_);
    va_end(payload_);

    // end msg with newline
    ::std::size_t const length_ = ::std::strlen(buffer_);
    // -1 null, -1 for newline
    if (length_ < sizeof(buffer_) - 2) ::std::strcat(buffer_, "\n");
    else buffer_[length_ - 1] = '\n';

    SERVER_PRINT(buffer_);
}

// Log regular message to logs; newline added.
static void log_message_(plid_t plugin, char const *format, ...) {
    ::std::va_list payload_;
    va_start(payload_, format);
    char buffer_[MAX_LOGMSG_LEN];
    safevoid_vsnprintf(buffer_, sizeof(buffer_), format, payload_);
    va_end(payload_);
    ALERT(at_logged, "[%s] %s\n", plugin->logtag, buffer_);
}

// Log an error message to logs; newline added.
static void log_error_(plid_t plugin, char const *format, ...) {
    ::std::va_list payload_;
    va_start(payload_, format);
    char buffer_[MAX_LOGMSG_LEN];
    safevoid_vsnprintf(buffer_, sizeof(buffer_), format, payload_);
    va_end(payload_);
    ALERT(at_logged, "[%s] ERROR: %s\n", plugin->logtag, buffer_);
}

// Log a message only if cvar "developer" set; newline added.
static void log_developer_(plid_t plugin, char const *format, ...) {
    if (0 == static_cast<int>(CVAR_GET_FLOAT("developer"))) return;

    ::std::va_list payload_;
    va_start(payload_, format);
    char buffer_[MAX_LOGMSG_LEN];
    safevoid_vsnprintf(buffer_, sizeof(buffer_), format, payload_);
    va_end(payload_);

    ALERT(at_logged, "[%s] dev: %s\n", plugin->logtag, buffer_);
}

// Print a center-message, with text parameters and varargs.  Provides
// functionality to the above center_say interfaces.
static void center_say_varargs_(
    plid_t plugin, hudtextparms_t parameters,
    char const *format, ::std::va_list payload
) {
    char buffer_[MAX_LOGMSG_LEN];
    safevoid_vsnprintf(buffer_, sizeof(buffer_), format, payload);
    log_message_(plugin, "(centersay) %s", buffer_);
    for (int index_ = 1; index_ <= gpGlobals->maxClients; index_++) {
        META_UTIL_HudMessage(INDEXENT(index_), parameters, buffer_);
    }
}

// Print message on center of all player's screens.  Uses default text
// parameters (color green, 10 second fade-in).
static void center_say_(plid_t plugin, char const *format, ...) {
    ::std::va_list payload_;
    va_start(payload_, format);
    center_say_varargs_(plugin, default_csay_text_parmeters_, format, payload_);
    va_end(payload_);
}

// Print a center-message, with given text parameters.
static void center_say_params_(
    plid_t plugin, hudtextparms_t parameters,
    char const *format, ...
) {
    ::std::va_list payload_;
    va_start(payload_, format);
    center_say_varargs_(plugin, parameters, format, payload_);
    va_end(payload_);
}

// Allow plugins to call the entity functions in the GameDLL.  In
// particular, calling "player()" as needed by most Bots.  Suggested by
// Jussi Kivilinna.
static qboolean call_game_entity_(
    plid_t plugin, char const *entity, entvars_t *variables
) {
    META_DEBUG(8, (
        "Looking up game entity '%s' for plugin '%s'", entity, plugin->name
    ));

    ENTITY_FN const callable_ = reinterpret_cast<ENTITY_FN>(DLSYM(GameDLL.handle, entity));
    if (! callable_) {
        META_WARNING(
            "Couldn't find game entity '%s' in game DLL '%s' for plugin '%s'",
            entity, GameDLL.name, plugin->name
        );
        return false;
    }

    META_DEBUG(7, (
        "Calling game entity '%s' for plugin '%s'", entity, plugin->name
    ));

    (*callable_)(variables);
    return true;
}

// Find a usermsg, registered by the gamedll, with the corresponding
// msgname, and return remaining info about it (msgid, size).
static int get_user_message_id_(plid_t plugin, char const *name, int *size) {
    META_DEBUG(8, (
        "Looking up usermsg name '%s' for plugin '%s'", name, plugin->name
    ));

    if (MRegMsg const * const message_ = RegMsgs->find(name)) {
        if (size) *size = message_->size;
        return message_->msgid;
    }

    return 0;
}

// Find a usermsg, registered by the gamedll, with the corresponding
// msgid, and return remaining info about it (msgname, size).
static char const * get_user_message_name_(plid_t plugin, int id, int *size) {
    META_DEBUG(8, (
        "Looking up usermsg id '%d' for plugin '%s'", id, plugin->name
    ));

    // Guess names for any built-in Engine messages mentioned in the SDK;
    // from dlls/util.h.
    if (64 > id) switch(id) {
        default: break;
        case SVC_TEMPENTITY:
            if (size) *size = -1;
            return "tempentity?";
        case SVC_INTERMISSION:
            if (size) *size = -1;
            return "intermission?";
        case SVC_CDTRACK:
            if (size) *size = -1;
            return "cdtrack?";
        case SVC_WEAPONANIM:
            if (size) *size = -1;
            return "weaponanim?";
        case SVC_ROOMTYPE:
            if (size) *size = -1;
            return "roomtype?";
        case SVC_DIRECTOR:
            if (size) *size = -1;
            return "director?";
    }

    if (MRegMsg const * const message_ = RegMsgs->find(id)) {
        if (size) *size = message_->size;
        // 'name' is assumed to be a constant string, allocated in the gamedll.
        return message_->name;
    }

    return 0;
}

// Return the full path of the plugin's loaded dll/so file.
static char const *get_plugin_path_(plid_t plugin) {
    MPlugin *plugin_ = Plugins->find(plugin);
    if (! plugin_) {
        META_WARNING("GetPluginPath: couldn't find plugin '%s'", plugin->name);
        return 0;
    }
    static char buffer_[PATH_MAX];
    STRNCPY(buffer_, plugin_->pathname, sizeof(buffer_));
    return buffer_;
}

// Return various string-based info about the game/MOD/gamedll.
static char const *get_game_info_(plid_t plugin, ginfo_t type) {
    char const *info_;
    switch(type) {
        default:
            META_WARNING(
                "GetGameInfo: invalid request '%d' from plugin '%s'",
                type, plugin->name
            );
            return 0;
        case GINFO_NAME:
            info_ = GameDLL.name;
            break;
        case GINFO_DESC:
            info_ = GameDLL.desc;
            break;
        case GINFO_GAMEDIR:
            info_ = GameDLL.gamedir;
            break;
        case GINFO_DLL_FULLPATH:
            info_ = GameDLL.pathname;
            break;
        case GINFO_DLL_FILENAME:
            info_ = GameDLL.file;
            break;
        case GINFO_REALDLL_FULLPATH:
            info_ = GameDLL.real_pathname;
            break;
    }
    static char buffer_[MAX_STRBUF_LEN];
    STRNCPY(buffer_, info_, sizeof(buffer_));
    return buffer_;
}

static int load_plugin_(
    plid_t plugin, char const *file_name,
    PLUG_LOADTIME now, void **handle
) {
    if (0 == file_name) return ME_ARGUMENT;

    meta_errno = ME_NOERROR;
    MPlugin const * const plugin_ = Plugins->plugin_addload(plugin, file_name, now);
    if (! plugin_) {
        if (handle) *handle = 0;
        return meta_errno;
    }

    if (handle) *handle = static_cast<void *>(plugin_->handle);
    return 0;
}

static int unload_plugin_(
    plid_t plugin, char const *file_name,
    PLUG_LOADTIME now, PL_UNLOAD_REASON reason
) {
    if (0 == file_name) return ME_ARGUMENT;

    char *end_pointer_ = 0;
    int const index_ = ::std::strtol(file_name, &end_pointer_, 10);

    MPlugin *plugin_ = 0;
    if (('\0' != *file_name) && ('\0' == *end_pointer_)) plugin_ = Plugins->find(index_);
    else plugin_ = Plugins->find_match(file_name);
    if (! plugin_) return meta_errno;

    meta_errno = ME_NOERROR;
    if (plugin_->plugin_unload(plugin, now, reason)) return 0;

    return meta_errno;
}

static int unload_plugin_by_handle_(
    plid_t plugin, void *handle,
    PLUG_LOADTIME now, PL_UNLOAD_REASON reason
) {
    if (0 == handle) return ME_ARGUMENT;

    MPlugin * const plugin_ = Plugins->find(reinterpret_cast<DLHANDLE>(handle));
    if (! plugin_) return ME_NOTFOUND;

    meta_errno = ME_NOERROR;
    if (plugin_->plugin_unload(plugin, now, reason)) return 0;

    return meta_errno;
}

// Check if player is being queried for cvar
static char const * is_querying_client_cvar_(plid_t, const edict_t *player) {
    return g_Players.is_querying_cvar(player);
}

//
static int make_request_id_(plid_t) {
    return ::std::abs(0xbeef<<16) + (++requestid_counter);
}

//
static void get_hook_tables_(
    plid_t, enginefuncs_t **engine,
    DLL_FUNCTIONS **dll, NEW_DLL_FUNCTIONS **new_dll
) {
    if (engine) *engine = &meta_engfuncs;
    if (dll) *dll = g_pHookedDllFunctions;
    if (new_dll) *new_dll = g_pHookedNewDllFunctions;
}

// Meta Utility Function table.
mutil_funcs_t MetaUtilFunctions = {
    log_console_,               // pfnLogConsole
    log_message_,               // pfnLogMessage
    log_error_,                 // pfnLogError
    log_developer_,             // pfnLogDeveloper
    center_say_,                // pfnCenterSay
    center_say_params_,         // pfnCenterSayParms
    center_say_varargs_,        // pfnCenterSayVarargs
    call_game_entity_,          // pfnCallGameEntity
    get_user_message_id_,       // pfnGetUserMsgID
    get_user_message_name_,     // pfnGetUserMsgName
    get_plugin_path_,           // pfnGetPluginPath
    get_game_info_,             // pfnGetGameInfo
    load_plugin_,               // pfnLoadPlugin
    unload_plugin_,             // pfnUnloadPlugin
    unload_plugin_by_handle_,   // pfnUnloadPluginByHandle
    is_querying_client_cvar_,   // pfnIsQueryingClientCvar
    make_request_id_,           // pfnMakeRequestID
    get_hook_tables_,           // pfnGetHookTables
};
