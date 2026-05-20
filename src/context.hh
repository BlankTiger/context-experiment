#pragma(once)

#include <cstdlib>
#include <new>
#include <print>

struct Allocator {
    void* (*alloc)(std::size_t size);
    void (*free)(void* pointer);
};

Allocator default_allocator = {
    .alloc = [](std::size_t size) -> void* { return std::malloc(size); },
    .free = [](void* pointer) { std::free(pointer); },
};

Allocator logging_default_allocator = {
    .alloc = [](std::size_t size) -> void* {
        std::print("default alloc: {}\n", size);
        return std::malloc(size);
    },
    .free =
        [](void* pointer) {
            std::print("default free: {}\n", pointer);
            std::free(pointer);
        },
};

Allocator temp_allocator = {
    .alloc = [](std::size_t size) -> void* {
        std::print("temp alloc: {}\n", size);
        return std::malloc(size);
    },
    .free =
        [](void* pointer) {
            std::print("temp free: {}\n", pointer);
            std::free(pointer);
        },
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

void* operator new(std::size_t sz) {
    void* pointer = context.allocator.alloc(sz);
    if (!pointer)
        throw std::bad_alloc{};
    return pointer;
}

void* operator new[](std::size_t sz) { return ::operator new(sz); }

void* operator new(std::size_t sz, const std::nothrow_t&) noexcept {
    try {
        return ::operator new(sz);
    } catch (...) {
        return nullptr;
    }
}

void* operator new[](std::size_t sz, const std::nothrow_t&) noexcept {
    try {
        return ::operator new[](sz);
    } catch (...) {
        return nullptr;
    }
}

void operator delete(void* pointer) noexcept {
    context.allocator.free(pointer);
}

void operator delete[](void* pointer) noexcept { ::operator delete(pointer); }

void operator delete(void* pointer, const std::nothrow_t&) noexcept {
    ::operator delete(pointer);
}

void operator delete[](void* pointer, const std::nothrow_t&) noexcept {
    ::operator delete[](pointer);
}

// C++14: sized deallocation
void operator delete(void* pointer, std::size_t) noexcept {
    ::operator delete(pointer);
}

void operator delete[](void* pointer, std::size_t) noexcept {
    ::operator delete[](pointer);
}
