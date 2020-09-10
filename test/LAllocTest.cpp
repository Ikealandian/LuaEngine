//
// Lua Allocation Test
//

#include <stdlib.h>
#include <stdio.h>
#include <lua.hpp>

static void* LAllocator_Alloc(size_t _Size)
{

}

static void* LAllocator_Realloc(void* _Data, size_t _Size)
{

}

static void LAllocator_Free(void* _Data)
{
    
}

static void* LAllocator(void *ud, void *ptr, size_t osize, size_t nsize)
{
    if (nsize == 0)
    {
        free(ptr);
        return NULL;
    }
    else return realloc(ptr, nsize);
}

int main(int argc, char** argv)
{
    lua_State* L;
    lua_setallocf(L, LAllocator, NULL);

    return 0;
}