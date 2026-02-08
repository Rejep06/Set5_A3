#pragma once
#include<bits/stdc++.h>

using namespace std;

class HyperLogLog {
public:
    HyperLogLog(int B);

    void add(uint32_t x);

    double estimate() const;

    void reset();
private:
    int B;
    uint32_t m;
    vector<uint8_t> regs;
    double alpha;

    double calcAlpha(uint32_t m);
    uint8_t rho(uint32_t w, int mx);
};