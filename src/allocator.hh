#pragma once

#include <cstddef>
#include <cstdlib>
#include <new>

struct Allocator {
    void* (*alloc)(void* data, std::size_t size, std::size_t alignment);
    void (*free)(void* data, void* pointer, std::size_t alignment);

    void* data;
};

static constexpr std::size_t DEFAULT_ALIGNMENT = alignof(std::max_align_t);

Allocator default_allocator = {
    .alloc = [](void*, std::size_t size, std::size_t alignment) -> void* {
        if (alignment <= DEFAULT_ALIGNMENT)
            return std::malloc(size);
        return std::aligned_alloc(alignment, size);
    },
    .free = [](void*, void* pointer, std::size_t) { std::free(pointer); },
    .data = nullptr,
};
