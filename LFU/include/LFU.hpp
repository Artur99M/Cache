#pragma once
#include <unordered_map>
#include <list>
#include <utility>
#include <valarray>
#include <vector>
#include <iostream>
namespace cache
{
    template<typename T, typename keyT = int>
    struct LFU_cache
    {
        private:
        //TYPES
        struct TK
        {
            T value_;
            keyT key_;
            size_t n_used_;
        };
        using it = typename std::list<TK>::iterator;
        using cit = typename std::list<TK>::const_iterator;
        // using it_h = typename std::unordered_map<keyT, cit>::iterator;

        //MEMBERS
        std::list<TK> cache_;
        std::unordered_map<keyT, it> hash_;
        size_t sz_ = 5;

        cit elem_out () const
        {
            std::vector<cit> mins;
            mins.emplace_back (cache_.begin());

            for (auto i = std::next (cache_.begin()); i != cache_.end(); i++)
            {
                if ((*i).n_used_ < (*(mins[0])).n_used_)
                {
                    mins.clear();
                    mins.push_back (i);
                }
                else if ((*i).n_used_ == (*(mins[0])).n_used_)
                {
                    mins.push_back (i);
                }
            }
            return mins[mins.size() - 1];

        }
        LFU_cache () {}
        public:
        explicit LFU_cache (size_t sz) : sz_(sz) {if (sz_ <= 0) sz_ = 5;}
        inline size_t size() const {return sz_;}
        inline size_t set_size (size_t sz)
        {
            if (sz > 0)
            {
                if (sz < sz_)
                {
                    for (int i = sz_; i > sz; i++)
                    {
                        auto oit = elem_out();
                        hash_.erase((*oit).key_); // for this line key_ was added in TK
                        cache_.erase (oit);
                    }
                }
                sz_ = sz;
            }
            return sz_;
        }

        template <class F> bool lookup_update (keyT key, F slow_get_page)
        {
            auto hit = hash_.find(key);

            if (hit == hash_.end())
            {
                if (sz_ == cache_.size())
                {
                    auto oit = elem_out ();
                    hash_.erase((*oit).key_); // for this line key_ was added in TK
                    cache_.erase (oit);
                }
                TK new_elem = {slow_get_page (key), key, 1};
                cache_.emplace_front (new_elem);
                hash_.emplace (key, cache_.begin());

                return false;
            }
            else
            {
                auto eltit = hit->second;
                if (eltit != cache_.begin())
                    cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
                ++((*eltit).n_used_);
                return true;
            }
        }
    };
}
