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
#define L_SetField          lua_setfield

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
#define L_GetTop            lua_gettop
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
 * Pushing types onto stack
 **/
#define L_PushIValue        lua_pushvalue
#define L_PushNumber        lua_pushnumber
#define L_PushString        lua_pushstring
#define L_PushTable         lua_newtable
#define L_PushBoolean       lua_pushboolean
#define L_PushUserdata      lua_newuserdata
#define L_PushLightUserData lua_pushlightuserdata
#define L_PushFunction      lua_pushcfunction
#define L_PushThread        lua_pushthread
#define L_PushNil           lua_pushnil
#define L_PushMetaTable     luaL_newmetatable

/**
 * Pushing values onto the Lua stack
 **/
#define L_Push              LuaPush
#define L_PushValue         LuaPushValue
#define L_PushFunc(L, N, F) (L_PushFunction(L, F), L_SetGlobal(L, N))

/**
 * Popping values from the Lua stack
 **/

/**
 * Metatable
 **/
#define L_MakeMeta(L, N)    (luaL_getmetatable(L, N), lua_setmetatable(L, LUA_SECOND))

/**
 * Tables
 **/
#define L_TGet(L, K, S)     (L_Push(L, K), lua_gettable(L, S - 1))
#define L_TPush(L, K, V, S) (L_Push(L, K, V), lua_settable(L, S - 2))
#define L_TPushR(L, K, V, S)(L_Push(L, K, V), lua_rawset(L, S - 2))
#define L_TPushF(L, K, V, S)(L_Push(L, V), L_SetField(L, S - 1, K))

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