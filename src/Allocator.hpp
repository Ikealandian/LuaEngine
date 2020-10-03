//
// Lua Engine Memory Allocator
//

#ifndef _LE_ALLOCATOR_HPP_
#define _LE_ALLOCATOR_HPP_

#include <vector>
#include <inttypes.h>

/**
 * Memory Types
 **/
using Pointer_t = void*;
using Address_t = uintptr_t;
using Bytes_t = size_t;

/**
 * Allocator Data
 *  - Allocation Organization
 *  - Total Allocated (Bytes)
 *  - Total Allocated Lifetime (Bytes)
 **/
typedef struct LE_AllocInfo
{
    int GenericAllocated;
    int ScriptsAllocated;

    int TotalAllocated;
    int LifetimeAllocated;
}Allocation;



////////////////////////
/* Memory Functions { */

/**
 * Allocate 
 *  Allocates a block of memory of size _bSize in Bytes
 **/
Pointer_t Allocate(const Bytes_t& _bSize);

/**
 * Deallocate
 *  Frees a block of memory stating at _pPointer
 **/
void Deallocate(Pointer_t _pPointer);

/** 
 * Reallocate
 *  Reallocates _pPointer to a new size of _bSize
 **/
void Reallocate(Pointer_t* _pPointer, const Bytes_t& _bSize);

/** 
 * Reallocate
 *  Reallocates _pPointer to a new size of _bSize and returns the new address
 **/
Pointer_t Reallocate(Pointer_t _pPointer, const Bytes_t& _bSize);

/* Memory Functions } */
////////////////////////



/**
 * Allocator
 *  - Allocates Memory
 *  - Deallocates Memory
 *  - Tracks usage
 *  - Automatically deallocate unfreed memory on the call of the destructor
 **/
typedef class LE_Allocator
{
private:
    Allocation AllocInfo;
    std::vector<Address_t> Allocs;

public:
    LE_Allocator();
    ~LE_Allocator();

    Pointer_t Alloc(const Bytes_t& _bSize);
    void Free(Pointer_t _Pointer);

}Allocator;



/** TODO: Lua Allocator     **/
/** TODO: Lua Memory Pool   **/

#endif