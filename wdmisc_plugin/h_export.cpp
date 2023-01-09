// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// h_export.cpp - main exported DLL functionality

// From SDK dlls/h_export.cpp:

/***
*
*   Copyright (c) 1999, 2000 Valve LLC. All rights reserved.
*
*   This product contains software technology licensed from Id
*   Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*   All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
/*

===== h_export.cpp ========================================================

  Entity classes exported by Halflife.

*/

#include <extdll.h>

#include <cstring>

#include <h_export.h>
#include <meta_api.h>


//! Holds engine functionality callbacks
enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;

// Receive engine function table from engine.
// This appears to be the _first_ DLL routine called by the engine, so we
// do some setup operations here.
WINAPI void GiveFnptrsToDll(enginefuncs_t *engine_functions, globalvars_t *global_variables) {
    ::std::memcpy(&g_engfuncs, engine_functions, sizeof(enginefuncs_t));
    gpGlobals = global_variables;
    // NOTE!  Have to call logging function _after_ copying into g_engfuncs, so
    // that g_engfuncs.pfnAlertMessage() can be resolved properly, heh. :)
    UTIL_LogPrintf("[%s] dev: called: GiveFnptrsToDll\n", Plugin_info.logtag);
}
