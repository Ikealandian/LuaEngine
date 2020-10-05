//
// Lua Allocators
//

#ifndef _LE_LUA_ALLOCATORS_HPP_
#define _LE_LUA_ALLOCATORS_HPP_

// Lua Engine Allocators
#include <Allocator.hpp>

/**
 * A Lua allocator function utilizing standard Malloc, Realloc and Free
 *  No userdata is required to be passed through to be used
 **/
Pointer_t LE_Default_LAlloc(Pointer_t _pAllocator, Pointer_t _pPointer, Bytes_t _oSize, Bytes_t _nSize);

/**
 * A Lua allocator function utilizing the Allocator class
 *  Pass through to lua_newstate with an Allocator class as the userdata
 **/
Pointer_t LE_Allocator_LAlloc(Pointer_t _pAllocator, Pointer_t _pPointer, Bytes_t _oSize, Bytes_t _nSize);

#endif