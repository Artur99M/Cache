// #include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>

namespace cache
{
    template <class T> static size_t find(const std::vector<T> & vec, const T elem)
    {
        for (size_t i = 0; i < vec.size(); i++)
            if (vec[i] == elem) return i;

        return -1;
    }

    template <class T, class keyT = int>
    size_t lookup_update_perfect (std::vector<keyT> & lookup, T (*slow_get_page)(keyT), const size_t cache_size)
    {

        std::list<std::pair<keyT, T>> cache;
        using it = typename std::list<std::pair<keyT, T>>::iterator;
        std::unordered_map<keyT, it> hash;
        size_t value = 0;
        while (lookup.size() > 0)
        {
            keyT key = lookup[0];
            lookup.erase (lookup.begin());
            if (hash.find(key) != hash.end())
            {
                value++;
                continue;
            }
            else
            {
                size_t will_find;
                if ((will_find = find (lookup, key)) == -1)
                    continue;
                if (cache.size() < cache_size)
                {
                    cache.emplace_front (std::make_pair (key, slow_get_page (key)));
                    hash.emplace (key, cache.begin());
                    continue;
                }
                auto out = cache.end();
                size_t out_val = will_find;
                auto i = cache.begin();
                for (; i != cache.end(); i++)
                {
                    size_t x = find (lookup, i->first);
                    if (x > out_val) out = i;
                    else if (x == -1) {out = i; break;}
                }
                hash.erase ((*i).first);
                cache.erase (i);
                cache.emplace_front (std::make_pair (key, slow_get_page (key)));
                hash.emplace (key, cache.begin());
            }
        }

        return value;
    }


} //namespace cache
