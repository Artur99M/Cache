#include <cassert>
#include <iostream>
#include <vector>

#include "../include/perfect_cache.hpp"

// slow get page imitation
int slow_get_page_int(int key) { return key; }

int main() {
    int n, number;
    size_t m;

    std::cin >> number >> m >> n;
    assert(std::cin.good());
    std::vector<int> lookup;
    for (int i = 0; i < n; ++i)
    {
        int x;
        std::cin >> x;
        assert (std::cin.good());
        lookup.push_back(x);
    }

    std::cout << cache::lookup_update_perfect(lookup, slow_get_page_int, m) << std::endl;
}
