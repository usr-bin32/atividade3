#include <fstream>
#include <iostream>

#include "hash.h"

int main() {
    std::ifstream file("./input");
    if (!file.is_open()) {
        std::cerr << "Failed to open the input file!" << std::endl;
        return -1;
    }

    int m = -1;
    int n = -1;
    int s = -1;
    file >> m >> n >> s;

    HashFunction h2;
    if (s == 1) {
        // para a sondagem linear, usa-se h2(k) = 1
        h2 = unit_hash;
    } else {
        // para a sondagem dupla, usa-se h2(k) = multiplication
        h2 = multiplication_hash;
    }

    HashTable<int> hash_table(m, h2);

    for (int i = 0; i < n; i++) {
        int k = -1;
        file >> k;

        hash_table.insert(k, k * 2);
    }

    std::cout << "Collisions: " << hash_table.collisions() << std::endl;

    return 0;
}
