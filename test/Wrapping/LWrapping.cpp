//
// Wrapping Testing
//

#include <stdlib.h>
#include <stdio.h>
#include <string_view>
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
 * Push a value onto the Lua stack
 **/
template<typename T>
void LuaPushValue(lua_State* _State, const T& _Value)
{
    printf("Lua (LuaPushValue) [Error]: Unable to push value of type: %s\n", typeid(T).name());
}

/* LuaPushValue: Number */
template<>
void LuaPushValue(lua_State* _State, const lua_Number& _Number)
{
    lua_pushnumber(_State, _Number);
}

/* LuaPushValue: String */
template<>
void LuaPushValue(lua_State* _State, const std::string_view& _Number)
{
    lua_pushstring(_State, _Number.data());
}

/* LuaPushValue: Boolean */
template<>
void LuaPushValue(lua_State* _State, const bool& _Number)
{
    lua_pushboolean(_State, _Number);
}

/**
 * Push multiple values onto the Lua stack
 **/
template<typename First, typename ... Others>
void LuaPush(lua_State* _State, First _First, const Others& ... _Other)
{
    // Push the first value
    LuaPushValue(_State, _First);

    // Keep calling LuaPush until _Other has no other arguments
    LuaPush(_State, _Other ...);
}

/* LuaPush: No more arguments */
void LuaPush(lua_State* _State)
{
    // Unused
    (void)(_State);
}

/********************************/
/** Useful Lua function macros **/
/********************************/

/**
 * Run and Loading Lua scripts
 **/
#define L_LoadFile(L, F)    LuaCall(L, luaL_loadfile(L, F))
#define L_LoadStr(L, F)     LuaCall(L, luaL_loadstring(L, F))
#define L_RunFile(L, F)     LuaCall(L, luaL_dofile(L, F))
#define L_RunStr(L, F)      LuaCall(L, luaL_dostring(L, F))

/**
 * Lua stack helpers
 **/
#define L_GetGlobal(L, N)   lua_getglobal(L, N)

/**
 * Lua stack check types
 **/
#define L_LuaType(L, S)     lua_type(L, S) 
#define L_IsNumber(L, S)    lua_isnumber(L, S)
#define L_IsString(L, S)    lua_isstring(L, S)
#define L_IsFunction(L, S)  lua_isfunction(L, S)
#define L_IsNil(L, S)       lua_isnil(L, S)

int main()
{
    // Create a new Lua state
    // NOTE: State automatically closes at end of scope
    LStatePtr L;
    
    // Open Lua libraries
    luaL_openlibs(L);

    // Run the test script
    L_RunFile(L, "test/Wrapping/test.lua");

    // Move global HelloLua to top of stack
    L_GetGlobal(L, "HelloLua");

    // Make sure HelloLua (top of stack) is a function
    if (L_IsFunction(L, LUA_TOP))
    {
        // Push a boolean arugment
        LuaPush(L, true);

        // Call function at the top of the stack
        // 1 -> Function takes 1 value
        // 0 -> Function returns 0 values
        // 0 -> "errfunc"
        lua_pcall(L, 1, 0, 0);
    }

    return 0;
}