#include "StackAllocator.h"
#include <print>

int main()
{
	StackAllocator allocator{ 1024 }; // Allocate 1KB stack

	void* ptr1 = allocator.Allocate(10, 16); // Allocate 100 bytes aligned to 16 bytes
	std::println("Allocated 10 bytes at address: {}", ptr1);

	// Get a marker to rollback later
	StackAllocator::Marker marker = allocator.GetMarker();

	// Allocater another 200 bytes with default alignment
	void* ptr2 = allocator.Allocate(200);
	std::println("Allocated 200 bytes, current marker: {}", marker);

	// Rollback to the marker
	allocator.FreeToMarker(marker);
	std::println("Rolled back to marker, current marker: {}", allocator.GetMarker());

	// Allocate 50 bytes
	void* ptr3 = allocator.Allocate(50);
	std::println("Allocated 50 bytes at address: {}", ptr3);

	// Use RAII to automatically roll back
	{
		StackAllocator::RewindScope rewindScope(allocator);
		void* ptr4 = allocator.Allocate(300); // Allocate 300 bytes
		std::println("Allocated 300 bytes at address: {}", ptr4);
	}
	// After exiting the scope, the stack will be rolled back

	// Clear the stack allocator
	allocator.Clear();
	std::println("Stack cleared, current marker: {}", allocator.GetMarker());

	return 0;
}