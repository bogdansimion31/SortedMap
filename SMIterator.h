#pragma once
#include "SortedMap.h"

// DO NOT CHANGE THIS PART
class SMIterator {
    friend class SortedMap;

  private:
    const SortedMap &map;
    SMIterator(const SortedMap &mapionar);

    int size;
    TElem *elems;
    int partition(TElem *arr, int low, int high);
    void quickSort(TElem *arr, int low, int high);
    int current;

  public:
    ~SMIterator();
    void first();
    void next();
    bool valid() const;
    TElem getCurrent() const;
};
