//
// Lua State Handler
//

#ifndef _LE_LUA_STATE_HANDLER_HPP_
#define _LE_LUA_STATE_HANDLER_HPP_

// Include Lua
#include <Lua/LLua.hpp>

// Define Raw State
using LRawState = lua_State*;

// LState
typedef struct LE_StateHandler
{
    // lua_State*
    LRawState lState;

    // Create the lua_State - lState
    LE_StateHandler()
    {
        lState = luaL_newstate();
    }

    // Close the lua_State - lState
    ~LE_StateHandler()
    {
        lua_close(lState);
    }

    // Return the raw pointer
    // Type: Explicit
    lua_State* operator*()
    {
        return lState;
    }

    // Return the raw pointer
    // Type: Implicit
    operator lua_State*()
    {
        return lState;
    }

// LE_StateHandler Typedef
}LState;

#endif