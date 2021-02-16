//
// Created by bczhc on 2/13/21.
//

#ifndef BCZHC_CPP_SYMBOL_TABLE_HPP
#define BCZHC_CPP_SYMBOL_TABLE_HPP

#include "./linked_list.hpp"

using namespace bczhc;

namespace bczhc {
    template<typename K, typename V>
    class KVPair {
    public:
        K key{};
        V val{};

        KVPair(K key, V val) : key(key), val(val) {}

        KVPair() {}
    };

    template<typename K, typename V>
    class SymbolTable : public LinkedList<KVPair<K, V>> {
    public:
        using Pair = KVPair<K, V>;

        void put(K k, V v) {
            this->insert(KVPair<K, V>(k, v));
        }

        V get(K k) {
            auto it = this->getIterator();
            size_t index = 0;
            while (it.hasNext()) {
                auto e = it.next();
                if (e.key == k) return e.val;
                ++index;
            }
            throw NoSuchElementException();
        }
    };
} // namespace bczhc

#endif //BCZHC_CPP_SYMBOL_TABLE_HPP
