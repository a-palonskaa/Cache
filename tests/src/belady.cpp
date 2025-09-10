#include <cassert>
#include <iostream>

#include "belady_cache.hpp"

int slow_get_page_int(int key) { return key; }

int main() {
    uint hits = 0;
    size_t cap = 0, elem_amount = 0;

    std::cin >> cap >> elem_amount;
    assert(std::cin.good());

    std::vector<int> v;
    for (size_t i = 0; i < elem_amount; i++) {
        size_t elem;
        std::cin >> elem;
        assert(std::cin.good());
        v.push_back(elem);
    }

    belady_cache_t<int, int> c{cap, v};

    for (auto& elem : v) {
        if (c.lookup_update(elem, slow_get_page_int)) {
            hits++;
        }
    }

    std::cout << "this shit works " << hits << std::endl;
    assert(std::cout.good());
}
