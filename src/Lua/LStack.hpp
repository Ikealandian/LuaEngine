//
// Lua Stack Manipulation
//

#ifndef _LE_LUA_STACK_HPP_
#define _LE_LUA_STACK_HPP_

// Include Lua
#include <Lua/LLua.hpp>

// Include Lua State
#include <Lua/LState.hpp>

// Include Lua Debug
#include <Lua/LDebug.hpp>

// Include Push
// Include Pop

/* Lua stack positions */
#define LUA_TOP         -1
#define LUA_FIRST       LUA_TOP - 0
#define LUA_SECOND      LUA_TOP - 1
#define LUA_THIRD       LUA_TOP - 2

/**
 * Lua Types
 **/
enum class LTypes
{
    Number          = LUA_TNUMBER,
    String          = LUA_TSTRING,
    Table           = LUA_TTABLE,
    Boolean         = LUA_TBOOLEAN,
    Userdata        = LUA_TUSERDATA,
    LightUserData   = LUA_TLIGHTUSERDATA,
    Function        = LUA_TFUNCTION,
    Thread          = LUA_TTHREAD,
    Nil             = LUA_TNIL
};

/**
 * Lua Type compare result
 **/
enum class LCompare
{
    IsBad,
    IsOK,
    IsConvertable,
    IsUserdata
};

/** Lua Functions { **/

/**
 * Register a C function for Lua
 **/
void LEF_RegisterCFunction(
    LRawState _State, const char* _GlobalName, lua_CFunction _CFunction
);

/**
 * Call Lua function from C
 **/
//// LEF_CallLuaFunction

/** Lua Functions } **/

/** Lua Macros { **/

/**
 * Stack Globals
 **/
#define L_SetGlobal         lua_setglobal
#define L_GetGlobal         lua_getglobal

/**
 * Type Checking
 **/
#define L_IsNumber          lua_isnumber
#define L_IsString          lua_isstring
#define L_IsTable           lua_istable
#define L_IsBoolean         lua_isboolean
#define L_IsUserdata        lua_isuserdata
#define L_IsLightUserData   lua_islightuserdata
#define L_IsFunction        lua_isfunction
#define L_IsThread          lua_isthread
#define L_IsNil             lua_isnil

/**
 * Type getters
 **/
#define L_GetNumber         lua_tonumber
#define L_GetString         lua_tostring
#define L_GetTable          /* no function */
#define L_GetBoolean        lua_toboolean
#define L_GetUserdata       lua_touserdata
#define L_GetLightUserData  /* no function */
#define L_GetFunction       lua_tocfunction
#define L_GetThread         lua_tothread
#define L_GetNil            /* no function */

/**
 * Pushing and Executing scripts on the stack
 **/
#define L_LoadScript(L, S)  LuaCall(L, luaL_loadstring(L, S))
#define L_LoadFile(L, F)    LuaCall(L, luaL_loadfile(L, F))
#define L_RunScript(L, S)   LuaCall(L, luaL_dostring(L, S))
#define L_RunFile(L, F)     LuaCall(L, luaL_dofile(L, F))

/** Lua Macros } **/

#endif