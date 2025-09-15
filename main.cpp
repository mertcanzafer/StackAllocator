#include "StackAllocator.h"
#include <print> 
#include <string>

int main()
{
    try {
        // Create a stack allocator of 1KB
        StackAllocator allocator{ 1024 };

        // Allocate trivial type (int)
        int* intArray = AllocateFromStack<int>(5, allocator); // 5 ints
        for (int i = 0; i < 5; ++i)
            intArray[i] = i * 10;

        for (int i = 0; i < 5; ++i)
            std::println("intArray[{}] = {}", i, intArray[i]);

        // Allocate non-trivial type (std::string)
        std::string* strArray = AllocateFromStack<std::string>(3, allocator, "Hello");

        strArray[1] = "World";
        strArray[2] = "StackAllocator";

        for (int i = 0; i < 3; ++i)
            std::println("strArray[{}] = {}", i, strArray[i]);

        // Manually destroy non-trivial objects
        DestroyObjects(strArray, 3);

        // Demonstrate stack markers
        auto marker = allocator.GetMarker();
        std::println("Marker after allocations: {}", marker);

        // Allocate more ints
        int* extraInts = AllocateFromStack<int>(2, allocator);
        extraInts[0] = 100;
        extraInts[1] = 200;

        std::println("Extra ints: {}, {}", extraInts[0], extraInts[1]);

        // Rollback to previous marker
        allocator.FreeToMarker(marker);
        std::println("Rolled back to marker {}, current marker: {}", marker, allocator.GetMarker());

        // RAII rollback using RewindScope
        {
            StackAllocator::RewindScope scope(allocator);
            auto tempArray = AllocateFromStack<int>(4, allocator);
            for (int i = 0; i < 4; ++i)
                tempArray[i] = i + 1;
            std::println("Marker inside RewindScope: {}", allocator.GetMarker());
        }
        std::println("Marker after RewindScope: {}", allocator.GetMarker());

        // Clear entire stack
        allocator.Clear();
        std::println("Marker after Clear: {}", allocator.GetMarker());
    }
    catch (const std::exception& e) {
        std::println("Exception: {}", e.what());
        return 1;
    }

    return 0;
}
