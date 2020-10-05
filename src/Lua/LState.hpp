//
// Lua State Handler
//

#ifndef _LE_LUA_STATE_HANDLER_HPP_
#define _LE_LUA_STATE_HANDLER_HPP_

// Include Lua
#include <Lua/LLua.hpp>

// Include Lua Alloc
#include <Lua/LAlloc.hpp>

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
        lState = lua_newstate(LE_Default_LAlloc, nullptr);
    }

    // Create the lua_State with a custom Allocator
    LE_StateHandler(Allocator& _refAllocator)
    {
        lState = lua_newstate(LE_Allocator_LAlloc, &_refAllocator);
    }

    // Close the lua_State - lState
    ~LE_StateHandler()
    {
        Close();
    }

    // Close the lua_State
    void Close()
    {
        if (lState)
        {
            lua_close(lState);
            lState = nullptr;
        }
    }

    // Return the raw pointer
    // Type: Explicit
    LRawState operator*()
    {
        return lState;
    }

    // Return the raw pointer
    // Type: Implicit
    operator LRawState()
    {
        return lState;
    }

// LE_StateHandler Typedef
}LState;

#endif