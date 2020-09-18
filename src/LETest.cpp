// 
// Lua Engine Testing     
//

#include <LEngine.hpp>
#include <string>

const char* GetLuaString(LRawState L, int _StackIndex)
{
    switch ((LTypes)lua_type(L, _StackIndex))
    {
    case LTypes::Number:
    case LTypes::String:
        return L_GetString(L, _StackIndex);
    case LTypes::Boolean:
        return (L_GetBoolean(L, _StackIndex) ? "true" : "false");
    case LTypes::Nil:
        return "nil";
    default: break;
    }
    return "(unprintable data)";
}

int LuaPrint(LRawState L)
{
    // Get # of arguments passed
    int Arguments = lua_gettop(L);

    // Loop through arguments
    for (int i = Arguments - 1; i > -1; i--)
    {
        // Get string version of the item at the top of the stack
        const char* String = GetLuaString(L, LUA_TOP - i);
        // Print the value
        fputs(String, stdout);
    }

    // New line
    fputc('\n', stdout);
    
    return 1;
}

int main(int, char**)
{
    // Automatically Creates and Destroys
    LState L;

    std::string Script = R"(
        print("LUA", "Hello!")
    )";

    L_RunScript(L, Script.c_str());

    return 0;
}