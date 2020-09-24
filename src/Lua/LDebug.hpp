//
// Lua Debug
//

#ifndef _LE_LUA_DEBUGGING_HPP_
#define _LE_LUA_DEBUGGING_HPP_

// Include Lua
#include <Lua/LLua.hpp>

// Include Lua State
#include <Lua/LState.hpp>

/////////////////////////////
/** Forward Declaration { **/

/**
 * Lua types defined in LStack.hpp
 **/
enum class LTypes;

/** Forward Declaration } **/
/////////////////////////////



////////////////////
/** Debug Data { **/

/**
 * Debug data structure containing:
 *  File path to function
 *  Function name
 *  Line number
 **/
typedef struct LE_DebugData
{
    const char* File;
    const char* Function;
    int Line;
}DebugData;

/**
 * Automatically generate LE_DebugData values
 **/
#define LE_DebugData { __FILE__, __FUNCTION__, __LINE__ }

/** Debug Data { **/
////////////////////



/**
 * Verify Lua function results
 **/
bool LuaCall(
    LRawState _State, int _LFuncResult,
    DebugData _DbgData
);

/**
 * Verify Stack type
 **/
bool LuaVerifyType(
    LRawState _State, LTypes _LType, int _Index,
    DebugData _DbgData
);

#endif