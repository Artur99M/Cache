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

    template <class T> static size_t find(const T * const arr, size_t arr_size, const T elem)
    {
        for (size_t i = 0; i < arr_size; i++)
            if (arr[i] == elem) return i;

        return 0xDEAD0BED;
    }

    template <class T, class keyT = int>
    size_t lookup_update_perfect (keyT* lookup, size_t n_hits, T (*slow_get_page)(keyT), const size_t cache_size)
    {
    struct cache_part
    {
        T elem_;
        keyT key_;
        size_t next_using;

        static cache_part func(T elem, keyT key, size_t next)
        {
            cache_part p;
            p.key_ = key;
            p.next_using = next;
            p.elem_ = elem;
            return p;
        }
    };
    debug << "start\n";
    struct PCache
    {
        std::list<cache_part> cache_;
        using it = typename std::list<cache_part>::iterator;
        std::unordered_map<keyT, it> hash_;
    };
    size_t value = 0;

    PCache pcache;
    auto out = pcache.cache_.end();

    for (size_t i_hit = 0; i_hit < n_hits; ++i_hit)
    {
        keyT hit = lookup[i_hit];
        debug << "cache: ";
        for (auto i =  pcache.cache_.begin(); i != pcache.cache_.end(); ++i)
        {
            debug << '{' << i->elem_ << ", " << i->next_using << "} ";
            if (i->next_using != 0xDEAD0BED)
                --(i->next_using);
        }
        debug << "\nhit = " << hit << '\n';
        auto hhit = pcache.hash_.find(hit);
        if (hhit != pcache.hash_.end())
        {
            debug << "in cache\n";
            ++value;
            auto chit = hhit->second;
            chit->next_using = find (lookup + i_hit + 1, n_hits - i_hit - 1, hit);
            if (chit->next_using > out->next_using) out = chit;
        }
        else
        {
            size_t next_using = find(lookup + i_hit + 1, n_hits - i_hit - 1, hit);
            debug << "no in cache, next_using = ";
            if (next_using == 0xDEAD0BED)
            {
                debug << reinterpret_cast<void*>(next_using) << '\n';
                continue;
            }
            else debug << next_using << '\n';
            debug << "out = {" << out->elem_ << ", " << out->next_using << "}\n";

            if (pcache.cache_.size() < cache_size)
            {
                pcache.cache_.emplace_front(cache_part::func((*slow_get_page)(hit), hit, next_using));
                pcache.hash_.emplace(hit, pcache.cache_.begin());
                out = pcache.cache_.begin();
                for (auto i = pcache.cache_.begin(); i != pcache.cache_.end(); ++i)
                {
                    if (out->next_using < i->next_using)
                        out = i;
                }
            }
            else if (out->next_using > next_using)
            {
                debug << "add elem in cache\n";
                if (pcache.cache_.size() == cache_size)
                {
                    // out = pcache.cache_.begin();
                    // for (auto i = pcache.cache_.begin(); i != pcache.cache_.end(); ++i)
                    // {
                    //     if (out->next_using > i->next_using)
                    //         out = i;
                    // }
                    pcache.hash_.erase(out->key_);
                    pcache.cache_.erase(out);
                }
                pcache.cache_.emplace_front(cache_part::func((*slow_get_page)(hit), hit, next_using));
                pcache.hash_.emplace(hit, pcache.cache_.begin());
                out = pcache.cache_.begin();
                for (auto i = pcache.cache_.begin(); i != pcache.cache_.end(); ++i)
                {
                    if (out->next_using < i->next_using)
                        out = i;
                }

            }
        }
        debug << '\n';
    }

    return value;
  }

} //namespace cache

