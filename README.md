# Memory Allocation

Memory management plays a critical role in game development, especially when dealing with third-party assets such as textures, models, animations, or audio. Efficient memory use directly impacts performance, load times, and overall stability.

As C++ programmers, we commonly rely on two core memory management techniques:

1. **Stack Allocation**
2. **Dynamic (Heap) Allocation**

---

## Dynamic Memory Allocation

Dynamic memory allocation—via `malloc()` / `free()` in C, or `new` / `delete` in C++—is also known as **heap allocation**. While flexible, it is generally considered **slow and expensive**, especially in performance-critical systems like real-time games.

### ❌ Why is Heap Allocation Slow?

There are two major reasons:

- **General-Purpose Overhead**  
  Heap allocators are designed to support arbitrary allocation sizes—from a single byte to multiple gigabytes. This flexibility introduces significant management overhead.

- **Kernel Context Switching**  
  On many operating systems, calls to `malloc()` or `free()` may require transitioning from **user mode** to **kernel mode** and back. These context switches are computationally expensive and can dramatically affect performance.

---

## ⚠️ Best Practices in Game Development

> ✅ **Rule of Thumb:**  
> Keep heap allocations to a minimum, and **never allocate from the heap inside a tight loop.**

This is why many games implement custom memory management systems such as:

- 🌀 **Stack Allocators**
- 📦 **Pool Allocators**
- 🧱 **Arena Allocators**

These specialized allocators offer **faster**, more **predictable performance** and **reduce memory fragmentation**, making them ideal for real-time applications like games.

## Stack-Based Allocators:
![Stack Allocator](https://www.gingerbill.org/images/memory-allocation-strategies/stack_allocator_alloc.svg)


- Many games allocate memory in a stack-like fashion. Whenever a new game 
level is loaded, memory is allocated for it. Once the level has been loaded, 
little or no dynamic memory allocation takes place. At the conclusion of 
the level, its data is unloaded and all of its memory can be freed. It makes 
a lot of sense to use a stack-like data structure for these kinds of memory 
allocations.
- You can use this custom stack allocator implementation example in your projects,
- It’s important to consider alignment when dealing with memory in C and C++.


For more information about memory allocation, see [Game Engine Architecture](https://www.gameenginebook.com/) by Jason Gregory.


