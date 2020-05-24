#pragma once

#include <vector>
#include <utility>

class Dsu {
private:
    struct Item {
        mutable size_t RootIndex = 0;
        size_t Size = 0;
    };
    std::vector<Item> Items;
public:
    explicit Dsu(size_t size) 
        : Items(size) 
    {
        for (size_t ind = 0; ind < size; ++ind) {
            Items[ind] = { ind, 1 };
        }
    }

    size_t GetRootIndex(size_t ind) const {
        if (Items[ind].RootIndex == ind) {
            return ind;
        }
        Items[ind].RootIndex = GetRootIndex(Items[ind].RootIndex);
        return Items[ind].RootIndex;
    }
    
    size_t GetSetSize(size_t ind) const {
        return Items[GetRootIndex(ind)].Size;
    }

    bool TryUnion(size_t x, size_t y) {
        size_t rootX = GetRootIndex(x);
        size_t rootY = GetRootIndex(y);

        if (rootX == rootY) {
            return false;
        }

        if (Items[rootX].Size < Items[rootY].Size) {
            std::swap(rootX, rootY);
        }
        Items[rootY].RootIndex = rootX;
        Items[rootX].Size += Items[rootY].Size;

        return true;
    }
};
