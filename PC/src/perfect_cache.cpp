#include <cassert>
#include <iostream>

#include "perfect_cache.hpp"

// slow get page imitation
int slow_get_page_int(int key) { return key; }

int main() {
    int hits = 0;
    int n;
    size_t m;

    std::cin >> m >> n;
    assert(std::cin.good());
    cache::PC_cache<int> c(m);

    int* hits_data = new int[n];
    for (int i = 0; i < n; ++i)
    {
        std::cin >> hits_data[i];
        assert(std::cin.good());
    }

    c.make_lookup(hits_data, n);
    for (int i = 0; i < n; ++i)
    {
        if (c.lookup_update(hits_data[i], slow_get_page_int))
            hits += 1;
    }

  std::cout << hits << std::endl;
}
