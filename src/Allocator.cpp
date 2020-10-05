//
// Lua Engine Memory Allocator
//
#include "Allocator.hpp"

// Malloc, Realloc, Free
#include <stdlib.h>

// Printf
#include <stdio.h>

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
    // : if (_bSize > LARGE_ALLOCATION)
    // :     return nullptr;
    
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



/////////////////
/* Allocator { */

/** Implementation **/
LE_Allocator::LE_Allocator()
    : AllocInfo({0,0,0,0})
{
}

/** Implementation **/
LE_Allocator::~LE_Allocator()
{
    /* Free all Allocations */
    FreeAll();
}

/** Implementation                                  **/
/** Frees all allocated memory by this Allocator    **/
void LE_Allocator::FreeAll(void)
{
    // Loop through all active allocations
    for (auto& Alloc : Allocs)
    {
        // Free current allocation
        Free(reinterpret_cast<Pointer_t>(Alloc.first));
    }

    // Clear all
    Allocs.clear();
}

/** Implementation              **/
/** Print out Allocation data   **/
void LE_Allocator::PrintInfo(void)
{
    printf("Alloc:\n\tLifetime: %dB\n\tTotal: %dB\n\tActive: %ld\n",
     AllocInfo.LifetimeAllocated,
     AllocInfo.TotalAllocated,
     Allocs.size()
    );
}

/** Implementation  **/
/** Calls Allocate  **/
Pointer_t LE_Allocator::Alloc(const Bytes_t& _bSize)
{
    // Allocate
    Pointer_t Allocation = Allocate(_bSize);
    
    // Save address and size in active allocations
    Allocs[reinterpret_cast<Address_t>(Allocation)] = _bSize;

    // Add to total allocated
    AllocInfo.TotalAllocated += _bSize;
    AllocInfo.LifetimeAllocated += _bSize;

    // Return pointer
    return Allocation;
}

/** Implementation      **/
/** Calls Reallocate    **/
Pointer_t LE_Allocator::Realloc(Pointer_t _pPointer, const Bytes_t& _bSize)
{
    // Get Address_t
    Address_t Address = reinterpret_cast<Address_t>(_pPointer);

    // Get the old size of pointer
    Bytes_t OldSize = Allocs[Address];

    // Get size difference
    Bytes_t DifSize = _bSize - OldSize;

    // Add the size difference
    AllocInfo.LifetimeAllocated += (DifSize > 0) ? DifSize : 0;
    AllocInfo.TotalAllocated += DifSize;

    // Reallocate
    Pointer_t NewPointer = Reallocate(_pPointer, _bSize);

    // If the new pointer is different
    if (NewPointer != _pPointer)
    {
        // Get the new pointers address
        Address_t NewAddress = reinterpret_cast<Address_t>(NewPointer);

        // Erase old data
        Allocs.erase(Address);

        // Set new data
        Allocs[NewAddress] = _bSize;
    }

    // Return new pointer
    return NewPointer;
}

/** Implementation      **/
/** Calls Deallocate    **/
void LE_Allocator::Free(Pointer_t _pPointer)
{
    // Get Address_t
    Address_t Address = reinterpret_cast<Address_t>(_pPointer);

    // Get size of pointer
    Bytes_t Size = Allocs[Address];

    // Sub from total allocated
    AllocInfo.TotalAllocated -= Size;

    // Deallocate
    Deallocate(_pPointer);

    // Remove from active allocations
    Allocs.erase(Address);
}

/* Allocator } */
/////////////////