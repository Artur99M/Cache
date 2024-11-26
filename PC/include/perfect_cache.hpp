#pragma once
#include <unordered_map>
#include <list>
#include <utility>
#include <valarray>
#include <vector>
#include <iostream>
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
    template<typename T, typename keyT = int>
    struct PC_cache
    {
        private:
        //TYPES
        struct TK
        {
            T value_;
            keyT key_;
            size_t next_using;
        };
        using it = typename std::list<TK>::iterator;
        using cit = typename std::list<TK>::const_iterator;
        // using it_h = typename std::unordered_map<keyT, cit>::iterator;

        //MEMBERS
        std::list<TK> cache_;
        std::unordered_map<keyT, it> hash_;
        std::unordered_map<keyT, std::list<size_t>> lookup_;
        size_t sz_ = 5;

        cit elem_out () const
        {
            cit out = cache_.begin();
            cit end = cache_.end();
            for (cit i = out; i != end; ++i)
            {
                if (out->next_using < i->next_using)
                    out = i;
            }

            return out;
        }
        PC_cache () {}
        public:
        explicit PC_cache (size_t sz) : sz_(sz) {if (sz_ <= 0) sz_ = 5;}
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

        void make_lookup(keyT* lookup, size_t n)
        {
            for (size_t i = 0; i < n; ++i)
            {
                auto f = lookup_.find(lookup[i]);
                if (f == lookup_.end())
                {
                    std::list<size_t> l;
                    l.emplace_front(i);
                    lookup_.emplace(lookup[i], l);
                }
                else
                {
                    (f->second).emplace_back(i);
                }
            }
            for (auto i = lookup_.begin(); i != lookup_.end(); ++i)
            {
                (i->second).emplace_back(0xDEAD0BED);
                debug << i->first << ": ";
                for (auto j = i->second.begin(); j != i->second.end(); j++)
                    debug << *j << ' ';
                debug << '\n';
            }

        }

        template <class F> bool lookup_update (keyT key, F slow_get_page)
        {
            debug << "\nkey = " << key << "\ncache:\n";
            for (it i = cache_.begin(); i != cache_.end(); ++i)
            {
                debug << '{' << i->next_using << ", " << i->key_ << "} ";
            }
            debug << '\n';
            auto hit = hash_.find(key);
            std::list<size_t>& l = lookup_[key];
            l.erase(l.begin());
            size_t next_using = *(l.begin());
            debug << "next_using = " << next_using << '\n';

            if (hit == hash_.end())
            {
                debug << "no in cache\n";
                if (next_using == 0xdead0bed) return false;
                if (sz_ == cache_.size())
                {
                    auto oit = elem_out ();
                    debug << "oit = {" << oit->next_using << ", " << oit->key_ << "}\n";
                    debug << "oit->next_using < next_using = " << (oit->next_using < next_using) << '\n';
                    if (oit->next_using < next_using)
                        return false;
                    hash_.erase((*oit).key_); // for this line key_ was added in TK
                    cache_.erase (oit);
                }
                TK new_elem = {slow_get_page (key), key, next_using};
                cache_.emplace_front (new_elem);
                hash_.emplace (key, cache_.begin());

                return false;
            }
            else
            {
                debug << "in cache\n";
                hit->second->next_using = next_using;
                return true;
            }
        }
    };
}
