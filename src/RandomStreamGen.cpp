#include "../include/RandomStreamGen.h"

string RandomStreamGen::genString() {
    uniform_int_distribution<int> ld(1, 30);
    uniform_int_distribution<int> chd(0, (int)alphabet.size() - 1);
    string res;
    size_t len = ld(rng);
    for (size_t i = 0; i < len; ++i) {
        res.push_back(alphabet[chd(rng)]);
    }
    return res;
}

RandomStreamGen::RandomStreamGen(uint64_t seed, size_t n) : n(n), rng(seed) {
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-";
    stream.reserve(n);
    for (size_t i = 0; i < n; ++i) stream.push_back(genString());
}

size_t RandomStreamGen::size() const { return n; }

const string& RandomStreamGen::at(size_t i) const { return stream[i]; }

size_t RandomStreamGen::prefixLen(double frac) const {
    if (frac < 0) frac = 0;
    if (frac > 1) frac = 1;
    return (size_t)floor(frac * (double)n);
}


vector<size_t> RandomStreamGen::timePoints(double step) const {
    vector<size_t> pts;
    if (step <= 0) return pts;

    for (double f = step; f <= 1.0 + 1e-12; f += step) {
        size_t len = prefixLen(f);
        if (pts.empty() || pts.back() != len) pts.push_back(len);
    }
    if (pts.empty() || pts.back() != n) pts.push_back(n);
    return pts;
}
