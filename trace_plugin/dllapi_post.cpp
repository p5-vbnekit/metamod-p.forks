// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// dllapi_post.cpp - implementation of "post" versions of Half-Life DLL routines

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

#include <extdll.h>

#include <meta_api.h>

#include "dllapi_post.h"
#include "trace_api.h"
#include "log_plugin.h"


// from SDK dlls/game.cpp:
void GameDLLInit_Post(void) {
    DLL_TRACE(pfnGameInit, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}

// from SDK dlls/cbase.cpp:
int DispatchSpawn_Post(edict_t *entity) {
    DLL_TRACE(pfnSpawn, P_POST, (
        "classname=%s, returning %d",
        entity ? STRING(entity->v.classname) : "nil",
        META_RESULT_ORIG_RET(int)
    ));
    // 0==Success, -1==Failure ?
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void DispatchThink_Post(edict_t *) {
    DLL_TRACE(pfnThink, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void DispatchUse_Post(edict_t */*used*/, edict_t */*other*/) {
    DLL_TRACE(pfnUse, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void DispatchTouch_Post(edict_t */*touched*/, edict_t */*other*/) {
    DLL_TRACE(pfnTouch, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void DispatchBlocked_Post(edict_t */*blocked*/, edict_t */*other*/) {
    DLL_TRACE(pfnBlocked, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void DispatchKeyValue_Post(edict_t *, KeyValueData *kvd) {
    DLL_TRACE(pfnKeyValue, P_POST, (
        "classname=%s keyname=%s value=%s",
        kvd->szClassName, kvd->szKeyName, kvd->szValue
    ));
    RETURN_META(MRES_IGNORED);
}
void DispatchSave_Post(edict_t *, SAVERESTOREDATA *) {
    DLL_TRACE(pfnSave, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
int DispatchRestore_Post(edict_t *, SAVERESTOREDATA *, int /*global_entity*/) {
    DLL_TRACE(pfnRestore, P_POST, ("returning %d", META_RESULT_ORIG_RET(int)));
    // 0==Success, -1==Failure ?
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void DispatchObjectCollsionBox_Post(edict_t *) {
    DLL_TRACE(pfnSetAbsBox, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void SaveWriteFields_Post(
    SAVERESTOREDATA *, const char */*name*/, void */*base_data*/,
    TYPEDESCRIPTION *, int /*field_count*/
) {
    DLL_TRACE(pfnSaveWriteFields, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void SaveReadFields_Post(
    SAVERESTOREDATA *, const char */*name*/, void */*base_data*/,
    TYPEDESCRIPTION *, int /*field_count*/
) {
    DLL_TRACE(pfnSaveReadFields, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}

// from SDK dlls/world.cpp:
void SaveGlobalState_Post(SAVERESTOREDATA *) {
    DLL_TRACE(pfnSaveGlobalState, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void RestoreGlobalState_Post(SAVERESTOREDATA *) {
    DLL_TRACE(pfnRestoreGlobalState, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ResetGlobalState_Post(void) {
    DLL_TRACE(pfnResetGlobalState, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}

//! from SDK dlls/client.cpp:
BOOL ClientConnect_Post(
    edict_t *,
    const char */*name*/, const char */*address*/,
    char reject_reason[128]
) {
    BOOL const ret = META_RESULT_ORIG_RET(BOOL);
    if(ret) DLL_TRACE(pfnClientConnect, P_POST, ("returning %d", ret));
    else DLL_TRACE(pfnClientConnect, P_POST, ("returning %d, reason=%s", ret, reject_reason));
    RETURN_META_VALUE(MRES_IGNORED, TRUE);
}
void ClientDisconnect_Post(edict_t *) {
    DLL_TRACE(pfnClientDisconnect, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ClientKill_Post(edict_t *) {
    DLL_TRACE(pfnClientKill, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ClientPutInServer_Post(edict_t *) {
    DLL_TRACE(pfnClientPutInServer, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ClientCommand_Post(edict_t *) {
    DLL_TRACE(pfnClientCommand, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ClientUserInfoChanged_Post(edict_t *, char */*info_buffer*/) {
    DLL_TRACE(pfnClientUserInfoChanged, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ServerActivate_Post(edict_t */*list*/, int /*count*/, int /*client_max*/) {
    DLL_TRACE(pfnServerActivate, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ServerDeactivate_Post(void) {
    DLL_TRACE(pfnServerDeactivate, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void PlayerPreThink_Post(edict_t *) {
    DLL_TRACE(pfnPlayerPreThink, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void PlayerPostThink_Post(edict_t *) {
    DLL_TRACE(pfnPlayerPostThink, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void StartFrame_Post(void) {
    DLL_TRACE(pfnStartFrame, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ParmsNewLevel_Post(void) {
    DLL_TRACE(pfnParmsNewLevel, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void ParmsChangeLevel_Post(void) {
    DLL_TRACE(pfnParmsChangeLevel, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
const char * GetGameDescription_Post(void) {
    DLL_TRACE(pfnGetGameDescription, P_POST, ("returning %s", META_RESULT_ORIG_RET(char *)));
    RETURN_META_VALUE(MRES_IGNORED, "");
}
void PlayerCustomization_Post(edict_t *, customization_t *) {
    DLL_TRACE(pfnPlayerCustomization, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void SpectatorConnect_Post(edict_t *) {
    DLL_TRACE(pfnSpectatorConnect, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void SpectatorDisconnect_Post(edict_t *) {
    DLL_TRACE(pfnSpectatorDisconnect, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void SpectatorThink_Post(edict_t *) {
    DLL_TRACE(pfnSpectatorThink, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void Sys_Error_Post(const char *) {
    DLL_TRACE(pfnSys_Error, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}

// from SDK pm_shared/pm_shared.c:
void PM_Move_Post(struct playermove_s *, int /*server*/) {
    DLL_TRACE(pfnPM_Move, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void PM_Init_Post(struct playermove_s *) {
    DLL_TRACE(pfnPM_Init, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
char PM_FindTextureType_Post(char */*name*/) {
    DLL_TRACE(pfnPM_FindTextureType, P_POST, (""));
    RETURN_META_VALUE(MRES_IGNORED, '\0');
}

// from SDK dlls/client.cpp:
void SetupVisibility_Post(
    edict_t */*view_entity*/, edict_t */*client*/,
    unsigned char **/*pvs*/, unsigned char **/*pas*/
) {
    DLL_TRACE(pfnSetupVisibility, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void UpdateClientData_Post(const struct edict_s *, int /*send_weapons*/, struct clientdata_s *) {
    DLL_TRACE(pfnUpdateClientData, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
int AddToFullPack_Post(
    struct entity_state_s *, int /*e*/, edict_t */*entity*/,
    edict_t */*host*/, int /*host_flags*/,
    int /*player*/, unsigned char */*set*/
) {
    DLL_TRACE(pfnAddToFullPack, P_POST, ("returning %d", META_RESULT_ORIG_RET(int)));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void CreateBaseline_Post(
    int /*player*/, int /*entity_index*/,
    struct entity_state_s *, struct edict_s *, int /*player_model_index*/,
    vec3_t /*player_mins*/, vec3_t /*player_maxs*/
) {
    DLL_TRACE(pfnCreateBaseline, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void RegisterEncoders_Post(void) {
    DLL_TRACE(pfnRegisterEncoders, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
int GetWeaponData_Post(struct edict_s *, struct weapon_data_s *) {
    DLL_TRACE(pfnGetWeaponData, P_POST, ("returning %d", META_RESULT_ORIG_RET(int)));
    RETURN_META_VALUE(MRES_IGNORED, 1);
}
void CmdStart_Post(const edict_t *, const struct usercmd_s *, unsigned int /*random_seed*/) {
    DLL_TRACE(pfnCmdStart, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void CmdEnd_Post(const edict_t *) {
    DLL_TRACE(pfnCmdEnd, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
int ConnectionlessPacket_Post(
    const struct netadr_s *, const char */*args*/,
    char */*response_buffer*/, int */*response_buffer_size*/
) {
    DLL_TRACE(pfnConnectionlessPacket, P_POST, ("returning %d", META_RESULT_ORIG_RET(int)));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int GetHullBounds_Post(int /*hull_number*/, float */*mins*/, float */*maxs*/) {
    DLL_TRACE(pfnGetHullBounds, P_POST, ("returning %d", META_RESULT_ORIG_RET(int)));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void CreateInstancedBaselines_Post(void) {
    DLL_TRACE(pfnCreateInstancedBaselines, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
int InconsistentFile_Post(const edict_t *, const char */*file_name*/, char *disconnect_message) {
    DLL_TRACE(pfnInconsistentFile, P_POST, (
        "returning %d, message=%s", META_RESULT_ORIG_RET(int), disconnect_message
    ));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int AllowLagCompensation_Post(void) {
    DLL_TRACE(pfnAllowLagCompensation, P_POST, ("returning %d", META_RESULT_ORIG_RET(int)));
    RETURN_META_VALUE(MRES_IGNORED, 1);
}


// from SDK ?
void OnFreeEntPrivateData_Post(edict_t *) {
    NEWDLL_TRACE(pfnOnFreeEntPrivateData, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
void GameShutdown_Post(void) {
    NEWDLL_TRACE(pfnGameShutdown, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}
int ShouldCollide_Post(edict_t */*touched*/, edict_t */*other*/) {
    NEWDLL_TRACE(pfnShouldCollide, P_POST, ("returning %d", META_RESULT_ORIG_RET(int)));
    RETURN_META_VALUE(MRES_IGNORED, 1);
}

// Added 2005-08-11 (no SDK update)
void CvarValue_Post(const edict_t *, const char *) {
    NEWDLL_TRACE(pfnCvarValue, P_POST, (""));
    RETURN_META(MRES_IGNORED);
}

// Added 2005-11-22 (no SDK update)
void CvarValue2_Post(
    const edict_t *entity, int request_id,
    const char *name, const char *value
) {
    NEWDLL_TRACE(pfnCvarValue2, P_POST, (
        "player=%s, requestID=%d, cvar=%s, value=%s",
        STRING(entity->v.netname),
        request_id,
        name ? name : "nil",
        value ? value : "nil"
    ));
    RETURN_META(MRES_IGNORED);
}

// from SDK dlls/cbase.cpp:
// (wd) indicates my comments on the functions
static DLL_FUNCTIONS global_functions_post = {
    GameDLLInit_Post,               //! pfnGameInit()                   Initialize the game (one-time call after loading of game .dll)
    DispatchSpawn_Post,             //! pfnSpawn()
    DispatchThink_Post,             //! pfnThink()
    DispatchUse_Post,               //! pfnUse()
    DispatchTouch_Post,             //! pfnTouch()
    DispatchBlocked_Post,           //! pfnBlocked()
    DispatchKeyValue_Post,          //! pfnKeyValue()
    DispatchSave_Post,              //! pfnSave()
    DispatchRestore_Post,           //! pfnRestore()
    DispatchObjectCollsionBox_Post, //! pfnSetAbsBox()

    SaveWriteFields_Post,           //! pfnSaveWriteFields()
    SaveReadFields_Post,            //! pfnSaveReadFields()

    SaveGlobalState_Post,           //! pfnSaveGlobalState()
    RestoreGlobalState_Post,        //! pfnRestoreGlobalState()
    ResetGlobalState_Post,          //! pfnResetGlobalState()

    ClientConnect_Post,             //! pfnClientConnect()              (wd) Client has connected
    ClientDisconnect_Post,          //! pfnClientDisconnect()           (wd) Player has left the game
    ClientKill_Post,                //! pfnClientKill()                 (wd) Player has typed "kill"
    ClientPutInServer_Post,         //! pfnClientPutInServer()          (wd) Client is entering the game
    ClientCommand_Post,             //! pfnClientCommand()              (wd) Player has sent a command (typed, or from a bind)
    ClientUserInfoChanged_Post,     //! pfnClientUserInfoChanged()      (wd) Client has updated their setinfo structure
    ServerActivate_Post,            //! pfnServerActivate()             (wd) Server is starting a new map
    ServerDeactivate_Post,          //! pfnServerDeactivate()           (wd) Server is leaving the map (shutdown, or changelevel); SDK2

    PlayerPreThink_Post,            //! pfnPlayerPreThink()
    PlayerPostThink_Post,           //! pfnPlayerPostThink()

    StartFrame_Post,                //! pfnStartFrame()
    ParmsNewLevel_Post,             //! pfnParmsNewLevel()
    ParmsChangeLevel_Post,          //! pfnParmsChangeLevel()

    GetGameDescription_Post,        //! pfnGetGameDescription()         Returns string describing current .dll.  E.g. "TeamFotrress 2", "Half-Life"
    PlayerCustomization_Post,       //! pfnPlayerCustomization()        Notifies .dll of new customization for player.

    SpectatorConnect_Post,          //! pfnSpectatorConnect()           Called when spectator joins server
    SpectatorDisconnect_Post,       //! pfnSpectatorDisconnect()        Called when spectator leaves the server
    SpectatorThink_Post,            //! pfnSpectatorThink()             Called when spectator sends a command packet (usercmd_t)

    Sys_Error_Post,                 //! pfnSys_Error()                  Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.  SDK2

    PM_Move_Post,                   //! pfnPM_Move()                    (wd) SDK2
    PM_Init_Post,                   //! pfnPM_Init()                    Server version of player movement initialization; (wd) SDK2
    PM_FindTextureType_Post,        //! pfnPM_FindTextureType()         (wd) SDK2

    SetupVisibility_Post,           //! pfnSetupVisibility()            Set up PVS and PAS for networking for this client; (wd) SDK2
    UpdateClientData_Post,          //! pfnUpdateClientData()           Set up data sent only to specific client; (wd) SDK2
    AddToFullPack_Post,             //! pfnAddToFullPack()              (wd) SDK2
    CreateBaseline_Post,            //! pfnCreateBaseline()             Tweak entity baseline for network encoding, allows setup of player baselines, too.; (wd) SDK2
    RegisterEncoders_Post,          //! pfnRegisterEncoders()           Callbacks for network encoding; (wd) SDK2
    GetWeaponData_Post,             //! pfnGetWeaponData()              (wd) SDK2
    CmdStart_Post,                  //! pfnCmdStart()                   (wd) SDK2
    CmdEnd_Post,                    //! pfnCmdEnd()                     (wd) SDK2
    ConnectionlessPacket_Post,      //! pfnConnectionlessPacket()       (wd) SDK2
    GetHullBounds_Post,             //! pfnGetHullBounds()              (wd) SDK2
    CreateInstancedBaselines_Post,  //! pfnCreateInstancedBaselines()   (wd) SDK2
    InconsistentFile_Post,          //! pfnInconsistentFile()           (wd) SDK2
    AllowLagCompensation_Post,      //! pfnAllowLagCompensation()       (wd) SDK2
};

C_DLLEXPORT int GetEntityAPI_Post(DLL_FUNCTIONS *functions, int interface_version) {
    LOG_DEVELOPER(PLID, "called: GetEntityAPI_Post; version=%d", interface_version);

    if(!functions) {
        LOG_ERROR(PLID, "GetEntityAPI_Post called with null pFunctionTable");
        return FALSE;
    }

    if(interface_version != INTERFACE_VERSION) {
        LOG_ERROR(
            PLID, "GetEntityAPI_Post version mismatch; requested=%d ours=%d",
            interface_version, INTERFACE_VERSION
        );
        return FALSE;
    }

    memcpy(functions, &global_functions_post, sizeof(DLL_FUNCTIONS));
    return TRUE;
}

C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS *functions, int *interface_version) {
    LOG_DEVELOPER(PLID, "called: GetEntityAPI2_Post; version=%d", *interface_version);

    if(!functions) {
        LOG_ERROR(PLID, "GetEntityAPI2_Post called with null pFunctionTable");
        return FALSE;
    }

    if(*interface_version != INTERFACE_VERSION) {
        LOG_ERROR(
            PLID, "GetEntityAPI2_Post version mismatch; requested=%d ours=%d",
            *interface_version, INTERFACE_VERSION
        );
        //! Tell engine what version we had, so it can figure out who is out of date.
        *interface_version = INTERFACE_VERSION;
        return FALSE;
    }

    memcpy(functions, &global_functions_post, sizeof(DLL_FUNCTIONS));
    return TRUE;
}


static NEW_DLL_FUNCTIONS global_new_functions_post = {
    OnFreeEntPrivateData_Post,  //! pfnOnFreeEntPrivateData() Called right before the object's memory is freed.  Calls its destructor.
    GameShutdown_Post,          //! pfnGameShutdown()
    ShouldCollide_Post,         //! pfnShouldCollide()
    CvarValue_Post,             //! pfnCvarValue()
    CvarValue2_Post,            //! pfnCvarValue2()
};

C_DLLEXPORT int GetNewDLLFunctions_Post(NEW_DLL_FUNCTIONS *functions, int *interface_version) {
    LOG_DEVELOPER(PLID, "called: GetNewDLLFunctions_Post; version=%d", *interface_version);
    if(!functions) {
        LOG_ERROR(PLID, "GetNewDLLFunctions_Post called with null pNewFunctionTable");
        return FALSE;
    }

    if(*interface_version != NEW_DLL_FUNCTIONS_VERSION) {
        LOG_ERROR(
            PLID, "GetNewDLLFunctions_Post version mismatch; requested=%d ours=%d",
            *interface_version, NEW_DLL_FUNCTIONS_VERSION
        );
        //! Tell engine what version we had, so it can figure out who is out of date.
        *interface_version = NEW_DLL_FUNCTIONS_VERSION;
        return FALSE;
    }

    memcpy(functions, &global_new_functions_post, sizeof(NEW_DLL_FUNCTIONS));
    return TRUE;
}
