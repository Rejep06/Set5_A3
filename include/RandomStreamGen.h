#pragma once
#include<bits/stdc++.h>

using namespace std;

class RandomStreamGen {
    string alphabet;
    size_t n;
    vector<string> stream;
    mt19937_64 rng;

    string genString();
public:
    RandomStreamGen(uint64_t seed, size_t n);

    size_t size() const;

    const string& at(size_t i) const;

    size_t prefixLen(double frac) const;

    vector<size_t> timePoints(double step) const;
};