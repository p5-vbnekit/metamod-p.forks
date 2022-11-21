// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// engine_api.cpp - implementation of Half-Life engine functions

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

#include <cstdio>
#include <cstdarg>

#include <extdll.h>

#include "engine_api.h"
#include "meta_api.h"
#include "trace_api.h"
#include "support_meta.h" // MAX_STRBUF_LEN
#include "log_plugin.h"


int PrecacheModel(char *name) {
    ENGINE_TRACE(pfnPrecacheModel, P_PRE, ("model=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int PrecacheSound(char *name) {
    ENGINE_TRACE(pfnPrecacheSound, P_PRE, ("sound=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void SetModel(edict_t *entity, const char *model) {
    ENGINE_TRACE(pfnSetModel, P_PRE, (
        "classname=%s netname=%s model=%s",
        entity ? STRING(entity->v.classname) : "nil",
        entity ? STRING(entity->v.netname) : "nil",
        model
    ));
    RETURN_META(MRES_IGNORED);
}
int ModelIndex(const char *name) {
    ENGINE_TRACE(pfnModelIndex, P_PRE, ("model=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int ModelFrames(int index) {
    ENGINE_TRACE(pfnModelFrames, P_PRE, ("index=%d", index));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

void SetSize(edict_t *, const float */*rgfl_min*/, const float */*rgfl_max*/) {
    ENGINE_TRACE(pfnSetSize, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void ChangeLevel(char *from, char *to) {
    ENGINE_TRACE(pfnChangeLevel, P_PRE, ("from=%s, to=%s", from, to));
    RETURN_META(MRES_IGNORED);
}
void GetSpawnParms(edict_t *) {
    ENGINE_TRACE(pfnGetSpawnParms, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void SaveSpawnParms(edict_t *) {
    ENGINE_TRACE(pfnSaveSpawnParms, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

float VecToYaw(const float *) {
    ENGINE_TRACE(pfnVecToYaw, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
void VecToAngles(const float */*rgfl_vector_in*/, float */*rgfl_vector_out*/) {
    ENGINE_TRACE(pfnVecToAngles, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void MoveToOrigin(edict_t *, const float */*goal*/, float /*dist*/, int /*move_type*/) {
    ENGINE_TRACE(pfnMoveToOrigin, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void ChangeYaw(edict_t *) {
    ENGINE_TRACE(pfnChangeYaw, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void ChangePitch(edict_t *) {
    ENGINE_TRACE(pfnChangePitch, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

edict_t * FindEntityByString(edict_t *entity, const char *field, const char *value) {
    ENGINE_TRACE(pfnFindEntityByString, P_PRE, (
        "start=%s field=%s value=%s",
        entity ? STRING(entity->v.classname) : "nil", field, value
    ));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int GetEntityIllum(edict_t *) {
    ENGINE_TRACE(pfnGetEntityIllum, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
edict_t * FindEntityInSphere(edict_t *, const float */*org*/, float /*rad*/) {
    ENGINE_TRACE(pfnFindEntityInSphere, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t * FindClientInPVS(edict_t *) {
    ENGINE_TRACE(pfnFindClientInPVS, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t * EntitiesInPVS(edict_t *) {
    ENGINE_TRACE(pfnEntitiesInPVS, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}

void MakeVectors(const float *) {
    ENGINE_TRACE(pfnMakeVectors, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void AngleVectors(
    const float */*rgfl_vector*/,
    float */*forward*/, float */*right*/, float */*up*/
) {
    ENGINE_TRACE(pfnAngleVectors, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

edict_t * CreateEntity(void) {
    ENGINE_TRACE(pfnCreateEntity, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void RemoveEntity(edict_t *entity) {
    ENGINE_TRACE(pfnRemoveEntity, P_PRE, ("name=%s", STRING(entity->v.classname)));
    RETURN_META(MRES_IGNORED);
}
edict_t * CreateNamedEntity(int class_name) {
    ENGINE_TRACE(pfnCreateNamedEntity, P_PRE, ("name=%s", STRING(class_name)));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}

void MakeStatic(edict_t *) {
    ENGINE_TRACE(pfnMakeStatic, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
int EntIsOnFloor(edict_t *) {
    ENGINE_TRACE(pfnEntIsOnFloor, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int DropToFloor(edict_t *) {
    ENGINE_TRACE(pfnDropToFloor, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

int WalkMove(edict_t *, float /*yaw*/, float /*dist*/, int /*mode*/) {
    ENGINE_TRACE(pfnWalkMove, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void SetOrigin(edict_t *entity, const float */*rgfl_origin*/) {
    ENGINE_TRACE(pfnSetOrigin, P_PRE, ("classname=%s netname=%s",
                entity ? STRING(entity->v.classname) : "nil",
                entity ? STRING(entity->v.netname) : "nil"));
    RETURN_META(MRES_IGNORED);
}

void EmitSound(
    edict_t *,
    int /*channel*/, const char *samples,
    /*int*/float /*volume*/, float /*attenuation*/,
    int /*fFlags*/, int /*pitch*/
) {
    ENGINE_TRACE(pfnEmitSound, P_PRE, ("samples=%s", samples));
    RETURN_META(MRES_IGNORED);
}
void EmitAmbientSound(
    edict_t *, float */*position*/,
    const char *samples, float /*volume*/, float /*attenuation*/,
    int /*flags*/, int /*pitch*/
) {
    ENGINE_TRACE(pfnEmitAmbientSound, P_PRE, ("samples=%s", samples));
    RETURN_META(MRES_IGNORED);
}

void TraceLine(
    const float *, const float *,
    int /*no_monsters*/, edict_t */*skip*/,
    TraceResult *
) {
    ENGINE_TRACE(pfnTraceLine, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void TraceToss(edict_t *, edict_t */*skip*/, TraceResult *) {
    ENGINE_TRACE(pfnTraceToss, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
int TraceMonsterHull(
    edict_t *, const float *, const float *,
    int /*no_monsters*/, edict_t */*skip*/,
    TraceResult *
) {
    ENGINE_TRACE(pfnTraceMonsterHull, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void TraceHull(
    const float *, const float *,
    int /*no_monsters*/, int /*hull_number*/, edict_t */*skip*/,
    TraceResult *
) {
    ENGINE_TRACE(pfnTraceHull, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void TraceModel(
    const float *, const float *,
    int /*hull_number*/, edict_t *,
    TraceResult *
) {
    ENGINE_TRACE(pfnTraceModel, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
const char * TraceTexture(edict_t *, const float *, const float *) {
    ENGINE_TRACE(pfnTraceTexture, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void TraceSphere(
    const float *, const float *,
    int /*no_monsters*/, float /*radius*/, edict_t */*skip*/,
    TraceResult *
) {
    ENGINE_TRACE(pfnTraceSphere, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void GetAimVector(edict_t *, float /*speed*/, float */*rgfl_return*/) {
    ENGINE_TRACE(pfnGetAimVector, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

void ServerCommand(char *command) {
    ENGINE_TRACE(pfnServerCommand, P_PRE, ("cmd=%s", command));
    RETURN_META(MRES_IGNORED);
}
void ServerExecute(void) {
    ENGINE_TRACE(pfnServerExecute, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void engClientCommand(edict_t *, char *format, ...) {
    va_list ap;
    char buf[1024];
    char *cp;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    cp=buf+strlen(buf)-1;
    if(*cp=='\n') *cp='\0';
    ENGINE_TRACE(pfnClientCommand, P_PRE, ("cmd='%s'", buf));
    RETURN_META(MRES_IGNORED);
}

void ParticleEffect(
    const float */*org*/, const float */*dir*/,
    float /*color*/, float /*count*/
) {
    ENGINE_TRACE(pfnParticleEffect, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void LightStyle(int /*style*/, char */*value*/) {
    ENGINE_TRACE(pfnLightStyle, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
int DecalIndex(const char *) {
    ENGINE_TRACE(pfnDecalIndex, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int PointContents(const float *) {
    ENGINE_TRACE(pfnPointContents, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

void MessageBegin(int destination, int type, const float */*origin*/, edict_t *entity) {
    const char *name, *dest;
    name=GET_USER_MSG_NAME(PLID, type, NULL);
    if(!name) name="unknown";
    dest=msg_dest_types[destination];
    if(!dest) dest="unknown";
    ENGINE_TRACE(pfnMessageBegin, P_PRE, (
        "type=%s(%d), dest=%s(%d), classname=%s netname=%s",
        name, type, dest, destination,
        entity ? STRING(entity->v.classname) : "nil",
        entity ? STRING(entity->v.netname) : "nil"
    ));
    RETURN_META(MRES_IGNORED);
}
void MessageEnd(void) {
    ENGINE_TRACE(pfnMessageEnd, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

void WriteByte(int) {
    ENGINE_TRACE(pfnWriteByte, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void WriteChar(int) {
    ENGINE_TRACE(pfnWriteChar, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void WriteShort(int) {
    ENGINE_TRACE(pfnWriteShort, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void WriteLong(int) {
    ENGINE_TRACE(pfnWriteLong, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void WriteAngle(float) {
    ENGINE_TRACE(pfnWriteAngle, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void WriteCoord(float) {
    ENGINE_TRACE(pfnWriteCoord, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void WriteString(const char *payload) {
    ENGINE_TRACE(pfnWriteString, P_PRE, ("string=%s", payload));
    RETURN_META(MRES_IGNORED);
}
void WriteEntity(int) {
    ENGINE_TRACE(pfnWriteEntity, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

void CVarRegister(cvar_t *variable) {
    ENGINE_TRACE(pfnCVarRegister, P_PRE, ("cvar=%s", variable->name));
    RETURN_META(MRES_IGNORED);
}
float CVarGetFloat(const char *name) {
    // more trace output in Post
    ENGINE_TRACE(pfnCVarGetFloat, P_PRE, ("cvar=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
const char * CVarGetString(const char *name) {
    // more trace output in Post
    ENGINE_TRACE(pfnCVarGetString, P_PRE, ("cvar=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void CVarSetFloat(const char *name, float value) {
    ENGINE_TRACE(pfnCVarSetFloat, P_PRE, ("cvar=%s, val=%f", name, value));
    RETURN_META(MRES_IGNORED);
}
void CVarSetString(const char *name, const char *value) {
    ENGINE_TRACE(pfnCVarSetString, P_PRE, ("cvar=%s, val=%s", name, value));
    RETURN_META(MRES_IGNORED);
}

void AlertMessage(ALERT_TYPE type, char *format, ...) {
    char *astr;
    va_list ap;
    char buf[MAX_STRBUF_LEN];
    switch(type) {
        case at_notice:
            astr="at_notice";
            break;
        case at_console:
            astr="at_console";
            break;
        case at_aiconsole:
            astr="at_aiconsole";
            break;
        case at_warning:
            astr="at_warning";
            break;
        case at_error:
            astr="at_error";
            break;
        case at_logged:
            astr="at_logged";
            break;
        default:
            astr="unknown";
            break;
    }
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    ENGINE_TRACE(pfnAlertMessage, P_PRE, ("atype=%s(%d) msg=%s", astr, type, buf));
    RETURN_META(MRES_IGNORED);
}
#ifdef HLSDK_3_2_OLD_EIFACE
void EngineFprintf(FILE *, char *format, ...) {
#else
void EngineFprintf(void *, char *format, ...) {
#endif
    va_list ap;
    char buf[1024];
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    ENGINE_TRACE(pfnEngineFprintf, P_PRE, ("line=%s", buf));
    RETURN_META(MRES_IGNORED);
}

#ifdef HLSDK_3_2_OLD_EIFACE
void * PvAllocEntPrivateData(edict_t *, long) {
#else
void * PvAllocEntPrivateData(edict_t *, int32) {
#endif
    ENGINE_TRACE(pfnPvAllocEntPrivateData, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void * PvEntPrivateData(edict_t *) {
    ENGINE_TRACE(pfnPvEntPrivateData, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void FreeEntPrivateData(edict_t *) {
    ENGINE_TRACE(pfnFreeEntPrivateData, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

const char * SzFromIndex(int) {
    ENGINE_TRACE(pfnSzFromIndex, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int AllocString(const char *value) {
    // more trace output in Post
    ENGINE_TRACE(pfnAllocString, P_PRE, ("str=%s", value));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

struct entvars_s * GetVarsOfEnt(edict_t *) {
    ENGINE_TRACE(pfnGetVarsOfEnt, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t * PEntityOfEntOffset(int) {
    ENGINE_TRACE(pfnPEntityOfEntOffset, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int EntOffsetOfPEntity(const edict_t *entity) {
    ENGINE_TRACE(pfnEntOffsetOfPEntity, P_PRE, (
        "classname=%s netname=%s",
        entity ? STRING(entity->v.classname) : "nil",
        entity ? STRING(entity->v.netname) : "nil"
    ));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int IndexOfEdict(const edict_t *) {
    ENGINE_TRACE(pfnIndexOfEdict, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
edict_t * PEntityOfEntIndex(int index) {
    ENGINE_TRACE(pfnPEntityOfEntIndex, P_PRE, ("index=%d", index));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
edict_t * FindEntityByVars(struct entvars_s *) {
    ENGINE_TRACE(pfnFindEntityByVars, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void * GetModelPtr(edict_t *) {
    ENGINE_TRACE(pfnGetModelPtr, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int RegUserMsg(const char *name, int size) {
    // more trace output in Post
    ENGINE_TRACE(pfnRegUserMsg, P_PRE, ("msg=%s size=%d", name, size));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

void AnimationAutomove(const edict_t *, float /*time*/) {
    ENGINE_TRACE(pfnAnimationAutomove, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void GetBonePosition(
    const edict_t *, int /*bone*/,
    float */*rgfl_origin*/, float */*rgfl_angles*/
) {
    ENGINE_TRACE(pfnGetBonePosition, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

#ifdef HLSDK_3_2_OLD_EIFACE
unsigned long FunctionFromName(const char *name) {
#else
uint32 FunctionFromName(const char *name) {
#endif
    ENGINE_TRACE(pfnFunctionFromName, P_PRE, ("name=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, 0UL);
}
#ifdef HLSDK_3_2_OLD_EIFACE
const char * NameForFunction(unsigned long) {
#else
const char * NameForFunction(uint32) {
#endif
    ENGINE_TRACE(pfnNameForFunction, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}

//! JOHN: engine callbacks so game DLL can print messages to individual clients
void ClientPrintf(edict_t *, PRINT_TYPE, const char *message) {
    ENGINE_TRACE(pfnClientPrintf, P_PRE, ("msg=%s", message));
    RETURN_META(MRES_IGNORED);
}
void ServerPrint(const char *szMsg) {
    ENGINE_TRACE(pfnServerPrint, P_PRE, ("msg=%s", szMsg));
    RETURN_META(MRES_IGNORED);
}

//! these 3 added so game DLL can easily access client 'cmd' strings
const char * Cmd_Args(void) {
    // trace output in Post
    ENGINE_TRACE(pfnCmd_Args, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
const char * Cmd_Argv(int argc) {
    // more trace output in Post
    ENGINE_TRACE(pfnCmd_Argv, P_PRE, ("argc=%d", argc));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
int Cmd_Argc(void) {
    // trace output in Post
    ENGINE_TRACE(pfnCmd_Argc, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

void GetAttachment(
    const edict_t *, int /*attachment*/,
    float */*rgfl_origin*/, float */*rgfl_angles*/
) {
    ENGINE_TRACE(pfnGetAttachment, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

void CRC32_Init(CRC32_t *) {
    ENGINE_TRACE(pfnCRC32_Init, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void CRC32_ProcessBuffer(CRC32_t *, void *, int /*size*/) {
    ENGINE_TRACE(pfnCRC32_ProcessBuffer, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void CRC32_ProcessByte(CRC32_t *, unsigned char) {
    ENGINE_TRACE(pfnCRC32_ProcessByte, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
CRC32_t CRC32_Final(CRC32_t) {
    ENGINE_TRACE(pfnCRC32_Final, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

#ifdef HLSDK_3_2_OLD_EIFACE
long RandomLong(long /*low*/, long high) {
#else
int32 RandomLong(int32 low, int32 high) {
#endif
    // more output in Post
    ENGINE_TRACE(pfnRandomLong, P_PRE, ("low=%ld, high=%ld", low, high));
    RETURN_META_VALUE(MRES_IGNORED, 0L);
}
float RandomFloat(float low, float high) {
    // more output in Post
    ENGINE_TRACE(pfnRandomFloat, P_PRE, ("low=%f, high=%f", low, high));
    RETURN_META_VALUE(MRES_IGNORED, 0.0);
}

void SetView(const edict_t */*client*/, const edict_t */*view*/) {
    ENGINE_TRACE(pfnSetView, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
float Time(void) {
    // trace output in Post
    ENGINE_TRACE(pfnTime, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0.0);
}
void CrosshairAngle(const edict_t *, float /*pitch*/, float /*yaw*/) {
    ENGINE_TRACE(pfnCrosshairAngle, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

byte * LoadFileForMe(char *name, int */*size*/) {
    ENGINE_TRACE(pfnLoadFileForMe, P_PRE, ("file=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void FreeFile(void *) {
    ENGINE_TRACE(pfnFreeFile, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

//! trigger_endsection
void EndSection(const char *name) {
    ENGINE_TRACE(pfnEndSection, P_PRE, ("section=%s", name));
    RETURN_META(MRES_IGNORED);
}
int CompareFileTime(char *name_1, char *name_2, int */*compare*/) {
    ENGINE_TRACE(pfnCompareFileTime, P_PRE, ("file1=%s, file2=%s", name_1, name_2));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void GetGameDir(char *) {
    // trace output in Post
    ENGINE_TRACE(pfnGetGameDir, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void Cvar_RegisterVariable(cvar_t *variable) {
    ENGINE_TRACE(pfnCvar_RegisterVariable, P_PRE, ("cvar=%s", variable->name));
    RETURN_META(MRES_IGNORED);
}
void FadeClientVolume(
    const edict_t *,
    int /*fade_percent*/, int /*fade_out_seconds*/,
    int /*hold_time*/, int /*fade_in_seconds*/
) {
    ENGINE_TRACE(pfnFadeClientVolume, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void SetClientMaxspeed(const edict_t *, float /*value*/) {
    ENGINE_TRACE(pfnSetClientMaxspeed, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
//! returns NULL if fake client can't be created
edict_t * CreateFakeClient(const char *name) {
    ENGINE_TRACE(pfnCreateFakeClient, P_PRE, ("name=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void RunPlayerMove(
    edict_t *,
    const float */*view_angles*/, float /*forward_move*/,
    float /*side_move*/, float /*up_move*/,
    unsigned short /*buttons*/, byte /*impulse*/, byte /*msec*/
) {
    ENGINE_TRACE(pfnRunPlayerMove, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
int NumberOfEntities(void) {
    // trace output in Post
    ENGINE_TRACE(pfnNumberOfEntities, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

//! passing in NULL gets the serverinfo
char * GetInfoKeyBuffer(edict_t *) {
    ENGINE_TRACE(pfnGetInfoKeyBuffer, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
char * InfoKeyValue(char */*info_buffer*/, char *key) {
    ENGINE_TRACE(pfnInfoKeyValue, P_PRE, ("key=%s", key));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void SetKeyValue(char */*info_buffer*/, char *key, char *value) {
    ENGINE_TRACE(pfnSetKeyValue, P_PRE, ("key=%s, value=%s", key, value));
    RETURN_META(MRES_IGNORED);
}
void SetClientKeyValue(int client_index, char */*info_buffer*/, char *key, char *value) {
    ENGINE_TRACE(pfnSetClientKeyValue, P_PRE, ("index=%d, key=%s, value=%s", client_index, key, value));
    RETURN_META(MRES_IGNORED);
}

int IsMapValid(char *file_name) {
    // more trace output in Post
    ENGINE_TRACE(pfnIsMapValid, P_PRE, ("filename=%s", file_name));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void StaticDecal(
    const float */*origin*/, int /*decal_index*/,
    int /*entity_index*/, int /*model_index*/
) {
    ENGINE_TRACE(pfnStaticDecal, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
int PrecacheGeneric(char *name) {
    ENGINE_TRACE(pfnPrecacheGeneric, P_PRE, ("name=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
//! returns the server assigned userid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
int GetPlayerUserId(edict_t *entity) {
    // more trace output in Post
    ENGINE_TRACE(pfnGetPlayerUserId, P_PRE, (
        "netname=%s", entity ? STRING(entity->v.netname) : "nil"
    ));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void BuildSoundMsg(
    edict_t *, int /*channel*/, const char *samples,
    /*int*/float /*volume*/, float /*attenuation*/,
    int /*flags*/, int /*pitch*/,
    int /*destination*/, int /*type*/,
    const float */*origin*/, edict_t *
) {
    ENGINE_TRACE(pfnBuildSoundMsg, P_PRE, ("sample=%s", samples));
    RETURN_META(MRES_IGNORED);
}
//! is this a dedicated server?
int IsDedicatedServer(void) {
    ENGINE_TRACE(pfnIsDedicatedServer, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
cvar_t * CVarGetPointer(const char *name) {
    ENGINE_TRACE(pfnCVarGetPointer, P_PRE, ("cvar=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
//! returns the server assigned WONid for this player. useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
unsigned int GetPlayerWONId(edict_t *entity) {
    // more output in Post
    ENGINE_TRACE(pfnGetPlayerWONId, P_PRE, (
        "netname=%s", entity ? STRING(entity->v.netname) : "nil"
    ));
    RETURN_META_VALUE(MRES_IGNORED, 0U);
}

//! YWB 8/1/99 TFF Physics additions
void Info_RemoveKey(char *, const char *key) {
    ENGINE_TRACE(pfnInfo_RemoveKey, P_PRE, ("key=%s", key));
    RETURN_META(MRES_IGNORED);
}
const char * GetPhysicsKeyValue(const edict_t *, const char *key) {
    ENGINE_TRACE(pfnGetPhysicsKeyValue, P_PRE, ("key=%s", key));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
void SetPhysicsKeyValue(const edict_t *, const char *key, const char *value) {
    ENGINE_TRACE(pfnSetPhysicsKeyValue, P_PRE, ("key=%s, value=%s", key, value));
    RETURN_META(MRES_IGNORED);
}
const char * GetPhysicsInfoString(const edict_t *) {
    ENGINE_TRACE(pfnGetPhysicsInfoString, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
unsigned short PrecacheEvent(int /*type*/, const char *name) {
    ENGINE_TRACE(pfnPrecacheEvent, P_PRE, ("event=%s", name));
    RETURN_META_VALUE(MRES_IGNORED, 0U);
}
void PlaybackEvent(
    int /*flags*/, const edict_t *,
    unsigned short /*event_index*/, float /*delay*/,
    float */*origin*/, float */*angles*/,
    float /*float_param_1*/, float /*float_param_2*/,
    int /*int_param_1*/, int /*int_param_2*/,
    int /*bool_param_1*/, int /*bool_param_2*/
) {
    ENGINE_TRACE(pfnPlaybackEvent, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

unsigned char * SetFatPVS(float *) {
    ENGINE_TRACE(pfnSetFatPVS, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}
unsigned char * SetFatPAS(float *) {
    ENGINE_TRACE(pfnSetFatPAS, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int CheckVisibility(const edict_t *, unsigned char *) {
    ENGINE_TRACE(pfnCheckVisibility, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

void DeltaSetField(struct delta_s *, const char *name) {
    ENGINE_TRACE(pfnDeltaSetField, P_PRE, ("field=%s", name));
    RETURN_META(MRES_IGNORED);
}
void DeltaUnsetField(struct delta_s *, const char *name) {
    ENGINE_TRACE(pfnDeltaUnsetField, P_PRE, ("field=%s", name));
    RETURN_META(MRES_IGNORED);
}
void DeltaAddEncoder(
    char */*name*/,
    void (*)(struct delta_s *fields, const unsigned char *from, const unsigned char *to)
) {
    ENGINE_TRACE(pfnDeltaAddEncoder, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
int GetCurrentPlayer(void) {
    // trace output in Post
    ENGINE_TRACE(pfnGetCurrentPlayer, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int CanSkipPlayer(const edict_t *) {
    ENGINE_TRACE(pfnCanSkipPlayer, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
int DeltaFindField(struct delta_s *, const char *) {
    ENGINE_TRACE(pfnDeltaFindField, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void DeltaSetFieldByIndex(struct delta_s *, int) {
    ENGINE_TRACE(pfnDeltaSetFieldByIndex, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}
void DeltaUnsetFieldByIndex(struct delta_s *, int) {
    ENGINE_TRACE(pfnDeltaUnsetFieldByIndex, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

void SetGroupMask(int /*mask*/, int /*op*/) {
    ENGINE_TRACE(pfnSetGroupMask, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

int engCreateInstancedBaseline(int /*class_name*/, struct entity_state_s *) {
    ENGINE_TRACE(pfnCreateInstancedBaseline, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}
void Cvar_DirectSet(struct cvar_s *variable, char *) {
    ENGINE_TRACE(pfnCvar_DirectSet, P_PRE, ("cvar=%s", variable->name));
    RETURN_META(MRES_IGNORED);
}

//! Forces the client and server to be running with the same version of the specified file
//!( e.g., a player model ).
//! Calling this has no effect in single player
void ForceUnmodified(FORCE_TYPE, float */*mins*/, float */*maxs*/, const char *file_name) {
    ENGINE_TRACE(pfnForceUnmodified, P_PRE, ("file=%s", file_name));
    RETURN_META(MRES_IGNORED);
}

void GetPlayerStats(const edict_t *client, int */*ping*/, int */*packet_loss*/) {
    ENGINE_TRACE(pfnGetPlayerStats, P_PRE, ("name=%s", STRING(client->v.netname)));
    RETURN_META(MRES_IGNORED);
}

void AddServerCommand(char *name, void (*)(void)) {
    ENGINE_TRACE(pfnAddServerCommand, P_PRE, ("cmd=%s", name));
    RETURN_META(MRES_IGNORED);
}

// Added in SDK 2.2:

//! For voice communications, set which clients hear eachother.
//! NOTE: these functions take player entity indices (starting at 1).
qboolean Voice_GetClientListening(int /*receiver*/, int /*sender*/) {
    ENGINE_TRACE(pfnVoice_GetClientListening, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, false);
}
qboolean Voice_SetClientListening(int /*receiver*/, int /*sender*/, qboolean /*listen*/) {
    ENGINE_TRACE(pfnVoice_SetClientListening, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, false);
}

// Added for HL 1109 (no SDK update):

const char * GetPlayerAuthId(edict_t *) {
    // trace output in Post
    ENGINE_TRACE(pfnGetPlayerAuthId, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}

// Added 2003/11/10 (no SDK update):

sequenceEntry_s * SequenceGet(const char* /*file_name*/, const char* /*entry_name*/) {
    ENGINE_TRACE(pfnSequenceGet, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}

sentenceEntry_s * SequencePickSentence(
    const char* /*group_name*/, int /*pick_method*/, int */*picked*/
) {
    ENGINE_TRACE(pfnSequencePickSentence, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, NULL);
}

int GetFileSize(char *) {
    ENGINE_TRACE(pfnGetFileSize, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

unsigned int GetApproxWavePlayLen(const char *) {
    ENGINE_TRACE(pfnGetApproxWavePlayLen, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

int IsCareerMatch(void) {
    ENGINE_TRACE(pfnIsCareerMatch, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

int GetLocalizedStringLength(const char *) {
    ENGINE_TRACE(pfnGetLocalizedStringLength, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

void RegisterTutorMessageShown(int) {
    ENGINE_TRACE(pfnRegisterTutorMessageShown, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

int GetTimesTutorMessageShown(int) {
    ENGINE_TRACE(pfnGetTimesTutorMessageShown, P_PRE, (""));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ProcessTutorMessageDecayBuffer(int */*buffer*/, int /*size*/) {
    ENGINE_TRACE(pfnProcessTutorMessageDecayBuffer, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

void ConstructTutorMessageDecayBuffer(int */*buffer*/, int /*size*/) {
    ENGINE_TRACE(pfnConstructTutorMessageDecayBuffer, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

void ResetTutorMessageDecayData(void) {
    ENGINE_TRACE(pfnResetTutorMessageDecayData, P_PRE, (""));
    RETURN_META(MRES_IGNORED);
}

void QueryClientCvarValue(const edict_t *, const char *name) {
    ENGINE_TRACE(pfnQueryClientCvarValue, P_PRE, ("queried=%s",name ? name : "nil"));
    RETURN_META(MRES_IGNORED);
}

void QueryClientCvarValue2(const edict_t *, const char *name, int request_id) {
    ENGINE_TRACE(pfnQueryClientCvarValue2, P_PRE, (
        "queried=%s, requestID=%d",
        name ? name : "nil", request_id
    ));
    RETURN_META(MRES_IGNORED);
}

int EngCheckParm(const char *cmd_line_token, char **/*next_value*/) {
    ENGINE_TRACE(pfnEngCheckParm, P_PRE, (
        "token=%s", cmd_line_token ? cmd_line_token : "nil"
    ));
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

enginefuncs_t meta_engfuncs = {
    PrecacheModel,          // pfnPrecacheModel()
    PrecacheSound,          // pfnPrecacheSound()
    SetModel,               // pfnSetModel()
    ModelIndex,             // pfnModelIndex()
    ModelFrames,            // pfnModelFrames()

    SetSize,                // pfnSetSize()
    ChangeLevel,            // pfnChangeLevel()
    GetSpawnParms,          // pfnGetSpawnParms()
    SaveSpawnParms,         // pfnSaveSpawnParms()

    VecToYaw,               // pfnVecToYaw()
    VecToAngles,            // pfnVecToAngles()
    MoveToOrigin,           // pfnMoveToOrigin()
    ChangeYaw,              // pfnChangeYaw()
    ChangePitch,            // pfnChangePitch()

    FindEntityByString,     // pfnFindEntityByString()
    GetEntityIllum,         // pfnGetEntityIllum()
    FindEntityInSphere,     // pfnFindEntityInSphere()
    FindClientInPVS,        // pfnFindClientInPVS()
    EntitiesInPVS,          // pfnEntitiesInPVS()

    MakeVectors,            // pfnMakeVectors()
    AngleVectors,           // pfnAngleVectors()

    CreateEntity,           // pfnCreateEntity()
    RemoveEntity,           // pfnRemoveEntity()
    CreateNamedEntity,      // pfnCreateNamedEntity()

    MakeStatic,             // pfnMakeStatic()
    EntIsOnFloor,           // pfnEntIsOnFloor()
    DropToFloor,            // pfnDropToFloor()

    WalkMove,               // pfnWalkMove()
    SetOrigin,              // pfnSetOrigin()

    EmitSound,              // pfnEmitSound()
    EmitAmbientSound,       // pfnEmitAmbientSound()

    TraceLine,              // pfnTraceLine()
    TraceToss,              // pfnTraceToss()
    TraceMonsterHull,       // pfnTraceMonsterHull()
    TraceHull,              // pfnTraceHull()
    TraceModel,             // pfnTraceModel()
    TraceTexture,           // pfnTraceTexture()
    TraceSphere,            // pfnTraceSphere()
    GetAimVector,           // pfnGetAimVector()

    ServerCommand,          // pfnServerCommand()
    ServerExecute,          // pfnServerExecute()
    engClientCommand,       // pfnClientCommand()   // d'oh, ClientCommand in dllapi too

    ParticleEffect,         // pfnParticleEffect()
    LightStyle,             // pfnLightStyle()
    DecalIndex,             // pfnDecalIndex()
    PointContents,          // pfnPointContents()

    MessageBegin,           // pfnMessageBegin()
    MessageEnd,             // pfnMessageEnd()

    WriteByte,              // pfnWriteByte()
    WriteChar,              // pfnWriteChar()
    WriteShort,             // pfnWriteShort()
    WriteLong,              // pfnWriteLong()
    WriteAngle,             // pfnWriteAngle()
    WriteCoord,             // pfnWriteCoord()
    WriteString,            // pfnWriteString()
    WriteEntity,            // pfnWriteEntity()

    CVarRegister,           // pfnCVarRegister()
    CVarGetFloat,           // pfnCVarGetFloat()
    CVarGetString,          // pfnCVarGetString()
    CVarSetFloat,           // pfnCVarSetFloat()
    CVarSetString,          // pfnCVarSetString()

    AlertMessage,           // pfnAlertMessage()
    EngineFprintf,          // pfnEngineFprintf()

    PvAllocEntPrivateData,  // pfnPvAllocEntPrivateData()
    PvEntPrivateData,       // pfnPvEntPrivateData()
    FreeEntPrivateData,     // pfnFreeEntPrivateData()

    SzFromIndex,            // pfnSzFromIndex()
    AllocString,            // pfnAllocString()

    GetVarsOfEnt,           // pfnGetVarsOfEnt()
    PEntityOfEntOffset,     // pfnPEntityOfEntOffset()
    EntOffsetOfPEntity,     // pfnEntOffsetOfPEntity()
    IndexOfEdict,           // pfnIndexOfEdict()
    PEntityOfEntIndex,      // pfnPEntityOfEntIndex()
    FindEntityByVars,       // pfnFindEntityByVars()
    GetModelPtr,            // pfnGetModelPtr()

    RegUserMsg,             // pfnRegUserMsg()

    AnimationAutomove,      // pfnAnimationAutomove()
    GetBonePosition,        // pfnGetBonePosition()

    FunctionFromName,       // pfnFunctionFromName()
    NameForFunction,        // pfnNameForFunction()

    ClientPrintf,           // pfnClientPrintf()            //! JOHN: engine callbacks so game DLL can print messages to individual clients
    ServerPrint,            // pfnServerPrint()

    Cmd_Args,               // pfnCmd_Args()                //! these 3 added
    Cmd_Argv,               // pfnCmd_Argv()                //! so game DLL can easily
    Cmd_Argc,               // pfnCmd_Argc()                //! access client 'cmd' strings

    GetAttachment,          // pfnGetAttachment()

    CRC32_Init,             // pfnCRC32_Init()
    CRC32_ProcessBuffer,    // pfnCRC32_ProcessBuffer()
    CRC32_ProcessByte,      // pfnCRC32_ProcessByte()
    CRC32_Final,            // pfnCRC32_Final()

    RandomLong,             // pfnRandomLong()
    RandomFloat,            // pfnRandomFloat()

    SetView,                // pfnSetView()
    Time,                   // pfnTime()
    CrosshairAngle,         // pfnCrosshairAngle()

    LoadFileForMe,          // pfnLoadFileForMe()
    FreeFile,               // pfnFreeFile()

    EndSection,             // pfnEndSection()              //! trigger_endsection
    CompareFileTime,        // pfnCompareFileTime()
    GetGameDir,             // pfnGetGameDir()
    Cvar_RegisterVariable,  // pfnCvar_RegisterVariable()
    FadeClientVolume,       // pfnFadeClientVolume()
    SetClientMaxspeed,      // pfnSetClientMaxspeed()
    CreateFakeClient,       // pfnCreateFakeClient()        //! returns NULL if fake client can't be created
    RunPlayerMove,          // pfnRunPlayerMove()
    NumberOfEntities,       // pfnNumberOfEntities()

    GetInfoKeyBuffer,       // pfnGetInfoKeyBuffer()        //! passing in NULL gets the serverinfo
    InfoKeyValue,           // pfnInfoKeyValue()
    SetKeyValue,            // pfnSetKeyValue()
    SetClientKeyValue,      // pfnSetClientKeyValue()

    IsMapValid,             // pfnIsMapValid()
    StaticDecal,            // pfnStaticDecal()
    PrecacheGeneric,        // pfnPrecacheGeneric()
    GetPlayerUserId,        // pfnGetPlayerUserId()         //! returns the server assigned userid for this player.
    BuildSoundMsg,          // pfnBuildSoundMsg()
    IsDedicatedServer,      // pfnIsDedicatedServer()       //! is this a dedicated server?
    CVarGetPointer,         // pfnCVarGetPointer()
    GetPlayerWONId,         // pfnGetPlayerWONId()          //! returns the server assigned WONid for this player.

    //! YWB 8/1/99 TFF Physics additions
    Info_RemoveKey,         // pfnInfo_RemoveKey()
    GetPhysicsKeyValue,     // pfnGetPhysicsKeyValue()
    SetPhysicsKeyValue,     // pfnSetPhysicsKeyValue()
    GetPhysicsInfoString,   // pfnGetPhysicsInfoString()
    PrecacheEvent,          // pfnPrecacheEvent()
    PlaybackEvent,          // pfnPlaybackEvent()

    SetFatPVS,              // pfnSetFatPVS()
    SetFatPAS,              // pfnSetFatPAS()

    CheckVisibility,        // pfnCheckVisibility()

    DeltaSetField,          // pfnDeltaSetField()
    DeltaUnsetField,        // pfnDeltaUnsetField()
    DeltaAddEncoder,        // pfnDeltaAddEncoder()
    GetCurrentPlayer,       // pfnGetCurrentPlayer()
    CanSkipPlayer,          // pfnCanSkipPlayer()
    DeltaFindField,         // pfnDeltaFindField()
    DeltaSetFieldByIndex,   // pfnDeltaSetFieldByIndex()
    DeltaUnsetFieldByIndex, // pfnDeltaUnsetFieldByIndex()

    SetGroupMask,           // pfnSetGroupMask()

    engCreateInstancedBaseline, // pfnCreateInstancedBaseline()     // d'oh, CreateInstancedBaseline in dllapi too
    Cvar_DirectSet,         // pfnCvar_DirectSet()

    ForceUnmodified,        // pfnForceUnmodified()

    GetPlayerStats,         // pfnGetPlayerStats()

    AddServerCommand,       // pfnAddServerCommand()

    // Added in SDK 2.2:
    Voice_GetClientListening,   // pfnVoice_GetClientListening()
    Voice_SetClientListening,   // pfnVoice_SetClientListening()

    // Added for HL 1109 (no SDK update):
    GetPlayerAuthId,        // pfnGetPlayerAuthId()

    // Added 2003/11/10 (no SDK update):
    SequenceGet,                    // pfnSequenceGet()
    SequencePickSentence,           // pfnSequencePickSentence()
    GetFileSize,                    // pfnGetFileSize()
    GetApproxWavePlayLen,           // pfnGetApproxWavePlayLen()
    IsCareerMatch,                  // pfnIsCareerMatch()
    GetLocalizedStringLength,       // pfnGetLocalizedStringLength()
    RegisterTutorMessageShown,      // pfnRegisterTutorMessageShown()
    GetTimesTutorMessageShown,      // pfnGetTimesTutorMessageShown()
    ProcessTutorMessageDecayBuffer, // pfnProcessTutorMessageDecayBuffer()
    ConstructTutorMessageDecayBuffer,   // pfnConstructTutorMessageDecayBuffer()
    ResetTutorMessageDecayData,     // pfnResetTutorMessageDecayData()
    QueryClientCvarValue,           // pfnQueryClientCvarValue()
    QueryClientCvarValue2,          // pfnQueryClientCvarValue2()
    EngCheckParm,                   // pfnEngCheckParm()
};

C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *engine_functions, int *interface_version) {
    LOG_DEVELOPER(PLID, "called: GetEngineFunctions; version=%d", *interface_version);

    if(!engine_functions) {
        LOG_ERROR(PLID, "GetEngineFunctions called with null pengfuncsFromEngine");
        return FALSE;
    }

    if(*interface_version != ENGINE_INTERFACE_VERSION) {
        LOG_ERROR(
            PLID,
            "GetEngineFunctions version mismatch; requested=%d ours=%d",
            *interface_version, ENGINE_INTERFACE_VERSION
        );

        // Tell metamod what version we had, so it can figure out who is
        // out of date.
        *interface_version = ENGINE_INTERFACE_VERSION;
        return FALSE;
    }

    memcpy(engine_functions, &meta_engfuncs, sizeof(enginefuncs_t));
    return TRUE;
}
