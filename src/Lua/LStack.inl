//
// Lua Stack Inline Manipulators
//

// _inl LStack Impl



/**
 * IValue Literal
 **/
inline IValue operator "" iv(unsigned long long _iv)
{
    return (IValue)_iv;
}



////////////////////
/** Push Value { **/

/* Inline Impl */
/**
 * Push a value onto the Lua stack
 **/
template<typename T>
inline void LuaPushValue(LRawState, T)
{
    printf("Push\tUnable to push value of type: %s\n", typeid(T).name());
}

/* Inline Impl */
/* LuaPushValue: Int */
template<>
inline void LuaPushValue(LRawState _State, int _Number)
{
    L_PushNumber(_State, _Number);
}

/* LuaPushValue: IValue */
template<>
inline void LuaPushValue(LRawState _State, IValue _IdxValue)
{
    // Lua push index value
    L_PushIValue(_State, _IdxValue);
}

/* Inline Impl */
/* LuaPushValue: Double */
template<>
inline void LuaPushValue(LRawState _State, double _Number)
{
    L_PushNumber(_State, _Number);
}

/* Inline Impl */
/* LuaPushValue: String */
template<>
inline void LuaPushValue(LRawState _State, const char* _String)
{
    L_PushString(_State, _String);
}

/* Inline Impl */
/* LuaPushValue: Boolean */
template<>
inline void LuaPushValue(LRawState _State, bool _Boolean)
{
    L_PushBoolean(_State, _Boolean);
}

/* Inline Impl */
/* LuaPushValue: CFunction */
template<>
inline void LuaPushValue(LRawState _State, lua_CFunction _Function)
{
    L_PushFunction(_State, _Function);
}

/** Push Value } **/
////////////////////



//////////////
/** Push { **/

/* Inline Impl */
/**
 * No more arguments to push to the Lua stack
 **/
inline void LuaPush(LRawState)
{
    // Do nothing
    return;
}

/* Inline Impl */
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

/* Inline Impl */
/**
 * Template Pop
 **/
template<typename T, LTypes LT>
inline T LuaPop(LRawState)
{
    printf("Pop \tUnable to pop value of null type.\n");
    return nullptr;
}

/* Inline Impl */
// Pop Number
template<LTypes LT = LTypes::Number>
inline lua_Number LuaPop(LRawState _State)
{
    return LuaPopNumber(_State);
}

/* Inline Impl */
// LuaPopNumber
inline lua_Number LuaPopNumber(LRawState _State)
{
    // Verify top element is a Number
    if (!LuaVerifyType(_State, LTypes::Number, LUA_TOP, LE_DebugData))
        return (lua_Number)NULL;
    // Get the top element off the Lua stack
    lua_Number Top = lua_tonumber(_State, LUA_TOP);
    // Pop off the top element
    lua_pop(_State, 1);
    // Return Top number
    return Top;
}

/* Inline Impl */
// LuaPopString
inline const char* LuaPopString(LRawState _State)
{
    // Verify top element is a String
    if (!LuaVerifyType(_State, LTypes::String, LUA_TOP, LE_DebugData))
        return NULL;
    // Get the top element off the Lua stack
    const char* Top = L_GetString(_State, LUA_TOP);
    // Pop off the top element
    lua_pop(_State, 1);
    // Return Top string
    return Top;
}

/* Inline Impl */
// LuaPopUserdata
inline void* LuaPopUserdata(LRawState _State)
{
    // Verify top element is a String
    if (!LuaVerifyType(_State, LTypes::Userdata, LUA_TOP, LE_DebugData))
        return NULL;
    // Get the top element off the Lua stack
    void* Top = L_GetUserdata(_State, LUA_TOP);
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
 * LEF_CallLuaFunction Implementation
 *  Call Lua function from C
 * TODO:
 *  LE_DebugData?
 **/
template<typename T, typename ...R>
inline bool LEF_CallLuaFunction(
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
        return LuaCall(_State, lua_pcall(_State, _Func.InputArguments, _Func.ReturnValues, 0), LE_DebugData);
    } else /* Not a Function */ {
        // Cant call a nonFunction
        printf("CallFun\tUnable to call a non-Function value\n");
        return false;
    }
}

/** Lua Functions } **/
///////////////////////