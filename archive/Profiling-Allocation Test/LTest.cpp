//
// Simple Lua Application
//

#include <stdlib.h>
#include <stdio.h>
#include <lua.hpp>

#include "LAllocTest.hpp"
#include "LProfiling.hpp"

lua_State* gL = nullptr;

void LPrintError(const char* _Error);
bool LuaCheckResult(int _Result);
double LuaGetNumber(const char* _Global);
const char* LuaGetString(const char* _Global);

#ifdef _WIN32
#   include <crtdbg.h>
#   define LBREAK           _CrtDbgBreak()
#else
#   include <csignal>
#   define LBREAK           std::raise(SIGINT)
#endif

#define LASSERT(V, M)       if(!(V)) { LERROR(M); LBREAK; }

#define LERROR              LPrintError
#define LASSERT_ERROR(M)    LERROR(M); LBREAK;

#define LCALL(R)            LuaCheckResult(R)

#define LDOSTR(S)           LCALL(luaL_dostring(gL, S))
#define LDOFILE(S)          LCALL(luaL_dofile(gL, S))

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

    // Open Lua libraries
	luaL_openlibs(gL);

    // Use custom allocator
    LAlloc Alloc;
    lua_setallocf(gL, LAllocator, &Alloc);

    // Create a profiler
    Profiler Profile = &Alloc;

    {
        // Profile the scope
        auto _ = Profile.ProfileThisScope("LuaTest");

        // Lua run program
        LDOFILE(argv[1]);
    }

    // Print scope profile data
    Profile.PrintProfileData("LuaTest");

    // Print allocated
    printf("Allocated: %d\n", Alloc.Allocated);

    // Close Lua state
    lua_close(gL);

    // Print allocated
    printf("Closed\nAllocated: %d\nTotal: %d\n", Alloc.Allocated, Alloc.Total);

    return 0;
}

void LPrintError(const char* _Error)
{
    fflush(stderr);
    fprintf(stderr, "Lua [ERROR]: %s\n", _Error);
}

bool LuaCheckResult(int _Result)
{
    // LuaCall == OK
    if(_Result == LUA_OK)
        return true;
    
    // Else LuaCall != OK
    // Print Error at top of stack
    LASSERT_ERROR(LTOSTRTOP());

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