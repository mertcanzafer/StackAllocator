# Memory Allocation

Efficient memory management is essential in game development — especially when working with third-party assets such as textures, models, animations, or audio. Memory usage directly impacts **performance**, **load times**, and **overall stability**.

In C++, we typically rely on two core memory management techniques:

1. **Stack Allocation**  
2. **Dynamic (Heap) Allocation**

---

## Dynamic Memory Allocation

Dynamic memory allocation — via `malloc()` / `free()` in C, or `new` / `delete` in C++ — is also known as **heap allocation**.  
While flexible, heap allocation is generally considered **slow and expensive**, particularly in performance-critical systems such as real-time games.

### ❌ Why Is Heap Allocation Slow?

There are two main reasons:

- **General-Purpose Overhead**  
  Heap allocators must handle arbitrary allocation sizes — from a few bytes to several gigabytes.  
  This flexibility introduces significant bookkeeping and management overhead.

- **Kernel Context Switching**  
  On many operating systems, calls to `malloc()` or `free()` may trigger a transition between **user mode** and **kernel mode**.  
  These context switches are computationally expensive and can severely impact performance.

---

## ⚠️ Best Practices in Game Development

> ✅ **Rule of Thumb:**  
> Keep heap allocations to a minimum — and **never allocate from the heap inside a tight loop.**

To achieve predictable and efficient performance, many games implement **custom memory management systems** such as:

- 🌀 **Stack Allocators**  
- 📦 **Pool Allocators**  
- 🧱 **Arena Allocators**

These specialized allocators are designed to be **fast**, **cache-friendly**, and **fragmentation-resistant**, making them ideal for real-time applications.

---

## Stack-Based Allocators

![Stack Allocator](https://www.gingerbill.org/images/memory-allocation-strategies/stack_allocator_alloc.svg)

In many game engines, memory is allocated in a **stack-like** fashion.  
For example, when a new level is loaded, the required memory is allocated once. During gameplay, little or no further dynamic allocation occurs.  
When the level ends, its data is unloaded, and all associated memory can be released in one operation.

This pattern perfectly matches a stack allocator’s behavior — **allocate sequentially, free all at once**.

You can use this custom stack allocator pattern in your own projects:

---

### Example Code (from *Perlin3D*)

This allocator is used in my engine for storing **temporary objects and data structures**:

```cpp
// Allocating 1KB of memory once
auto m_StackAllocator = eastl::make_unique<Core::Memory::StackAllocator>(1024);

// ... Example usage ...

// Storing shader data to create a Shader module
Core::Memory::StackAllocator::RewindScope rwn(*m_StackAllocator);
char* shaderCode = Core::Memory::AllocateFromStack<char>(*this, shaderFileSize);

// Must be called to destroy constructed objects
Core::Memory::DestroyObjects<char>(shaderCode, shaderFileSize);
```

---

### 📝 Notes

- **Page Size Considerations**  
  Most operating systems use a 4 KB memory page size.  
  While you can allocate more than 4 KB, larger allocations may span multiple pages, reducing cache locality and performance efficiency.  
  For larger allocations, consider using **huge pages** or a separate allocator optimized for big blocks of memory.

- **Thread Safety**  
  This stack-like allocator is **not thread-safe**.  
  Ensure that only one thread modifies its memory at a time.  
  In multithreaded systems, consider using **per-thread allocators** or synchronization mechanisms (e.g., mutexes) to avoid race conditions.

- **Alignment & Cache Locality**  
  When designing custom allocators, always ensure proper **memory alignment** and **cache-friendly access patterns** to maximize CPU performance.

---

## 📚 Further Reading

For more in-depth coverage of memory allocation strategies and system-level design, see  
**[Game Engine Architecture](https://www.gameenginebook.com/)** by *Jason Gregory*.

---

### 💡 Summary

- Avoid heap allocations in performance-critical loops.  
- Use specialized allocators (stack, pool, arena) for predictable performance.  
- Understand page sizes, alignment, and thread-safety implications.  
- Keep memory access patterns cache-efficient.

Efficient memory allocation isn’t just an optimization — it’s a **cornerstone of high-performance game engine design**.
