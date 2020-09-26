//
// Lua Debug
//

// Include Lua Stack
#include <Lua/LStack.hpp>

// Include Lua Debug
#include <Lua/LDebug.hpp>

/**
 * LEF_CallLuaFunction Implementation
 *  Push new global Lua table
 **/
IValue LEF_NewGlobalTable(LRawState _State, const char* _Name)
{
    // Push new table
    L_PushTable(_State);
    // Get table position on Lua stack
    IValue Position = L_GetTop(_State);
    // Push value of table index
    L_PushIValue(_State, Position);
    // Set the table name
    L_SetGlobal(_State, _Name);
    // Return table position
    return Position;
}