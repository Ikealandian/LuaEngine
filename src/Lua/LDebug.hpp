//
// Lua Debug
//

#ifndef _LE_LUA_DEBUGGING_HPP_
#define _LE_LUA_DEBUGGING_HPP_

// Include Lua
#include <Lua/LLua.hpp>

// Include Lua State
#include <Lua/LState.hpp>

/**
 * Verify Lua function results
 **/
bool LuaCall(
    LRawState _State, int _LFuncResult
);

/**
 * Verify Stack type
 **/
bool LuaVerifyType(
    LRawState _State, int _LType, int _Index
);

#endif