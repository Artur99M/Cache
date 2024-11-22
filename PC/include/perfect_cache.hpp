#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>

#ifdef DEBUG
static auto & debug = std::cerr;
#else
namespace
{
class debuging
{
    public:
    template <class T>
    debuging & operator<< (const T & elem)
    {
        return *this;
    }
};

debuging debug;
}
#endif

namespace cache
{
    template <class T, class keyT>
    struct cache_part
    {
        T elem_;
        keyT key_;
        size_t next_using;

        static cache_part<T, keyT> func(T elem, keyT key, size_t next)
        {
            cache_part<T, keyT> p;
            p.key_ = key;
            p.next_using = next;
            p.elem_ = elem;
            return p;
        }
    };

    template <class T> static size_t find(const T * const arr, size_t arr_size, const T elem)
    {
        for (size_t i = 0; i < arr_size; i++)
            if (arr[i] == elem) return i;

        return 0xDEAD0BED;
    }

    template <class T, class keyT = int>
    size_t lookup_update_perfect (keyT* lookup, size_t n_hits, T (*slow_get_page)(keyT), const size_t cache_size)
    {
    debug << "start\n";
    std::list<cache_part<T, keyT>> cache;
    using it = typename std::list<cache_part<T, keyT>>::iterator;
    std::unordered_map<keyT, it> hash;
    size_t value = 0;

    auto out = cache.end();
    for (size_t p = 0; p < n_hits; ++p)
    {
        keyT hit = lookup[p];
        debug << "cache: ";
        for (auto i = cache.begin(); i != cache.end(); ++i)
        {
            debug << '{' << i->elem_ << ", " << i->next_using << "} ";
            --((*i).next_using);
        }
        debug << "\nhit = " << hit << '\n';
        auto hhit = hash.find(hit);
        if (hhit != hash.end())
        {
            debug << "in cache\n";
            auto chit = (*hhit).second;
            chit->next_using = find (lookup + p + 1, n_hits - p - 1, hit);
            if (chit->next_using > out->next_using)
                out = chit;
            ++value;
        }
        else
        {
            size_t next_using = find(lookup + p + 1, n_hits - p - 1, hit);
            debug << "no in cache, next_using = " << next_using << '\n';
            if (next_using == 0xDEAD0BED)
                continue;

            if (out == cache.end() || (*out).next_using > next_using)
            {
                debug << "add elem in cache\n";
                if (cache.size() == cache_size)
                {
                    hash.erase(out->key_);
                    cache.erase(out);
                }
                cache.emplace_front(cache_part<T, keyT>::func((*slow_get_page)(hit), hit, next_using));
                hash.emplace(hit, cache.begin());
            }
        }
        debug << '\n';
    }

    return value;
  }

} //namespace cache

