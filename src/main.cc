#include "arena.hh"
#include "context.hh"
#include <iomanip>
#include <iostream>
#include <print>
#include <string>
#include <utility>
#include <vector>

Arena<true> arena;
const auto arena_allocator = arena.allocator();

Arena<true> arena2;
const auto arena_allocator2 = arena2.allocator();

std::pair<std::vector<int>, std::string> using_temp() {
    PushAllocator(arena_allocator);

    std::print("memory left: {}\n", arena.bytes_left());
    std::vector v{1, 2, 3};
    std::string a{"ssssssssssssssssssssssssssssssssssssssssssssssss"};
    std::print("memory left: {}\n", arena.bytes_left());
    return {std::move(v), std::move(a)};
}

void print_span(std::span<const std::byte> bytes) {
    for (std::byte b : bytes) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<unsigned>(std::to_integer<unsigned>(b)) << ' ';
    }
    std::cout << std::dec << '\n';
}

int main() {
    auto [v, s] = using_temp();
    std::print("{}, {}\n", v, s);

    {
        PushAllocator(arena_allocator2);
        auto v2 = v;
        auto s2 = s;
        std::print("{}, {} 2\n", v2, s2);
    }

    {
        auto v2 = v;
        auto s2 = s;
        std::print("{}, {} 3\n", v2, s2);
    }

    // std::span arena_memory{arena.memory_base, 150};
    // print_span(arena_memory);
    //
    // arena.reset();
    //
    // std::print("{}, {}\n", *v, *s);
    // print_span(arena_memory);

    return 0;
}
