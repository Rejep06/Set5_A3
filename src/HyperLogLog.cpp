#include "../include/HyperLogLog.h"

HyperLogLog::HyperLogLog(int B) : B(B) {
    m = 1u << B;
    regs.assign(m, 0);
    alpha = calcAlpha(m);
}

void HyperLogLog::add(uint32_t x) {
    uint32_t j = x >> (32 - B);
    uint32_t w = (x << B);
    uint8_t r = rho(w, 32 - B);
    regs[j] = max(regs[j], r);
}

double HyperLogLog::estimate() const {
    double sum = 0.0;
    int V = 0;
    for (uint8_t r : regs) {
        sum += ldexp(1.0, -(int)r);
        if (r == 0) V++;
    }
    double E = alpha * (double)m * (double)m / sum;

    if (E <= 2.5 * (double)m && V > 0) {
        E = (double)m * log((double)m / (double)V);
    }

    return E;
}

void HyperLogLog::reset() { fill(regs.begin(), regs.end(), 0); }

double HyperLogLog::calcAlpha(uint32_t m) {
    if (m == 2) return 0.3512;
    if (m == 4) return 0.5324;
    if (m == 16) return 0.673;
    if (m == 32) return 0.697;
    if (m == 64) return 0.709;
    return 0.7213 / (1.0 + 1.079 / (double)m);
}

uint8_t HyperLogLog::rho(uint32_t w, int mx) {
    if (w == 0) return (uint8_t)(mx + 1);

    int r = __builtin_clz(w) + 1;

    if (r > mx + 1) r = mx + 1;
    return (uint8_t)r;
}
