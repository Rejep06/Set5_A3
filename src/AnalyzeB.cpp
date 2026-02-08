#include<bits/stdc++.h>
#include "../include/HyperLogLog.h"
#include "../include/HashFuncGen.h"
#include "../include/RandomStreamGen.h"

using namespace std;

struct DistStats {
    double mu;  
    double sd;
    double cv;      
    int mn, mx;      
    double rho_mu;  
    double rho_sd;
};

static DistStats analyzeForB(const vector<uint32_t>& hashes, int B) {
    int m = 1 << B;
    vector<int> cnt(m, 0);
    vector<double> rho_sum(m, 0.0);
    vector<int> rho_cnt(m, 0);

    auto rho_tail = [&](uint32_t x)->int {
        uint32_t w = (x << B);
        int maxBits = 32 - B;
        if (w == 0) return maxBits + 1;
        int lz = __builtin_clz(w);
        int r = lz + 1;
        int cap = maxBits + 1;
        if (r > cap) r = cap;
        return r;
    };

    for (uint32_t x : hashes) {
        uint32_t j = x >> (32 - B);
        cnt[j]++;
        int r = rho_tail(x);
        rho_sum[j] += r;
        rho_cnt[j] += 1;
    }

    double mu = (double)hashes.size() / (double)m;
    double s2 = 0.0;
    int mn = cnt[0], mx = cnt[0];
    for (int c : cnt) {
        mn = min(mn, c);
        mx = max(mx, c);
        double d = (double)c - mu;
        s2 += d * d;
    }
    double sd = sqrt(s2 / (double)m);
    double cv = (mu > 0 ? sd / mu : 0);

    vector<double> rho_avg(m, 0.0);
    for (int j = 0; j < m; ++j) {
        if (rho_cnt[j] > 0) rho_avg[j] = rho_sum[j] / (double)rho_cnt[j];
        else rho_avg[j] = 0.0;
    }
    double rho_mu = 0.0;
    for (double v : rho_avg) rho_mu += v;
    rho_mu /= (double)m;

    double rho_s2 = 0.0;
    for (double v : rho_avg) {
        double d = v - rho_mu;
        rho_s2 += d * d;
    }
    double rho_sd = sqrt(rho_s2 / (double)m);

    return {mu, sd, cv, mn, mx, rho_mu, rho_sd};
}

static uint64_t exactUnique(const RandomStreamGen& gen) {
    unordered_set<string> s;
    s.reserve(gen.size() * 2);
    for (size_t i = 0; i < gen.size(); ++i) s.insert(gen.at(i));
    return (uint64_t)s.size();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const uint64_t seed = 12345;
    const size_t N = 300000;
    const int K_POLY = 5;        
    const int B_MIN = 8;
    const int B_MAX = 16;

    RandomStreamGen gen(seed, N);
    HashFuncGen hf(K_POLY, seed);

    vector<uint32_t> hashes;
    hashes.reserve(N);
    for (size_t i = 0; i < N; ++i) hashes.push_back(hf(gen.at(i)));

    uint64_t exact = exactUnique(gen);

    cout << "Exact unique F0 = " << exact << "\n\n";
    cout << "B,m,mu=N/m,cv_cnt,min_cnt,max_cnt,rho_mu_reg,rho_sd_reg,HLL_est,rel_error\n";

    int bestB = -1;
    double bestScore = 1e100;

    for (int B = B_MIN; B <= B_MAX; ++B) {
        int m = 1 << B;

        DistStats st = analyzeForB(hashes, B);

        HyperLogLog hll(B);
        for (uint32_t x : hashes) hll.add(x);
        double est = hll.estimate();
        double rel_err = fabs(est - (double)exact) / (double)exact;

        cout << B << "," << m << ","
             << st.mu << "," << st.cv << ","
             << st.mn << "," << st.mx << ","
             << st.rho_mu << "," << st.rho_sd << ","
             << est << "," << rel_err << "\n";

        if (st.mu >= 20.0) {
            double score = rel_err + 0.15 * st.cv;
            if (score < bestScore) {
                bestScore = score;
                bestB = B;
            }
        }
    }

    cout << "\nB = " << bestB <<
    \n;

    return 0;
}