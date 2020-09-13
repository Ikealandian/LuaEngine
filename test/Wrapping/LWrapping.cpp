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
 * lua_State Smart Pointer handler
**/
struct LuaStateHandler
{
    lua_State* lState;

    LuaStateHandler()
    {
        // TODO:
        // USE lua_newstate and create an Allocator
        lState = luaL_newstate();
    }

    ~LuaStateHandler()
    {
        lua_close(lState);
    }
};

/**
 * lua_State Smart Pointer
 **/
using LStatePtr = std::unique_ptr<lua_State, LuaStateHandler>;

int main()
{
    // Create a new Lua state
    // NOTE: State automatically closes at end of scope
    LStatePtr L;

    

    return 0;
}