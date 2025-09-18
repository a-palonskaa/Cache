#include <iostream>
#include <vector>

#include "belady_cache.hpp"

int slow_get_page_int(int key) { return key; }

int main() {
    uint hits = 0;
    size_t cap, elem_amount;

    std::cin >> cap >> elem_amount;
    if (!std::cin.good()) {
        std::cerr << "failed to read capacity and elem amount from cin\n";
        return EXIT_FAILURE;
    }

    std::vector<int> v;
    v.reserve(elem_amount);

    for (size_t i = 0; i < elem_amount; i++) {
        size_t elem;
        std::cin >> elem;
        if (!std::cin.good()) {
            std::cerr << "failed to read elem from cin\n";
            return EXIT_FAILURE;
        }

        v.push_back(elem);
    }

    belady_cache_t<int, int> c{cap, v, slow_get_page_int};

    for (auto& elem : v) {
        hits += c.lookup_update(elem);
    }

    std::cout << hits << '\n';
}
