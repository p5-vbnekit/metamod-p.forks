// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// mreg.h - description of registered items (classes MRegCmd, MRegCmdList)

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

#ifndef MREG_H
#define MREG_H

#include <cstddef>

#include <list>
#include <string>

#include "comp_dep.h"
#include "types_meta.h"
#include "new_baseclass.h"


// Flags to indicate if given cvar or func is part of a loaded plugin.
typedef enum { RG_INVALID, RG_VALID } REG_STATUS;

// Pointer to function registered by AddServerCommand.
typedef void (*REG_CMD_FN) (void);

// An individual registered function/command.
struct MRegCmd: class_metamod_new {
    ::std::string   name;
    REG_STATUS      status;     // whether corresponding plugin is loaded
    REG_CMD_FN      handler;    // pointer to the function
    int             plugin_id;  // index id of corresponding plugin

    // try to call the function
    DLLINTERNAL mBOOL call();

    DLLINTERNAL MRegCmd();
};

// A list of registered commands.
struct MRegCmdList: class_metamod_new {
    DLLINTERNAL MRegCmd * add(char const *name);
    DLLINTERNAL MRegCmd * find(char const *name);
    DLLINTERNAL MRegCmd const * find(char const *name) const;

    DLLINTERNAL void show() const;              // list items to console
    DLLINTERNAL void show(int plugin_id) const; // list items (by plugin) to console
    DLLINTERNAL void disable(int plugin_id);    // change status to Invalid

    DLLINTERNAL MRegCmdList();

private:
    ::std::size_t size_;
    ::std::list<MRegCmd> items_;

    DLLINTERNAL MRegCmdList(MRegCmdList const &);
    DLLINTERNAL MRegCmdList const & operator = (MRegCmdList const &);
};

// An individual registered cvar.
struct MRegCvar: class_metamod_new {
    cvar_t      data;       // actual cvar structure
    REG_STATUS  status;     // whether corresponding plugin is loaded
    int         plugin_id;  // index id of corresponding plugin

    DLLINTERNAL mBOOL set(cvar_t const *src);

    DLLINTERNAL MRegCvar();
};

// A list of registered cvars.
struct MRegCvarList: class_metamod_new {
    DLLINTERNAL MRegCvar * add(char const *name);
    DLLINTERNAL MRegCvar * find(char const *name);
    DLLINTERNAL MRegCvar const * find(char const *name) const;

    DLLINTERNAL void show() const;              // list all items to console
    DLLINTERNAL void show(int plugin_id) const; // list items (by plugin) to console
    DLLINTERNAL void disable(int plugin_id);    // change status to Invalid

    DLLINTERNAL MRegCvarList();

private:
    ::std::size_t size_;
    ::std::list<MRegCvar> items_;

    DLLINTERNAL MRegCvarList(MRegCvarList const &);
    DLLINTERNAL MRegCvarList const & operator = (MRegCvarList const &);
};

// An individual registered user msg, from gamedll.
struct MRegMsg: class_metamod_new {
    int             id;     // msgid, assigned by engine
    int             size;   // size, if given by gamedll
    ::std::string   name;   // name, assumed constant string in gamedll

    DLLINTERNAL MRegMsg();
};

// A list of registered user msgs.
struct MRegMsgList: class_metamod_new {
    DLLINTERNAL MRegMsg * add(int id, int size, char const *name);

    DLLINTERNAL MRegMsg const * find(int id) const;
    DLLINTERNAL MRegMsg const * find(char const *name) const;

    DLLINTERNAL void show() const; // list all items to console

    DLLINTERNAL MRegMsgList();

private:
    ::std::size_t size_;
    ::std::list<MRegMsg> items_;

    DLLINTERNAL MRegMsgList(MRegMsgList const &);
    DLLINTERNAL MRegMsgList const & operator = (MRegMsgList const &);
};

#endif /* MREG_H */
