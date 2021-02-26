#ifndef _STACK_ALLOCATOR_H
#define _STACK_ALLOCATOR_H

#include "Types.h"

#define ArrayCount(Array) (sizeof(Array) / sizeof(Array[0]))

class StackAllocator
{
    public:
    StackAllocator(u32 stackSize);
    ~StackAllocator() = default;
    
    public:
    void* Alloc(u32 size);
    void Free(u32 size);
    void Clear();
    void Delete();
    
    private:
    u8* m_Stack;
    u32 m_CurrentPos;
    u32 m_StackSize;
};

#endif //_STACK_ALLOCATOR_H
