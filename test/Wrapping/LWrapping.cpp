//
// Wrapping Testing
//

#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <string>
#include <fstream>

// Linux Only
#include <libgen.h>

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
void LuaPushValue(lua_State* _State, T _Value)
{
    // Unused
    (void)(_State);
    (void)(_Value);

    printf("Lua (LuaPushValue) [Error]: Unable to push value of type: %s\n", typeid(T).name());
}

/* LuaPushValue: Int */
template<>
void LuaPushValue(lua_State* _State, int _Number)
{
    lua_pushnumber(_State, _Number);
}

/* LuaPushValue: Double */
template<>
void LuaPushValue(lua_State* _State, double _Number)
{
    lua_pushnumber(_State, _Number);
}

/* LuaPushValue: String */
template<>
void LuaPushValue(lua_State* _State, const char* _String)
{
    lua_pushstring(_State, _String);
}

/* LuaPushValue: Boolean */
template<>
void LuaPushValue(lua_State* _State, bool _Boolean)
{
    lua_pushboolean(_State, _Boolean);
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

/**
 * Pop a value off of the Lua stack
 **/

// LuaPopNumber
lua_Number LuaPopNumber(lua_State* _State)
{
    lua_Number Top = lua_tonumber(_State, LUA_TOP);
    lua_pop(_State, 1);

    return Top;
}

// LuaPopString
std::string LuaPopString(lua_State* _State)
{
    std::string Top = lua_tostring(_State, LUA_TOP);
    lua_pop(_State, 1);

    return Top;
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

/**
 * Lua function defintion
 **/
typedef struct _LFuncDef
{
    std::string Name;
    int ReturnValues;
    int InputArguments;
}Function;

/**
 * Call a Lua function from C++
 **/
template<typename First, typename ... Others>
void LuaCallFunction(lua_State* _State, const Function& _Function, First _First, const Others& ... _Other)
{
    // Get the function
    L_GetGlobal(_State, _Function.Name.c_str());

    // Check if its a function
    if (L_IsFunction(_State, LUA_TOP))
    {
        // Push Arguments
        LuaPush(_State, _First, _Other ...);

        // Call the function
        lua_pcall(_State, _Function.InputArguments, _Function.ReturnValues, 0);
    }
    // Not a Function
    else
    {
        printf("Lua (LuaCallFunction) [Error]: Unable to call a non-Function value\n");
    }
}

/**
 * Register a C++ function for Lua
 **/
void LuaRegisterFunction(lua_State* _State, const char* _Name, lua_CFunction _Function)
{
    // Push the function to the stack
    lua_pushcfunction(_State, _Function);

    // Name the function
    lua_setglobal(_State, _Name);
}

/**
 * Functions to give to Lua
 **/

/* Lua Write to File */
int Lua_WriteFile(lua_State* L)
{
    static std::string Path = "test/Wrapping/tmp/test_tmp/";

    std::string NewContents = LuaPopString(L);
    std::string OriginalPath = LuaPopString(L);
    std::string NewPath = basename(OriginalPath.data());
    NewPath = Path + NewPath;

    printf("C++\tWriting to: %s\n", NewPath.c_str());

    std::ofstream Stream;
    Stream.open(NewPath);

    if (Stream.is_open())
    {
        Stream << NewContents;
        LuaPush(L, true);
    }
    else
    {
        // Failed to open file
        LuaPush(L, false);
    }

    Stream.close();

    return 1;
}

/* Lua Read */
int Lua_ReadFile(lua_State* L)
{
    static std::string Path = "test/Wrapping/tmp/test_tmp/";

    std::string OriginalPath = LuaPopString(L);
    std::string NewPath = basename(OriginalPath.data());
    NewPath = Path + NewPath;

    printf("C++\tReading from: %s\n", NewPath.c_str());

    std::ifstream Stream;
    Stream.open(NewPath);

    if (Stream.is_open())
    {
        std::string FileContents;
        std::string Line;
        while (getline(Stream, Line))
        {
            FileContents += Line;
            FileContents += '\n';
        }
        LuaPush(L, true, FileContents.c_str());
    }
    else
    {
        // Failed to open file
        LuaPush(L, false, NULL);
    }

    Stream.close();

    return 2;
}

int main()
{
    // Create a new Lua state
    // NOTE: State automatically closes at end of scope
    LStatePtr L;
    
    // Open Lua libraries
    luaL_openlibs(L);

    // Register C++ Functions for Lua
    // Write File
    LuaRegisterFunction(L, "FileW", Lua_WriteFile);

    // Read File
    LuaRegisterFunction(L, "FileR", Lua_ReadFile);


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

    // Same as last time but calling function with a String argument
    L_GetGlobal(L, "LuaPrint");
    if (L_IsFunction(L, LUA_TOP))
    {
        LuaPush(L, "Giving this message to Lua for printing...");
        lua_pcall(L, 1, 0, 0);
    }

    L_GetGlobal(L, "Square");
    if (L_IsFunction(L, LUA_TOP))
    {
        // Push mnumber 5 into function Square
        // expect 5 * 5 as return value
        LuaPush(L, 5);

        // 1 -> Function takes 1 value
        // 1 -> Function returns 0 values
        // 0 -> still an "errfunc"
        lua_pcall(L, 1, 1, 0);

        // Returned number should be at the top of the stack
        if (L_IsNumber(L, LUA_TOP))
        {
            double Square = lua_tonumber(L, LUA_TOP);
            printf("C++\tSquare(5) = %f\n", Square);
        }
    }

    // Define the Square function
    Function LuaSquare          = { };
    LuaSquare.Name              = "Square";
    LuaSquare.InputArguments    = 1;
    LuaSquare.ReturnValues      = 1;

    // Call the Square function
    // State, Function, Any Arguments to Push ...
    LuaCallFunction(L, LuaSquare, 10);

    // Get Return value at LUA_TOP
    double Square = LuaPopNumber(L);
    printf("C++\tSquare(10) = %f\n", Square);

    return 0;
}