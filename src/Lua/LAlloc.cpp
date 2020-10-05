//
// Lua Allocators
//

// Include Lua Alloc
#include <Lua/LAlloc.hpp>

// For Allocation functions
#include <Allocator.hpp>
#include <stdlib.h>

/**
 * Implementation
 * A Lua allocator function utilizing standard Malloc, Realloc and Free
 *  No userdata is required to be passed through to be used
 **/
Pointer_t LE_Default_LAlloc(Pointer_t /* _pAllocator */, Pointer_t _pPointer, Bytes_t /* _oSize */, Bytes_t _nSize)
{
    // if freeing the memory
    if (_nSize == 0)
    {
        Deallocate(_pPointer);
        return NULL;
    }
    
    // if allocating new memory
    if (_pPointer == nullptr)
    {
        return Allocate(_nSize);
    }

    // else reallocate
    return Reallocate(_pPointer, _nSize);
}

/**
 * Implementation
 * A Lua allocator function utilizing the Allocator class
 *  Pass through to lua_newstate with an Allocator class as the userdata
 **/
Pointer_t LE_Allocator_LAlloc(Pointer_t _pAllocator, Pointer_t _pPointer, Bytes_t _oSize, Bytes_t _nSize)
{
    // If an Allocator wasn't passed
    // Use the default lua allocator
    if (_pAllocator == nullptr)
        return LE_Default_LAlloc(_pAllocator, _pPointer, _oSize, _nSize);

    // Get the allocator
    Allocator* LAlloc = reinterpret_cast<Allocator*>(_pAllocator);
    
    // if freeing the pointer
    if (_nSize == 0)
    {
        LAlloc->Free(_pPointer);
        return NULL;
    }

    // if the pointer doesnt exist yet
    if (_pPointer == nullptr)
    {
        return LAlloc->Alloc(_nSize);
    }

    // Otherwise reallocate
    return LAlloc->Realloc(_pPointer, _nSize);
}