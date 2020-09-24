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

struct Lua_Vector2
{
    lua_Number x, y;

    static void Init(LRawState L)
    {
        // Define the Vector metatable
        L_PushMetaTable(L, "LuaVec2MetaTable");

        // Push destroy (garbage collection)
        L_TPush(L, "__gc", Lua_Vector2::Destroy, LUA_TOP);
        // Push add function
        L_TPush(L, "__add", Lua_Vector2::LuaAdd, LUA_TOP);
        // Push sub function
        L_TPush(L, "__sub", Lua_Vector2::LuaSub, LUA_TOP);

        // Create a new table
        L_PushTable(L);

        // Get Table position on the Lua stack
        int TablePosition = L_GetTop(L);
        // Push index back
        L_PushIValue(L, TablePosition);

        // Give the table a name
        L_SetGlobal(L, "Vec2");

        // Push create function onto Vector2 table
        L_TPushF(L, "new", Lua_Vector2::Create, LUA_TOP);
        // Push zero function onto Vector2 table
        L_TPushF(L, "zero", Lua_Vector2::CreateZero, LUA_TOP);
    }

    static int Create(LRawState L)
    {
        // Get the X, Y arguments
        lua_Number Y = LuaPopNumber(L);
        lua_Number X = LuaPopNumber(L);

        // Create new Raw vector
        return Lua_Vector2::CreateRaw(L, X, Y);
    }

    static int CreateZero(LRawState L)
    {
        return Lua_Vector2::CreateRaw(L, .0, .0);
    }

    static int CreateRaw(LRawState L, lua_Number X, lua_Number Y)
    {
        // Push a new empty Lua table
        L_PushTable(L);

        // Push item x to be the value of X on the table
        // and item y to be the value of Y on the table
        L_TPush(L, "x", X, LUA_TOP);
        L_TPush(L, "y", Y, LUA_TOP);

        // Make the top Table a metatable 
        L_MakeMeta(L, "LuaVec2MetaTable");

        return 1;
    }

    static int Destroy(LRawState)
    {
        puts("Lua Vec2 Destroyed by Garbage Collection...");
        return 0;
    }

    static Lua_Vector2 GetValues(LRawState L, int Table)
    {
        // Get left X coord
        L_TGet(L, "x", Table);
        lua_Number X = LuaPopNumber(L);
        // Get left Y coord
        L_TGet(L, "y", Table);
        lua_Number Y = LuaPopNumber(L);

        return { X, Y };
    }

    static void Debug(LRawState L)
    {
        // Verify 
        printf("LuaVec\tDebugging {\n");
        LuaVerifyType(L, LTypes::Table, LUA_FIRST, LE_DebugData);
        LuaVerifyType(L, LTypes::Table, LUA_SECOND, LE_DebugData);
        printf("LuaVec\t}\n");
    }

    static int LuaAdd(LRawState L)
    {
        // Right    LUA_SECOND
        // Left     LUA_FIRST
        Debug(L); // Make sure both sides are a table

        // Get the Right and Left vector
        Lua_Vector2 Left = Lua_Vector2::GetValues(L, LUA_FIRST);
        Lua_Vector2 Right = Lua_Vector2::GetValues(L, LUA_SECOND);

        // Add the numbers
        lua_Number NewX = Right.x + Left.x;
        lua_Number NewY = Right.y + Left.y;

        // Push new table onto the stack
        return Lua_Vector2::CreateRaw(L, NewX, NewY);
    }

    static int LuaSub(LRawState L)
    {
        // Right    LUA_SECOND
        // Left     LUA_FIRST
        Debug(L); // Make sure both sides are a table

        // Get the Right and Left vector
        Lua_Vector2 Left = Lua_Vector2::GetValues(L, LUA_FIRST);
        Lua_Vector2 Right = Lua_Vector2::GetValues(L, LUA_SECOND);

        // Subtract the numbers
        lua_Number NewX = Right.x - Left.x;
        lua_Number NewY = Right.y - Left.y;

        // Push new table onto the stack
        return Lua_Vector2::CreateRaw(L, NewX, NewY);
    }

};

int main(int, char**)
{
    // Automatically Creates and Destroys
    LState L;

    // Register LuaPrint
    L_PushFunc(L, "print", LuaPrint);

    std::string Script = R"(
        function add(a, b)
            return a + b
        end

        Vector1 = Vec2.new(2, 4)
        Vector2 = Vec2.new(6, 8)
        Vector3 = Vector1 - Vector2 

        print("Vector3:\n X: ", Vector3.x, "\n Y: ", Vector3.y)
        print("add(6, 4) = ", add(6, 4))
    )";

    // Init Lua Vectors
    Lua_Vector2::Init(L);

    // Load and run script
    if (!L_RunScript(L, Script.c_str()))
        return -1;

    // Define Lua function
    LFunction AddFunc       = {};
    AddFunc.Name            = "add";
    AddFunc.InputArguments  = 2;
    AddFunc.ReturnValues    = 1;

    // Call add from Lua
    if (L_CallFunc(L, AddFunc, 4, 4))
    {
        double Result = LuaPopNumber(L);
        printf("add(4, 4) = %d\n", (int)Result);
    }

    return 0;
}