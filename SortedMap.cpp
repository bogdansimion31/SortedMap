#include "SortedMap.h"
#include "SMIterator.h"
#include <cmath>
#include <exception>
using namespace std;
// theta(m)
SortedMap::SortedMap(Relation r) {
    this->r = r;
    this->m = 15;
    this->hash_table = new Node *[m];
    for (int i = 0; i < m; i++) {
        this->hash_table[i] = nullptr;
    }
    prime1 = 3;
    prime2 = 2;
    n = 0;
    findNewPrimes();
}

// O(m)
TValue SortedMap::add(TKey k, TValue v) {
    int i = 0;
    int position = hash(k, i);
    int position_removed = -1;
    while (i < m && hash_table[position] != nullptr) {
        if (hash_table[position]->removed && position_removed == -1) {
            position_removed = position;
        }
        if (hash_table[position]->elem.first == k) {
            if (hash_table[position]->removed) {
                n++;
            }
            int return_value = hash_table[position]->elem.second;
            delete hash_table[position];
            hash_table[position] = new Node{make_pair(k, v), false};
            return return_value;
        }
        i++;
        position = hash(k, i);
    }
    if (i == m) {
        resize();
        return add(k, v);
    }
    if (position_removed != -1) {
        position = position_removed;
    }
    if (hash_table[position] != nullptr) {
        delete hash_table[position];
    }
    hash_table[position] = new Node{make_pair(k, v), false};
    n++;
    return NULL_TVALUE;
}
// O(m)
TValue SortedMap::search(TKey k) const {
    int i = 0;
    int position = hash(k, i);
    while (i < m && (hash_table[position] != nullptr && hash_table[position]->elem.first != k)) {
        i++;
        position = hash(k, i);
    }
    if (i == m) {
        return NULL_TVALUE;
    }
    if (hash_table[position] == nullptr || hash_table[position]->removed) {
        return NULL_TVALUE;
    }
    return hash_table[position]->elem.second;
}
// O(m)
TValue SortedMap::remove(TKey k) {
    int i = 0;
    int position = hash(k, i);
    while (i < m && hash_table[position] != nullptr && hash_table[position]->elem.first != k) {
        i++;
        position = hash(k, i);
    }
    if (i == m) {
        return NULL_TVALUE;
    }
    if (hash_table[position] == nullptr || hash_table[position]->removed) {
        return NULL_TVALUE;
    }
    hash_table[position]->removed = true;
    n--;
    return hash_table[position]->elem.second;
}
// theta(1)
int SortedMap::size() const {
    return n;
}
// theta(1)
bool SortedMap::isEmpty() const {
    return size() == 0;
}
// theta(1)
SMIterator SortedMap::iterator() const {
    return SMIterator(*this);
}
// theta(m)
SortedMap::~SortedMap() {
    for (int i = 0; i < m; i++) {
        if (hash_table[i] != nullptr) {
            delete hash_table[i];
        }
    }
    delete[] hash_table;
}
// theta(1)
long long SortedMap::hash(TKey key, int i) const {
    int pos = (key % prime1 + i * (prime2 - key % prime2)) % prime1;
    if (pos < 0) {
        pos = pos * (-1);
    }
    return pos;
}
// theta(m)
void SortedMap::resize() {
    auto old_table = this->hash_table;
    this->m = m * 2;
    findNewPrimes();
    this->hash_table = new Node *[m];
    for (int i = 0; i < m; i++) {
        this->hash_table[i] = nullptr;
    }
    for (int i = 0; i < m / 2; i++) {
        if (old_table[i] == nullptr) {
            continue;
        }
        if (old_table[i]->removed) {
            delete old_table[i];
            continue;
        }
        int old_n = n;
        add(old_table[i]->elem.first, old_table[i]->elem.second);
        n = old_n;
        delete old_table[i];
    }
    delete[] old_table;
}
// theta(1)
long double SortedMap::loadFactor() {
    return (long double)size() / m;
}
// O(m2)
void SortedMap::findNewPrimes() {
    for (long long i = prime1; i < m; i++)
        for (long long j = 2; j * j <= i; j++) {
            if (i % j == 0)
                break;
            else if (j + 1 > sqrt(i)) {
                prime2 = prime1;
                prime1 = i;
            }
        }
}

// theta(m)
void SortedMap::filter(Condition c) {
    for (int i = 0; i < m; i++) {
        if (hash_table[i] != nullptr && !hash_table[i]->removed) {
            if (!c(hash_table[i]->elem.first)) {
                hash_table[i]->removed = true;
            }
        }
    }
}
