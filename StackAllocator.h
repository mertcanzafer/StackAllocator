#pragma once
#include "Defines.h"
#include <cassert>
#include <cstddef>
#include <cstring>

#ifndef NDEBUG
#define STACK_ALLOCATOR_DEBUG_FILL
#endif // in Debug mode

static constexpr std::byte DEBUG_FILL_BYTE = std::byte(0xCD); // Debug fill byte

class StackAllocator
{
public:
	// Stack marker: Represents the current top of the   
	// stack. You can only roll back to a marker, not to  
	// arbitrary locations within the stack.
	using Marker = u32;

	// Constructs a stack allocator with the given total   
	// size.
	explicit StackAllocator(u32 StackSizeBytes);

	// Allocates a new block of the given size from stack  
	// top.
	void* Allocate(u32 SizeBytes, u32 Alignment = alignof(std::max_align_t));

	// Returns a marker to the current stack top
	Marker GetMarker() const;

	// Rolls the stack back to a previous marker
	void FreeToMarker(Marker marker);

	// Clears the entire stack (rolls the stack back to   
	// zero).
	void Clear();
	// Frees the stack allocator and releases all memory
	~StackAllocator();

	// Scope for rewinding the stack allocator
	class RewindScope
	{
	public:
		// Constructs a rewind scope that will rewind the stack
		explicit RewindScope(StackAllocator& allocator);

		// Rewinds the stack allocator to the marker at the time of construction
		~RewindScope();
	private:
		// Reference to the stack allocator
		StackAllocator& m_Allocator;

		// Marker at the time of construction
		Marker m_Marker;
	};

private:
	// Pointer to the base of the stack
	std::byte* m_StackBase;
	// Size of the stack in bytes
	u32 m_StackSizeBytes;
	// Current stack top
	Marker m_CurrentMarker;
};