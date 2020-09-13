//
// Wrapping Testing
//

#include <stdlib.h>
#include <stdio.h>
#include <memory>

// Include Lua.hpp
#include <lua.hpp>

/** - Lua Types -
 * number
 * string
 * table
 * boolean
 * userdata
 * light userdata
 * function
 * thread
 * nil
 **/

/**
 * Useful macros
 **/
#define LUA_TOP -1

/**
 * lua_State Smart Pointer
 **/
struct LStatePtr
{
    // lua_State*
    lua_State* lState;

    // Create the lua_State - lState
    LStatePtr()
    {
        lState = luaL_newstate();
    }

    // Close the lua_State - lState
    ~LStatePtr()
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

};

/**
 * Verify the return value of Lua function
 **/
bool LuaCall(lua_State* _State, int _CallResult)
{
    if (_CallResult == LUA_OK)
        return true;

    printf("Lua [Error]: %s\n", lua_tostring(_State, LUA_TOP));

    return false;
}

/**
 * Useful Lua function macros
 **/
#define L_LoadFile(L, F) LuaCall(L, luaL_loadfile(L, F))
#define L_LRunFile(L, F) LuaCall(L, luaL_dofile(L, F))

int main()
{
    // Create a new Lua state
    // NOTE: State automatically closes at end of scope
    LStatePtr L;
    
    // Open Lua libraries
    luaL_openlibs(L);

    // Run the test script
    L_LRunFile(L, "test/Wrapping/test.lua");

    return 0;
}