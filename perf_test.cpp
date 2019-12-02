#include <hashtable.h>
#include <random>
#include <iostream>

#define TABLE_SIZE 100000

inline std::string GetString(const std::string alphabet, std::mt19937& gen,
                      std::uniform_int_distribution<>& dis) {
    std::string result;
    for (size_t i = 0; i < 16; ++i) {
        result += alphabet[dis(gen)];
    }
    return result;
}

std::string GetAlphabet() {
    std::string alphabet;
    for (size_t i = 0; i < 26; ++i) {
        alphabet += 'a' + i;
        alphabet += 'A' + i;
        if (i < 10) {
            alphabet += '0' + i;
        }
    }
    return alphabet;
}

int main() {
    std::string alphabet = GetAlphabet();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, alphabet.size());
    std::vector<std::string> strings;

    Hashtable<std::string, size_t> table(TABLE_SIZE);

    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        strings.push_back(GetString(alphabet, gen, dis));
    }
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        table.Insert(strings[i], i);
    }
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        if (table.Find(strings[i])->second != i) {
            std::cout << "asd\n";
        }
    }
}