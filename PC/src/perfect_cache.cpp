#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#include "perfect_cache.hpp"

// slow get page imitation
int slow_get_page_int(int key) { return key; }

int main() {
    int n;
    size_t m;

    std::cin >> m >> n;
    assert(std::cin.good());
    int* lookup = new int[n]{};
    for (int i = 0; i < n; ++i)
    {
        std::cin >> lookup[i];
        assert (std::cin.good());
    }

    std::cout << cache::lookup_update_perfect(lookup, n ,slow_get_page_int, m) << std::endl;
}
