#include <iostream>

#include "hash_set.hpp"


int main() {
    hash_set set1;
    set1.add(1);
    set1.add(2);
    set1.add(3);

    hash_set set2;
    set2.add(3);
    set2.add(4);
    set2.add(5);

    const hash_set union_set = set1 + set2;
    std::cout << "Union: " << union_set << std::endl;

    return 0;
}
