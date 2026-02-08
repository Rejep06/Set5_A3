#include "../include/HashFuncGEn.h"

uint32_t HashFuncGen::addmod(uint32_t x, uint32_t y) const {
    uint64_t sum = (uint64_t)x + y;
    if (sum >= M) sum -= M;
    return (uint32_t)sum;
}

uint32_t HashFuncGen::mulmod(uint32_t x, uint32_t y) const {
    uint64_t prod = (uint64_t)x * y;
    return (uint32_t)(prod % M);
}

uint64_t HashFuncGen::stringToFieldElement(const string& s) const {
    const uint64_t B = 19260817;
    uint32_t x = 0;
    for (unsigned char c : s) {
        uint32_t v = (uint32_t)c + 1;
        x = addmod(mulmod(x, B), v % M);
    }
    return x;
}

uint32_t HashFuncGen::polyHash(uint32_t x) const {
    uint32_t res = a[k - 1];
    for (int i = k - 2; i >= 0; --i) {
        res = addmod(mulmod(res, x), a[i]);
    }
    return res;
}


HashFuncGen::HashFuncGen(int k, uint64_t seed) : k(k), rng(seed) {
    uniform_int_distribution<uint32_t> dist(0, M - 1);
    a.reserve(k);
    for (int i = 0; i < k; ++i) {
        a.push_back(dist(rng));
    }
}

uint32_t HashFuncGen::operator()(const string& s) const {
    uint32_t x = stringToFieldElement(s);
    return polyHash(x);
}
