#include "StackAllocator.h"

StackAllocator::StackAllocator(u32 StackSizeBytes)
	:m_StackBase(nullptr), m_StackSizeBytes(0u), m_CurrentMarker(0u)
{
	assert(StackSizeBytes <= U32_MAX && "Stack size must not exceed U32_MAX.");
	m_StackSizeBytes = StackSizeBytes;
	m_StackBase = new std::byte[m_StackSizeBytes];
}

void* StackAllocator::Allocate(u32 SizeBytes, u32 Alignment)
{
	assert(SizeBytes > 0 && "Cannot allocate zero bytes.");
	assert((Alignment & (Alignment - 1)) == 0 && "Alignment must be a power of two.");

	// Calculate the aligned marker position
	// Calculate aligned marker

	uintptr_t currentAddress = reinterpret_cast<uintptr_t>(m_StackBase + m_CurrentMarker);
	uintptr_t alignedAddress = (currentAddress + (Alignment - 1)) & ~(Alignment - 1);
	u32 padding = static_cast<u32>(alignedAddress - currentAddress);

	u32 totalSize = SizeBytes + padding;
	assert(m_CurrentMarker + totalSize <= m_StackSizeBytes && "Stack overflow!");

	void* allocatedMemory = reinterpret_cast<void*>(alignedAddress);
	m_CurrentMarker += totalSize;
	return allocatedMemory;
}

using Marker = u32;
Marker StackAllocator::GetMarker() const
{
	return m_CurrentMarker;
}

void StackAllocator::FreeToMarker(Marker marker)
{
	assert(marker <= m_CurrentMarker && "Cannot free to a marker that is beyond the current stack top.");
#ifdef STACK_ALLOCATOR_DEBUG_FILL
	std::memset(m_StackBase + marker, 0, m_CurrentMarker - static_cast<size_t>(marker));
#endif
	m_CurrentMarker = marker;
}

void StackAllocator::Clear()
{
#ifdef STACK_ALLOCATOR_DEBUG_FILL
	std::memset(m_StackBase, 0, m_StackSizeBytes);
#endif
	// Reset the current marker to the base of the stack
	m_CurrentMarker = 0u;
}

StackAllocator::~StackAllocator()
{
	delete[] m_StackBase;
}

StackAllocator::RewindScope::RewindScope(StackAllocator& allocator)
	:m_Allocator(allocator), m_Marker(allocator.GetMarker()) {
}

StackAllocator::RewindScope::~RewindScope()
{
	m_Allocator.FreeToMarker(m_Marker);
}
