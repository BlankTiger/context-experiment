#pragma once

#include "allocator.hh"
#include <cassert>

const size_t DEFAULT_RESERVE_SIZE = 256 * 1024 * 1024;

struct Arena {
    std::byte* memory_base;
    std::byte* current_point;
    std::byte* address_limit;

    explicit Arena(size_t reserve = DEFAULT_RESERVE_SIZE) {
        reserve = align_forward(reserve, DEFAULT_PAGE_SIZE);

        // @TODO: Use mmap directly or something, for now use the default as
        // a backing allocator.
        memory_base = static_cast<std::byte*>(std::malloc(reserve));
        assert(memory_base != nullptr);
        current_point = memory_base;
        address_limit = memory_base + reserve;
    };

    Allocator allocator() {
        return {
            .alloc = &alloc,
            .free = &free,
            .data = this,
        };
    };

    void reset() { current_point = memory_base; };
    size_t space_left() { return address_limit - current_point; }

    static void* alloc(void* data, std::size_t size, std::size_t alignment) {
        auto* arena = static_cast<Arena*>(data);
        auto* old_point = arena->current_point;
        // @TODO: Unpacked allocations because of alignment.
        auto* new_point = old_point + align_forward(size, alignment);

        assert(new_point < arena->address_limit);
        arena->current_point = new_point;

        return static_cast<void*>(old_point);
    };

    static void free(void* data, void* pointer, std::size_t alignment) {};
};
