#include "StackAllocator.h"
#include <print>  // C++23

int main()
{
    try {
        StackAllocator allocator{ 1024 }; // Allocate 1KB stack

        // Allocate 100 bytes aligned to 16 bytes
        void* ptr1 = allocator.Allocate(100, 16);
        std::println("Allocated 100 bytes at address: {}", ptr1);

        // Get a marker to rollback later
        StackAllocator::Marker marker = allocator.GetMarker();
        std::println("Current marker after first allocation: {}", marker);

        // Allocate another 200 bytes with default alignment
        void* ptr2 = allocator.Allocate(200);
        std::println("Allocated 200 bytes at address: {}", ptr2);
        std::println("Current marker after second allocation: {}", allocator.GetMarker());

        // Rollback to the previous marker
        allocator.FreeToMarker(marker);
        std::println("Rolled back to marker {}, current marker: {}", marker, allocator.GetMarker());

        // Allocate 50 bytes
        void* ptr3 = allocator.Allocate(50);
        std::println("Allocated 50 bytes at address: {}", ptr3);
        std::println("Current marker after third allocation: {}", allocator.GetMarker());

        // Use RAII to automatically roll back
        {
            StackAllocator::RewindScope rewindScope(allocator);
            void* ptr4 = allocator.Allocate(300); // Allocate 300 bytes
            std::println("Allocated 300 bytes at address: {}", ptr4);
            std::println("Current marker inside scope: {}", allocator.GetMarker());
        }
        // After exiting the scope, the stack will be rolled back automatically
        std::println("Current marker after RewindScope: {}", allocator.GetMarker());

        // Clear the stack allocator
        allocator.Clear();
        std::println("Stack cleared, current marker: {}", allocator.GetMarker());
    }
    catch (const std::exception& e) {
        std::println("Exception: {}", e.what());
        return 1;
    }

    return 0;
}
