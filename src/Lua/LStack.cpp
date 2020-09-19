//
// Lua Debug
//

// Include Lua Stack
#include <Lua/LStack.hpp>

// Include Lua Debug
#include <Lua/LDebug.hpp>

/**
 * LEF_GetAndCompare Implementation
 *  Register a C function for Lua
 **/
void LEF_RegisterCFunction(LRawState _State, const char* _GlobalName, lua_CFunction _CFunction)
{
    // Push the function onto the Lua stack
    lua_pushcfunction(_State, _CFunction);
    // Set the function name
    L_SetGlobal(_State, _GlobalName);
}