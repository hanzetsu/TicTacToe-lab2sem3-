#include <iostream>
#include "structures/HashTableDictionary.hpp"

int main() {
    std::cout << "1. Before construction\n";
    HashTableDictionary<uint32_t, int> d;
    std::cout << "2. Constructed, count=" << d.GetCount()
              << ", capacity=" << d.GetCapacity() << "\n";

    std::cout << "3. Before Add(1, 100)\n";
    d.Add(1, 100);
    std::cout << "4. After Add, count=" << d.GetCount() << "\n";

    std::cout << "5. Get(1) = " << d.Get(1) << "\n";
    return 0;
}