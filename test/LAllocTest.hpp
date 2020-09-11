//
// Lua Allocation
//

#ifndef _LALLOCTEST_
#define _LALLOCTEST_

#include <stdlib.h>
#include <memory>
#include <map>
#include <lua.hpp>

typedef struct LAlloc
{
    int Total = 0;
    int Allocated = 0;
    std::map<uintptr_t, size_t> Allocations;
}LAlloc;

static inline void* LAllocator_Alloc(size_t _Size, LAlloc* _Alloc)
{
    void* Allocation = malloc(_Size);
    uintptr_t Pointer = reinterpret_cast<uintptr_t>(Allocation);

    _Alloc->Allocated += _Size;
    _Alloc->Total += _Size;

    _Alloc->Allocations[Pointer] = _Size;

    return Allocation;
}

static inline void* LAllocator_Realloc(void* _Data, size_t _Size, LAlloc* _Alloc)
{
    uintptr_t Pointer = reinterpret_cast<uintptr_t>(_Data);
    size_t Difference = _Size - _Alloc->Allocations[Pointer];

    _Alloc->Allocated += Difference;
    _Alloc->Total += (Difference > 0) ? Difference : 0;

    _Alloc->Allocations[Pointer] = _Size;

    return realloc(_Data, _Size);
}

static inline void LAllocator_Free(void* _Data, LAlloc* _Alloc)
{
    uintptr_t Pointer = reinterpret_cast<uintptr_t>(_Data);
    size_t Size = _Alloc->Allocations[Pointer];

    _Alloc->Allocated -= Size;

    _Alloc->Allocations.erase(Pointer);

    free(_Data);
}

static inline void* LAllocator(void* UserData, void* Pointer, size_t oSize, size_t nSize)
{
    LAlloc* lAlloc = static_cast<LAlloc*>(UserData);

    if (nSize == 0)
    {
        LAllocator_Free(Pointer, lAlloc);
        return NULL;
    }

    if (Pointer == NULL)
    {
        return LAllocator_Alloc(nSize, lAlloc);
    }

    return LAllocator_Realloc(Pointer, nSize, lAlloc);
}

#endif