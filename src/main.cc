#include "arena.hh"
#include "context.hh"
#include <print>
#include <thread>
#include <vector>

thread_local int thread_id;
const auto MAX_ITERATIONS = 100;

void print_thread_info() {
    std::print("Thread {}, allocator pointer: {}\n", thread_id,
               static_cast<void*>(&context.allocator));
}

Arena arena;
const auto arena_allocator = arena.allocator();

void using_temp() {
    thread_id = 3;
    PushAllocator(arena_allocator);

    for (size_t i{0}; i < MAX_ITERATIONS; ++i) {
        print_thread_info();
        std::vector v{1, 2, 3};
    }
}

void using_default() {
    thread_id = 2;
    PushAllocator(logging_default_allocator);

    for (size_t i{0}; i < MAX_ITERATIONS; ++i) {
        print_thread_info();
        std::vector v{1, 2, 3};
    }
}

int main() {

    thread_id = 1;

    print_thread_info();
    std::jthread t1(using_temp);
    std::jthread t2(using_default);

    t1.join();
    t2.join();

    print_thread_info();

    return 0;
}
