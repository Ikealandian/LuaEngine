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

int main()
{
    // Create a new Lua state
    // NOTE: State automatically closes at end of scope
    LStatePtr L;

    

    return 0;
}