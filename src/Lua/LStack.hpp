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
#define L_LoadScript(L, S)  LuaCall(L, luaL_loadstring(L, S))
#define L_LoadFile(L, F)    LuaCall(L, luaL_loadfile(L, F))
#define L_RunScript(L, S)   LuaCall(L, luaL_dostring(L, S))
#define L_RunFile(L, F)     LuaCall(L, luaL_dofile(L, F))

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
inline void LuaPushValue(LRawState _State, T _Value)
{
    // Unused
    (void)(_State);
    (void)(_Value);

    printf("Push\tUnable to push value of type: %s\n", typeid(T).name());
}

/* LuaPushValue: Int */
template<>
inline void LuaPushValue(LRawState _State, int _Number)
{
    lua_pushnumber(_State, _Number);
}

/* LuaPushValue: Double */
template<>
inline void LuaPushValue(LRawState _State, double _Number)
{
    lua_pushnumber(_State, _Number);
}

/* LuaPushValue: String */
template<>
inline void LuaPushValue(LRawState _State, const char* _String)
{
    lua_pushstring(_State, _String);
}

/* LuaPushValue: Boolean */
template<>
inline void LuaPushValue(LRawState _State, bool _Boolean)
{
    lua_pushboolean(_State, _Boolean);
}

/** Push Value } **/
////////////////////



//////////////
/** Push { **/

/**
 * No more arguments to push to the Lua stack
 **/
inline void LuaPush(LRawState) { }

/**
 * Push multiple values onto the Lua stack
 **/
template<typename T, typename ...R>
inline void LuaPush(LRawState _State, const T& _First, const R&... _Rest)
{
    // Push _FirstArg value
    LuaPushValue(_State, _First);

    // Keep calling LuaPush until all args are pushed
    LuaPush(_State, _Rest...);
}

/** Push } **/
//////////////



/////////////
/** Pop { **/

// LuaPopNumber
inline lua_Number LuaPopNumber(LRawState _State)
{
    // Verify top element is a Number
    if (!LuaVerifyType(_State, (int)LTypes::Number, LUA_TOP))
        return (lua_Number)NULL;
    // Get the top element off the Lua stack
    lua_Number Top = lua_tonumber(_State, LUA_TOP);
    // Pop off the top element
    lua_pop(_State, 1);
    // Return Top number
    return Top;
}

// LuaPopString
inline const char* LuaPopString(LRawState _State)
{
    // Verify top element is a String
    if (!LuaVerifyType(_State, (int)LTypes::String, LUA_TOP))
        return NULL;
    // Get the top element off the Lua stack
    const char* Top = lua_tostring(_State, LUA_TOP);
    // Pop off the top element
    lua_pop(_State, 1);
    // Return Top string
    return Top;
}

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
 * Register a C function for Lua
 **/
void LEF_RegisterCFunction(
    LRawState _State, const char* _GlobalName, lua_CFunction _CFunction
);

/**
 * LEF_CallLuaFunction Implementation
 *  Call Lua function from C
 **/
template<typename T, typename ...R>
bool LEF_CallLuaFunction(
    LRawState _State, const LFunction& _Func, const T& _First, const R& ..._Rest
) {
    // Get the function
    L_GetGlobal(_State, _Func.Name);
    // Check if its a function
    if (L_IsFunction(_State, LUA_TOP))
    {
        // Push Arguments
        LuaPush(_State, _First, _Rest...);
        // Call the function
        return LuaCall(_State, lua_pcall(_State, _Func.InputArguments, _Func.ReturnValues, 0));
    } else /* Not a Function */ {
        // Cant call a nonFunction
        printf("CallFun\tUnable to call a non-Function value\n");
        return false;
    }
}

/** Lua Functions } **/
///////////////////////



#endif