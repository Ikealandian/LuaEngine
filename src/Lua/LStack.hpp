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

// typeinfo
#include <typeinfo>

/* Lua stack positions */
#define LUA_TOP         (-1)
#define LUA_FIRST       (LUA_TOP - 0)
#define LUA_SECOND      (LUA_TOP - 1)
#define LUA_THIRD       (LUA_TOP - 2)

/**
 * Lua Types
 **/
enum class LTypes : int
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
enum class LCompare : int
{
    IsBad,
    IsOK,
    IsConvertable,
    IsUserdata
};



////////////////////
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
#define L_LoadScript(L, S)  LuaCall(L, luaL_loadstring(L, S), LE_DebugData)
#define L_LoadFile(L, F)    LuaCall(L, luaL_loadfile(L, F), LE_DebugData)
#define L_RunScript(L, S)   LuaCall(L, luaL_dostring(L, S), LE_DebugData)
#define L_RunFile(L, F)     LuaCall(L, luaL_dofile(L, F), LE_DebugData)

/**
 * Lua function macros
 **/
#define L_CallFunc          LEF_CallLuaFunction
#define L_RegFunc           LEF_RegisterCFunction

/** Lua Macros } **/
////////////////////


////////////////////
/** Push Value { **/

/**
 * Push a value onto the Lua stack
 **/
template<typename T>
void LuaPushValue(
    LRawState _State, T _Value
);

/* LuaPushValue: Int */
template<>
void LuaPushValue(
    LRawState _State, int _Number
);

/* LuaPushValue: Double */
template<>
void LuaPushValue(
    LRawState _State, double _Number
);

/* LuaPushValue: String */
template<>
void LuaPushValue(
    LRawState _State, const char* _String
);

/* LuaPushValue: Boolean */
template<>
void LuaPushValue(
    LRawState _State, bool _Boolean
);

/* LuaPushValue: CFunction */
template<>
void LuaPushValue(
    LRawState _State, lua_CFunction _Function
);

/** Push Value } **/
////////////////////



//////////////
/** Push { **/

/**
 * No more arguments to push to the Lua stack
 **/
void LuaPush(
    LRawState
);

/**
 * Push multiple values onto the Lua stack
 **/
template<typename T, typename ...R>
void LuaPush(
    LRawState _State, const T& _First, const R&... _Rest
);

/** Push } **/
//////////////



/////////////
/** Pop { **/

/**
 * Pop a Lua number off the top of the stack
 **/
lua_Number LuaPopNumber(
    LRawState _State
);

/**
 * Pop a Lua string off the top of the stack
 **/
const char* LuaPopString(
    LRawState _State
);

/** Pop } **/
/////////////



///////////////////////
/** Lua Functions { **/

/**
 * Lua Function data
 **/
typedef struct _LFuncDef
{
    const char* Name;
    int ReturnValues;
    int InputArguments;
}LFunction;

/**
 * LEF_CallLuaFunction Implementation
 *  Call Lua function from C
 **/
template<typename T, typename ...R>
bool LEF_CallLuaFunction(
    LRawState _State, const LFunction& _Func, const T& _First, const R& ..._Rest
);

/** Lua Functions } **/
///////////////////////

#include <Lua/LStack.inl>

#endif