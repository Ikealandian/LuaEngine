//
// Lua Debug
//

#ifndef _LE_LUA_DEBUGGING_HPP_
#define _LE_LUA_DEBUGGING_HPP_

// Include Lua
#include <Lua/LLua.hpp>

// Include Lua State
#include <Lua/LState.hpp>

// Include Lua Stack
#include <Lua/LStack.hpp>

/**
 * Verify Lua function results
 **/
void LuaCall(
    LRawState _State, int _LFuncResult
);

#endif