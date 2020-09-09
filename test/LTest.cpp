//
// Simple Lua Application
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lua.hpp>

lua_State* gL = nullptr;

void LPrintError(const char* _Error);
bool LuaCheckResult(int _Result);
double LuaGetNumber(const char* _Global);
const char* LuaGetString(const char* _Global);

#define LERROR              LPrintError
#define LASSERT_ERROR(M)    LERROR(M); assert(false)

#define LCALL_RELEASE(R)    LuaCheckResult(R)
#define LCALL_DEBUG(R)      assert(LuaCheckResult(R))
#define LCALL               LCALL_DEBUG

#define LDOSTR(S)           LCALL(luaL_dostring(gL, S))

#define LISNUM(I)           lua_isnumber(gL, I)
#define LISNUMTOP()         LISNUM(-1)
#define LTONUM(I)           lua_tonumber(gL, I)
#define LTONUMTOP()         LTONUM(-1)

#define LISSTR(I)           lua_isstring(gL, I)
#define LISSTRTOP()         LISSTR(-1)
#define LTOSTR(I)           lua_tostring(gL, I)
#define LTOSTRTOP()         LTOSTR(-1)

#define LGET(S)             lua_getglobal(gL, S)
#define LGETNUM(S)          LuaGetNumber(S)
#define LGETSTR(S)          LuaGetString(S)

int main(int argc, char** argv)
{
    // Create Lua state
    gL = luaL_newstate();

    // Lua run program
    const char* Program = "a = 100 + 2";
    LDOSTR(Program);

    // Get A result
    const char* A = LGETSTR("a");
    printf("%s\na = %f\n", Program, A);

    // Close Lua state
    lua_close(gL);

    return 0;
}

void LPrintError(const char* _Error)
{
    printf("Lua [ERROR]: %s", _Error);
}

bool LuaCheckResult(int _Result)
{
    // LuaCall == OK
    if(_Result == LUA_OK)
        return true;
    
    // Else LuaCall != OK
    // Print Error at top of stack
    LERROR(LTOSTRTOP());

    return false;
}

double LuaGetNumber(const char* _Global)
{
    LGET(_Global);
    if (LISNUMTOP()) return LTONUMTOP();
    LASSERT_ERROR("Tried to access data by invalid type. (Number)");
}

const char* LuaGetString(const char* _Global)
{
    LGET(_Global);
    if (LISSTRTOP()) return LTOSTRTOP();
    LASSERT_ERROR("Tried to access data by invalid type. (String)");
}