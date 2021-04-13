#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;
// O(n2)
SMIterator::SMIterator(const SortedMap &m) : map(m) {
    size = map.size();
    elems = new TElem[size];
    int j = 0;
    current = 0;
    for (int i = 0; i < map.m; i++) {
        if (map.hash_table[i] != nullptr && !map.hash_table[i]->removed) {
            elems[j++] = map.hash_table[i]->elem;
        }
    }
    quickSort(elems, 0, size - 1);
}
// theta(1)
void SMIterator::first() {
    current = 0;
}
// theta(1)
void SMIterator::next() {
    if (!valid()) {
        throw std::exception();
    }
    current++;
}
// theta(1)
bool SMIterator::valid() const {
    return current < size;
}
// theta(1)
TElem SMIterator::getCurrent() const {
    if (!valid()) {
        throw std::exception();
    }
    return elems[current];
}
// O(n)
int SMIterator::partition(TElem arr[], int low, int high) {
    TElem pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (map.r(arr[j].first, pivot.first)) {
            i++;
            auto t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    auto t = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = t;
    return (i + 1);
}
// O(n2)
void SMIterator::quickSort(TElem arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
// theta(1)
SMIterator::~SMIterator() {
    delete[] elems;
}
