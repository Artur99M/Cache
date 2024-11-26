# Cache by Artur Minnakhmetov
## Building
Для того, чтобы сгенерировать _Makefile_ или другую системы сборки, необходимо написать в Терминал
```
cmake -S . -B build
```
Для того, чтобы скомпилировать исполняемый файл, необходимо написать
```
cmake --build build
```

## Testing
Для теститрования __LFU__ кэша (наименее использумый элемент) необходимо написать в Терминал (работает только на Linux и MacOS)
```
cmake --build ./build --target test ARGS="-C LFU"
```
Для тестирования __perfect cache__ необходимо написать в Терминал (работает только на Linux и MacOS)
```
cmake --build ./build --target test ARGS="-C PC"
```

## LFU
Основная функция:
```C++
template <class F> bool lookup_update (keyT key, F slow_get_page)
        {
            auto hit = hash_.find(key);

            if (hit == hash_.end())
            {
                if (sz_ == cache_.size())
                {
                    auto oit = elem_out ();
                    hash_.erase((*oit).key_);
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
```
_slow_get_page_ - функция, которая возвращает страницу, которая имеет ключ _key_

Алгоритм:
1) Ищется элемент, полученный от пользователя, в кэше. Если он есть в кэше, то работа заканчивается, иначе идем дальше.
2) Ищется элемент в кэше, который использовался реже всех или, если такого не нашлось, тот, который использовался раньше других, и заменяется на тот, который получили от пользователя. Работа заканчивается

## Perfect Cache

Основная функция:
```C++
template <class F> bool lookup_update (keyT key, F slow_get_page)
        {
            auto hit = hash_.find(key);
            std::list<size_t>& l = lookup_[key];
            l.erase(l.begin());
            size_t next_using = *(l.begin());

            if (hit == hash_.end())
            {
                if (next_using == 0xdead0bed) return false;
                if (sz_ == cache_.size())
                {
                    auto oit = elem_out ();
                    if (oit->next_using < next_using)
                        return false;
                    hash_.erase((*oit).key_);
                    cache_.erase (oit);
                }
                TK new_elem = {slow_get_page (key), key, next_using};
                cache_.emplace_front (new_elem);
                hash_.emplace (key, cache_.begin());

                return false;
            }
            else
            {
                hit->second->next_using = next_using;
                return true;
            }
        }
```

Алгоритм:
1) Элемент, полученный от пользователя, ищется в кэше, если его там нет, переходим к пункту 2, иначе заканчиваем.
2) Если он не встречается потом, то алгоритм закчивается. Если он встретился, то ищем элемент в кэше, который встретится позже других.
3) Если элемент кэша встретится позже элемента, полученного от пользователя, то он заменяется.
