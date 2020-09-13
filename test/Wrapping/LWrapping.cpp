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
 * lua_State Smart Pointer deleter
**/
struct LuaStateDeleter
{

    // Close the lua_State
    void operator() (lua_State* _lua_state)
    {
        lua_close(_lua_state);
    }
};

/**
 * lua_State Smart Pointer
 **/
using LStatePtr = std::unique_ptr<lua_State, LuaStateDeleter>;

int main()
{
    // Create a new Lua state
    // NOTE: State automatically closes at end of scope
    LStatePtr L;



    return 0;
}