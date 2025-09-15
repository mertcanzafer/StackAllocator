#pragma once
#include "Defines.h"
#include <cassert>
#include <exception>
#include <cstddef>
#include <cstring>
#include <type_traits>

#ifndef NDEBUG
#define STACK_ALLOCATOR_DEBUG_FILL
#endif // in Debug mode

static constexpr std::byte DEBUG_FILL_BYTE = std::byte(0xCD); ///< Debug fill byte

/// A simple stack allocator for fast LIFO memory allocation.
/// You can only roll back to a previous marker or clear the entire stack.
class StackAllocator
{
public:
    /// Marker type for current stack top.
    using Marker = u32;

    /// Constructs a stack allocator with a given size in bytes.
    /// @param StackSizeBytes Total size of the stack in bytes.
    explicit StackAllocator(u32 StackSizeBytes);

    StackAllocator(const StackAllocator&) = delete; ///< Deleted copy constructor.
    StackAllocator& operator=(const StackAllocator&) = delete; ///< Deleted copy assignment.

    StackAllocator(StackAllocator&& other) noexcept; ///< Move constructor.
    StackAllocator& operator=(StackAllocator&& other) noexcept; ///< Move assignment.

    /// Allocates a block of memory from the top of the stack.
    /// @param SizeBytes Size of the allocation in bytes.
    /// @param Alignment Alignment requirement.
    /// @return Pointer to the allocated memory.
    void* Allocate(u32 SizeBytes, u32 Alignment = alignof(std::max_align_t));

    /// Returns a marker to the current stack top.
    /// @return Current marker.
    Marker GetMarker() const;

    /// Rolls the stack back to a previous marker.
    /// @param marker Marker to roll back to.
    void FreeToMarker(Marker marker);

    /// Clears the entire stack. All allocations are invalidated.
    void Clear();

    ~StackAllocator(); ///< Frees the stack memory.

    /// RAII scope for automatic rewind of the stack allocator.
    /// Rolls back to the saved marker on destruction.
    class RewindScope
    {
    public:
        /// Constructs a rewind scope. Saves current marker.
        explicit RewindScope(StackAllocator& allocator);

        /// Rewinds the stack to the saved marker.
        ~RewindScope();
    private:
        StackAllocator& m_Allocator; ///< Reference to the stack allocator.
        Marker m_Marker;             ///< Saved marker at construction time.
    };
private:
    /// Header for stack allocations, stores padding.
    struct StackAllocationHeader
    {
        u8 padding;
    };

    /// Calculates the padding required for alignment.
    /// @param SizeBytes Size of allocation.
    /// @param Alignment Alignment requirement.
    /// @param currentAddress Current address of stack top.
    /// @return Number of padding bytes required.
    u32 CalculatePadding(u32 SizeBytes, u32 Alignment, uintptr_t& currentAddress);
private:
    std::byte* m_StackBase; ///< Base pointer of the stack memory.
    u32 m_StackSizeBytes;   ///< Total size of the stack in bytes.
    Marker m_CurrentMarker; ///< Current top of the stack.
};


/// Allocates memory from a stack allocator and constructs objects if needed.
/// @tparam T Type of object to allocate.
/// @tparam Args Types of constructor arguments for non-trivial objects.
/// @param size Number of objects to allocate.
/// @param allocator Reference to the stack allocator.
/// @param args Arguments forwarded to the constructor of T.
/// @return Pointer to the first object of type T.
/// @note Trivial types are returned as-is. Non-trivial types are constructed in-place.
template <typename T, typename... Args>
T* AllocateFromStack(size_t size, StackAllocator& allocator, Args&&... args)
{
    void* mem = allocator.Allocate(sizeof(T) * size, alignof(T));

    if constexpr (std::is_trivial<T>::value)
    {
        return reinterpret_cast<T*>(mem);
    }
    else
    {
        T* array = reinterpret_cast<T*>(mem);
        for (size_t i = 0; i < size; ++i)
            new (&array[i]) T(std::forward<Args>(args)...);
        return array;
    }
}

/// Destroys non-trivial objects in-place.
/// @tparam T Type of object.
/// @param array Pointer to the array of objects to destroy.
/// @param size Number of objects to destroy.
/// @note Must be called for non-trivial types before rolling back or clearing the stack allocator to avoid leaks.
template <typename T>
void DestroyObjects(T* array, size_t size)
{
    if (!array) return;

    if constexpr (!std::is_trivial<T>::value)
    {
        for (size_t i = 0; i < size; ++i)
            array[i].~T();
    }
}
