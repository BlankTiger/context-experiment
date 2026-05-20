#pragma once

void* operator new(std::size_t size) {
    if (size == 0)
        size = 1;
    void* pointer = context.allocator.alloc(context.allocator.data, size,
                                            DEFAULT_ALIGNMENT);
    if (!pointer)
        throw std::bad_alloc{};
    return pointer;
}

void* operator new[](std::size_t size) { return ::operator new(size); }

void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    try {
        return ::operator new(size);
    } catch (...) {
        return nullptr;
    }
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept {
    try {
        return ::operator new[](size);
    } catch (...) {
        return nullptr;
    }
}

void* operator new(std::size_t size, std::align_val_t al) {
    if (size == 0)
        size = 1;
    void* pointer = context.allocator.alloc(context.allocator.data, size,
                                            static_cast<std::size_t>(al));
    if (!pointer)
        throw std::bad_alloc{};
    return pointer;
}

void* operator new[](std::size_t size, std::align_val_t al) {
    return ::operator new(size, al);
}

void* operator new(std::size_t size, std::align_val_t al,
                   const std::nothrow_t&) noexcept {
    try {
        return ::operator new(size, al);
    } catch (...) {
        return nullptr;
    }
}

void* operator new[](std::size_t size, std::align_val_t al,
                     const std::nothrow_t&) noexcept {
    try {
        return ::operator new[](size, al);
    } catch (...) {
        return nullptr;
    }
}

void operator delete(void* pointer) noexcept {
    context.allocator.free(context.allocator.data, pointer, DEFAULT_ALIGNMENT);
}

void operator delete[](void* pointer) noexcept { ::operator delete(pointer); }

void operator delete(void* pointer, const std::nothrow_t&) noexcept {
    ::operator delete(pointer);
}

void operator delete[](void* pointer, const std::nothrow_t&) noexcept {
    ::operator delete[](pointer);
}

void operator delete(void* pointer, std::size_t) noexcept {
    ::operator delete(pointer);
}

void operator delete[](void* pointer, std::size_t) noexcept {
    ::operator delete[](pointer);
}

void operator delete(void* pointer, std::align_val_t al) noexcept {
    context.allocator.free(context.allocator.data, pointer,
                           static_cast<std::size_t>(al));
}

void operator delete[](void* pointer, std::align_val_t al) noexcept {
    ::operator delete(pointer, al);
}

void operator delete(void* pointer, std::size_t, std::align_val_t al) noexcept {
    ::operator delete(pointer, al);
}

void operator delete[](void* pointer, std::size_t,
                       std::align_val_t al) noexcept {
    ::operator delete(pointer, al);
}

void operator delete(void* pointer, const std::nothrow_t&,
                     std::align_val_t al) noexcept {
    ::operator delete(pointer, al);
}

void operator delete[](void* pointer, const std::nothrow_t&,
                       std::align_val_t al) noexcept {
    ::operator delete(pointer, al);
}
