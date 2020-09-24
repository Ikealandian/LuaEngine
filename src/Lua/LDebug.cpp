//
// Lua Debug
//

// Include Lua Debug
#include <Lua/LDebug.hpp>

// Include Lua Stack
#include <Lua/LStack.hpp>

// For Printf
#include <stdio.h>

/**
 * LuaCall Implementation
 *  Verify Lua function results
 **/
bool LuaCall(LRawState _State, int _LFuncResult, DebugData _DbgData)
{
    // Lua function was executed successfully
    if (_LFuncResult == LUA_OK)
        return true;

    // Print the Lua error
    // Error is pushed to the top of the Lua stack
    // Get the error string and print it out using stderr
    fprintf(stderr, "LuaCall\t%s\t[%s:%s:%d]\n", L_GetString(_State, LUA_TOP),
        _DbgData.File, _DbgData.Function, _DbgData.Line);

    // Lua function was no successful
    return false;
}

/**
 * LuaVerifyType Implementation
 *  Verify Stack type
 **/
bool LuaVerifyType(LRawState _State, LTypes _LType, int _Index, DebugData _DbgData)
{
    // Get element type
    int Type = lua_type(_State, _Index);
    // Check if element is not equal to comparison type
    if(Type != (int)_LType)
    {
        // Print T != T
        fprintf(stderr, "Verify\tType %d != %d\t[%s:%s:%d]\n", Type, (int)_LType,
            _DbgData.File, _DbgData.Function, _DbgData.Line);
        return false;
    }
    // Return types match
    return true;
}