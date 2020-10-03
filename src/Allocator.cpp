//
// Lua Engine Memory Allocator
//
#include "Allocator.hpp"

// Malloc, Realloc, Free
#include <stdlib.h>

// Allocation Macros
#define LARGE_ALLOCATION  2048

////////////////////////
/* Memory Functions { */

/**
 * Allocate Implementation
 *  Allocates a block of memory of size _bSize in Bytes
 **/
Pointer_t Allocate(const Bytes_t& _bSize)
{
    /** TODO: Use Debugging features    **/
    /** TODO: Warn of large allocation  **/
    if (_bSize > LARGE_ALLOCATION)
        return nullptr;
    
    // Allocate memory
    Pointer_t Ptr = malloc(_bSize);

    /** TODO: Throw error **/
    if (Ptr == nullptr)
        return nullptr;

    // Return pointer
    return Ptr;
}

/**
 * Deallocate Implementation
 *  Frees a block of memory stating at _pPointer
 **/
void Deallocate(Pointer_t _pPointer)
{
    /** TODO: Use Debugging features    **/
    /** TODO: Throw nullptr error       **/
    if (_pPointer == nullptr)
        return;
    
    // Free block
    free(_pPointer);
}

/** 
 * Reallocate Implementation
 *  Reallocates _pPointer to a new size of _bSize and returns the new address
 **/
Pointer_t Reallocate(Pointer_t _pPointer, const Bytes_t& _bSize)
{
    /** TODO: Use Debugging features    **/
    /** TODO: Throw nullptr error       **/
    if (_pPointer == nullptr)
        return nullptr;
    
    /** TODO: Warn of large allocation  **/
    if (_bSize > LARGE_ALLOCATION)
        return nullptr;

    // Reallocate memory
    Pointer_t Ptr = realloc(_pPointer, _bSize);

    /** TODO: Throw nullptr error       **/
    // Failed to reallocate
    if (Ptr == nullptr)
        return nullptr;
    
    // Return new pointer
    return Ptr;
}

/** 
 * Reallocate
 *  Reallocates _pPointer to a new size of _bSize
 **/
void Reallocate(Pointer_t* _pPointer, const Bytes_t& _bSize)
{
    /** TODO: Use Debugging features    **/
    /** TODO: Throw nullptr error       **/
    if (*_pPointer == nullptr)
        return;
    
    /** TODO: Warn of large allocation  **/
    if (_bSize > LARGE_ALLOCATION)
        return;

    // Reallocate memory
    Pointer_t Ptr = realloc(*_pPointer, _bSize);

    /** TODO: Throw nullptr error       **/
    // Failed to reallocate
    if (Ptr == nullptr)
        return;
    
    // Set the new pointer
    *_pPointer = Ptr;
}

/* Memory Functions } */
////////////////////////