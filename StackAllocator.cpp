#include "StackAllocator.h"


StackAllocator::StackAllocator(u32 StackSizeBytes)
	: m_StackBase(nullptr), m_StackSizeBytes(StackSizeBytes), m_CurrentMarker(0u)
{
	assert(StackSizeBytes > 0 && StackSizeBytes <= U32_MAX);
	m_StackBase = new std::byte[m_StackSizeBytes];
#ifdef STACK_ALLOCATOR_DEBUG_FILL
	std::memset(m_StackBase, static_cast<int>(DEBUG_FILL_BYTE), m_StackSizeBytes);
#endif
}

StackAllocator::StackAllocator(StackAllocator&& other) noexcept
{
	m_StackBase = other.m_StackBase;
	m_StackSizeBytes = other.m_StackSizeBytes;
	m_CurrentMarker = other.m_CurrentMarker;

	other.m_StackBase = nullptr;
	other.m_StackSizeBytes = 0u;
	other.m_CurrentMarker = 0u;
}

StackAllocator& StackAllocator::operator=(StackAllocator&& other) noexcept
{
	if (this == &other) return *this;

	delete[] m_StackBase;

	m_StackBase = other.m_StackBase;
	m_StackSizeBytes = other.m_StackSizeBytes;
	m_CurrentMarker = other.m_CurrentMarker;

	other.m_StackBase = nullptr;
	other.m_StackSizeBytes = 0u;
	other.m_CurrentMarker = 0u;

	return *this;
}

u32 StackAllocator::CalculatePadding(u32 SizeBytes, u32 Alignment, uintptr_t& currentAddress)
{
	if (SizeBytes == 0)
		throw std::exception("Cannot allocate zero bytes.");
	if ((Alignment & (Alignment - 1)) != 0)
		throw std::exception("Alignment must be a power of two.");

	if (Alignment > 256) Alignment = 256; // max alignment supported by 8-bit padding

	currentAddress = reinterpret_cast<uintptr_t>(m_StackBase + m_CurrentMarker);

	// Calculate the aligned address including the header
	uintptr_t alignedAddress = (currentAddress + sizeof(StackAllocationHeader) + (Alignment - 1)) & ~(uintptr_t(Alignment) - 1);
	u32 padding = static_cast<u32>(alignedAddress - currentAddress);

	return padding;
}

void* StackAllocator::Allocate(u32 SizeBytes, u32 Alignment)
{
	uintptr_t currentAddress = 0;
	u32 padding = CalculatePadding(SizeBytes, Alignment, currentAddress);

	if (m_CurrentMarker + padding + SizeBytes > m_StackSizeBytes)
		throw std::exception("Stack allocator is out of memory");

	uintptr_t nextAddress = currentAddress + padding;

	auto* header = reinterpret_cast<StackAllocationHeader*>(nextAddress - sizeof(StackAllocationHeader));
	header->padding = static_cast<u8>(padding);

	m_CurrentMarker += padding + SizeBytes;

#ifdef STACK_ALLOCATOR_DEBUG_FILL
	std::memset(reinterpret_cast<void*>(nextAddress), 0, SizeBytes);
#endif
	return reinterpret_cast<void*>(nextAddress);
}

using Marker = u32;
Marker StackAllocator::GetMarker() const
{
	return m_CurrentMarker;
}

void StackAllocator::FreeToMarker(Marker marker)
{
	assert(marker <= m_CurrentMarker && "Cannot free to a marker beyond the current stack top.");
#ifdef STACK_ALLOCATOR_DEBUG_FILL
	std::memset(m_StackBase + marker, static_cast<int>(DEBUG_FILL_BYTE), m_CurrentMarker - marker);
#endif
	m_CurrentMarker = marker;
}

void StackAllocator::Clear()
{
#ifdef STACK_ALLOCATOR_DEBUG_FILL
	std::memset(m_StackBase, static_cast<int>(DEBUG_FILL_BYTE), m_StackSizeBytes);
#endif
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
