#pragma once
#include<bits/stdc++.h>

using namespace std;

class HashFuncGen {
    int k;
    vector<uint32_t> a;      // коэффициенты a0..a_{k-1}
    mutable mt19937_64 rng;

    uint32_t addmod(uint32_t x, uint32_t y) const;
    uint32_t mulmod(uint32_t x, uint32_t y) const;

    uint64_t stringToFieldElement(const string& s) const;
    uint32_t polyHash(uint32_t x) const;

public:
    static const uint32_t M = 4294967291; // простое

    HashFuncGen(int k, uint64_t seed);

    uint32_t operator()(const string& s) const;
};
