#pragma once

#include <print>

#include "allocator.hh"

Allocator logging_default_allocator = {
    .alloc = [](void*, std::size_t size, std::size_t alignment) -> void* {
        std::print("default alloc: {} align: {}\n", size, alignment);
        if (alignment <= DEFAULT_ALIGNMENT)
            return std::malloc(size);
        return std::aligned_alloc(alignment, size);
    },
    .free =
        [](void*, void* pointer, std::size_t) {
            std::print("default free: {}\n", pointer);
            std::free(pointer);
        },
    .data = nullptr,
};

Allocator temp_allocator = {
    .alloc = [](void*, std::size_t size, std::size_t alignment) -> void* {
        std::print("temp alloc: {} align: {}\n", size, alignment);
        if (alignment <= DEFAULT_ALIGNMENT)
            return std::malloc(size);
        return std::aligned_alloc(alignment, size);
    },
    .free =
        [](void*, void* pointer, std::size_t) {
            std::print("temp free: {}\n", pointer);
            std::free(pointer);
        },
    .data = nullptr,
};

struct Context {
    Allocator allocator = default_allocator;
};

thread_local Context context;

struct Allocator_Scope {
    Allocator previous;

    explicit Allocator_Scope(Allocator a) : previous(context.allocator) {
        context.allocator = a;
    }

    ~Allocator_Scope() { context.allocator = previous; }

    Allocator_Scope(const Allocator_Scope&) = delete;
    Allocator_Scope& operator=(const Allocator_Scope&) = delete;
};

#define PushAllocator(a) Allocator_Scope _alloc_scope_##__LINE__(a)

#include "operator_new_delete.hh"
