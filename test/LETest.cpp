// 
// Lua Engine Testing     
//

#include <LEngine.hpp>
#include <string>
#include <string.h>
#include <new>

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
    int Arguments = L_GetTop(L);

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
        // Make into a macro? //
        /* {

        // Create a new table
        L_PushTable(L);

        // Get Table position on the Lua stack
        IValue TablePosition = L_GetTop(L);
        // Push index back
        L_PushIValue(L, TablePosition);

        // Give the table a name
        L_SetGlobal(L, "Vec2");

        } */
        // Marcro:

        // Push a new global Lua table
        // And name it "Vec2"
        L_PushGTable(L, "Vec2");
        // IVec2 equal to the table position on stack

        // Push create function onto Vector2 table
        L_TablePushField(L, "new", Lua_Vector2::Create, LUA_TOP);
        // Push zero function onto Vector2 table
        L_TablePushField(L, "zero", Lua_Vector2::CreateZero, LUA_TOP);

        // Define the Vector metatable
        L_PushMetaTable(L, "LuaVec2MetaTable");

        // Push destroy (garbage collection)
        L_TablePush(L, "__gc", Lua_Vector2::Destroy, LUA_TOP);
        // Push add function
        L_TablePush(L, "__add", Lua_Vector2::LuaAdd, LUA_TOP);
        // Push sub function
        L_TablePush(L, "__sub", Lua_Vector2::LuaSub, LUA_TOP);
    }

    static int Create(LRawState L)
    {
        if(L_IsType(L, LTypes::Table, LUA_TOP))
        {
            Lua_Vector2 CopyVec2 = Lua_Vector2::GetValues(L, LUA_TOP);
            return Lua_Vector2::CreateRaw(L, CopyVec2.x, CopyVec2.y);
        }

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
        L_TablePushRaw(L, "x", X, LUA_TOP);
        L_TablePushRaw(L, "y", Y, LUA_TOP);

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
        // Get X coord
        L_TableGet(L, "x", Table);
        lua_Number X = LuaPopNumber(L);
        // Get Y coord
        L_TableGet(L, "y", Table);
        lua_Number Y = LuaPopNumber(L);
        // Return
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

class Sprite
{
private:
    int x, y;

public:
    static void Init(LRawState L)
    {
        // New global table
        L_PushGTable(L, "Sprite");
        // Push field onto Sprite table
        L_TablePushField(L, "new",      Sprite::LCreate,    LUA_TOP);
        L_TablePushField(L, "destroy",  Sprite::LDestroy,   LUA_TOP);
        L_TablePushField(L, "move",     Sprite::LMove,      LUA_TOP);
        L_TablePushField(L, "draw",     Sprite::LDraw,      LUA_TOP);

        // New Sprite Metatable
        L_PushMetaTable(L, "LuaSpriteMetaTable");
        // Push __index
        L_TablePush(L, "__index", Sprite::LIndex, LUA_TOP);
        // push __newindex
        L_TablePush(L, "__newindex", Sprite::LNewIndex, LUA_TOP);
        // Push __gc (Garbage Collection)
        L_TablePush(L, "__gc", Sprite::LDestroy, LUA_TOP);
    }

    static int LCreate(LRawState L)
    {
        // Create new Sprite
        Sprite* pSprite = (Sprite*)L_PushUserdata(L, sizeof(Sprite));
        new (pSprite) Sprite();

        // Make it a Metatable
        L_MakeMeta(L, "LuaSpriteMetaTable");

        // New Table
        L_PushTable(L);
        // Set uservalue
        lua_setuservalue(L, 1);

        return 1;
    }

    static int LDestroy(LRawState L)
    {
        Sprite* pSprite = (Sprite*)L_GetUserdata(L, LUA_TOP);
        pSprite->~Sprite();
        return 0;
    }

    static int LMove(LRawState L)
    {
        lua_Number Y = LuaPopNumber(L);
        lua_Number X = LuaPopNumber(L);
        Sprite* pSpr = (Sprite*)LuaPopUserdata(L);

        // Move
        pSpr->Move(X, Y);
        return 0;
    }

    static int LDraw(LRawState L)
    {
        Sprite* pSpr = (Sprite*)L_GetUserdata(L, LUA_FIRST);
        pSpr->Draw();
        return 0;
    }

    static int LNewIndex(LRawState L)
    {
        // -1 Data 3iv
        // -2 Name 2iv
        // -3 Userdata 1iv

        lua_getuservalue(L, 1);
        L_TablePush(L, 2_iv, 3_iv, LUA_TOP);

        return 0;
    }

    static int LIndex(LRawState L)
    {
        puts("Lua Sprite Indexed.");

        const char* Index = LuaPopString(L);
        Sprite* pSprite = (Sprite*)L_GetUserdata(L, LUA_TOP);

        if(strcmp(Index, "x") == 0)
        {
            L_Push(L, pSprite->x);
            return 1;
        }

        if(strcmp(Index, "y") == 0)
        {
            L_Push(L, pSprite->y);
            return 1;
        }

        lua_getuservalue(L, 1);
        L_TableGetRaw(L, 2_iv, LUA_TOP);

        if (L_IsNil(L, LUA_TOP))
        {
            L_GetGlobal(L, "Sprite");
            L_TableGetRaw(L, Index, LUA_TOP);
        }

        return 1;
    }

    void Move(int _x, int _y)
    {
        x += _x;
        y += _y;
    }

    void Draw()
    {
        printf("Sprite:\n X: %d\n Y: %d\n", x, y);
    }

    Sprite()
        : x(0), y(0)
    {
        puts("Lua Sprite Created.");
    }

    ~Sprite()
    {
        puts("Lua Sprite Destroyed.");
    }
    
};

int main(int, char**)
{
    // Create a new Allocator
    Allocator Alloc;

    // Automatically Creates and Destroys
    // Create LState with a custom Allocator
    LState L(Alloc);

    // Register LuaPrint
    L_PushFunc(L, "print", LuaPrint);

    const char* Script = R"(
        function add(a, b)
            return a + b
        end

        Vector1 = Vec2.new(2, 4)
        Vector2 = Vec2.new(6, 8)
        Vector3 = Vector1 - Vector2

        Vector4 = Vec2.new(Vector3)
        Vector4.x = Vector4.x + 4

        sprite = Sprite.new()
        sprite.v = 99
        sprite:draw()
        sprite:move(Vector4.x, Vector4.y)
        sprite:draw()

        print("LSprite:\n X: ", sprite.v, "\n Y: ", sprite.y)
        print("add(6, 4) = ", add(6, 4))
    )";

    // Init Lua Vectors
    Lua_Vector2::Init(L);
    Sprite::Init(L);

    // Load and run script
    if (!L_RunScript(L, Script))
        return -1;

    Alloc.PrintInfo();

    // Define Lua function
    LFunction AddFunc       = {};
    AddFunc.Name            = "add";
    AddFunc.InputArguments  = 2;
    AddFunc.ReturnValues    = 1;

    // Call add from Lua
    if (L_CallFunc(L, AddFunc, 4, 4))
    {
        double Result = LuaPop<LTypes::Number>(L);
        printf("add(4, 4) = %d\n", (int)Result);
    }

    // Close the state and check Allocation info
    L.Close();
    Alloc.PrintInfo();

    return 0;
}