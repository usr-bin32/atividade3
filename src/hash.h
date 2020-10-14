#include <functional>
#include <vector>

constexpr int EMPTY_RECORD_KEY = -1;
typedef std::function<int (int, unsigned int)> HashFunction;

int unit_hash(int k, unsigned int m) {
    return 1;
}

int division_hash(int k, unsigned int m) {
    return k % m;
}

int multiplication_hash(int k, int m) {
    double A = 0.6180339887;
    return static_cast<int>(m * (k * A - static_cast<int>(k * A)));
}

template <typename T>
struct Record {
    Record() {
        this->key = EMPTY_RECORD_KEY;
    }

    Record(int key, T const &data) {
        this->key = key;
        this->data = data;
    }

    bool empty() {
        return this->key == EMPTY_RECORD_KEY;
    }

    int key;
    T data;
};

template <typename T>
class HashTable {
  public:
    HashTable(unsigned int m, HashFunction h2) {
        this->table.resize(m);
        this->h1 = division_hash;
        this->h2 = h2;
    }

    T *insert(int key, T const &data) {
        Record<T> *record = this->probe(key);
        if (record == nullptr) {
            return nullptr;
        }

        record->key = key;
        record->data = data;

        return &record->data;
    }

    T *lookup(int key) {
        Record<T> *record = this->probe(key);
        if (record == nullptr || record->empty()) {
            return nullptr;
        }
        return &record->data;
    }

    unsigned int collisions() {
        unsigned int count = 0;
        for (auto &record : this->table) {
            if (!record.empty()) {
                count += this->probe_collisions(record.key);
            }
        }
        return count;
    }
    std::vector<Record<T>> table;

  private:
    unsigned int probe(int key, Record<T> *&result) {
        unsigned int i;
        for (i = 0; i < this->table.size(); i++) {
            Record<T> *record = &this->table[hash(key, i)];

            if (record->empty() || record->key == key) {
                result = record;
                break;
            }
        }
        return i;
    }

    Record<T> *probe(int key) {
        Record<T> *record = nullptr;
        this->probe(key, record);

        return record;
    }

    unsigned int probe_collisions(int key) {
        Record<T> *_ = nullptr;
        return this->probe(key, _);
    }

    int hash(int key, int index) {
        unsigned int m = this->table.size();
        return (h1(key, m) + index * h2(key, m)) % m;
    }

    HashFunction h1;
    HashFunction h2;
};
