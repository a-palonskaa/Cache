#include <cassert>
#include <iostream>

#include "two_q_cache.hpp"

int slow_get_page_int(int key) { return key; }

int main() {
    uint hits = 0;
    size_t cap = 0, elem_amount = 0;

    std::cin >> cap >> elem_amount;
    assert(std::cin.good());

    two_q_cache_t<int, int> c{cap};

    for (size_t i = 0; i < elem_amount; i++) {
        size_t elem;
        std::cin >> elem;
        assert(std::cin.good());

        if (c.lookup_update(elem, slow_get_page_int)) {
            hits++;
        }
    }

    std::cout << "god bless " << hits << std::endl;
    assert(std::cout.good());
}
