#include "StackAllocator.h"
#include <cstring> // for memset

StackAllocator::StackAllocator(u32 stackSize)
{
    // Adding () at the end initialize all bytes to 0.
    m_Stack = new u8[stackSize]();
    m_StackSize = stackSize;
    m_CurrentPos = stackSize;
}

void* StackAllocator::Alloc(u32 size)
{
    if (m_CurrentPos - size <= 0)
        return nullptr;
    
    m_CurrentPos -= size;
    
    return m_Stack + m_CurrentPos;
}

void StackAllocator::Free(u32 size)
{
    // TODO(Mikyan): abort? crash? error?
    if (m_StackSize < m_CurrentPos + size)
        return;
    
    // NOTE(Mikyan): Clear all bytes to 0.
    std::memset(m_Stack + m_CurrentPos, 0, size);
    
    m_CurrentPos += size;
}

void StackAllocator::Clear()
{
    std::memset(m_Stack, 0, m_StackSize);
    m_CurrentPos = m_StackSize;
}

void StackAllocator::Delete()
{
    delete m_Stack;
}
