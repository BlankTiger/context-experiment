#pragma once

#include "allocator.hh"

const size_t DEFAULT_RESERVE_SIZE = 256 * 1024 * 1024;

struct Arena {
    void* memory_base;
    void* current_point;
    void* address_limit;

    explicit Arena(size_t reserve = DEFAULT_RESERVE_SIZE) {};

    Allocator allocator() {
        return {
            .alloc = &alloc,
            .free = &free,
            .data = this,
        };
    };

    void reset() {};

    static void* alloc(void* data, std::size_t size, std::size_t alignment) {
        auto* arena = static_cast<Arena*>(data);
        return nullptr;
    };

    static void free(void* data, void* pointer, std::size_t alignment) {
        auto* arena = static_cast<Arena*>(data);
    };
};
