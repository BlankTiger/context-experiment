#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>
#include <cassert>

struct Allocator {
    void* (*alloc)(void* data, std::size_t size, std::size_t alignment);
    void (*free)(void* data, void* pointer, std::size_t alignment);

    void* data;
};

static constexpr std::size_t DEFAULT_ALIGNMENT = alignof(std::max_align_t);
static constexpr std::size_t DEFAULT_PAGE_SIZE = 4096;

Allocator default_allocator = {
    .alloc = [](void*, std::size_t size, std::size_t alignment) -> void* {
        if (alignment <= DEFAULT_ALIGNMENT)
            return std::malloc(size);
        return std::aligned_alloc(alignment, size);
    },
    .free = [](void*, void* pointer, std::size_t) { std::free(pointer); },
    .data = nullptr,
};

// align_forward adds to orig_size so that it becomes an even multiple of 'alignment'.
size_t align_forward(size_t original_size, size_t alignment) {
    assert(alignment > 0);

    auto size = ((original_size + alignment - 1) / alignment) * alignment;
    // The above code might be hard to think about. Here's another implementation,
    // that has a branch:

    //    size := original_size;
    //    remainder := original_size % a;
    //    if remainder  size += a - remainder;

    return size;
}
