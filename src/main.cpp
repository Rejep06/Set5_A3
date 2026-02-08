#include <bits/stdc++.h>
#include "../include/HyperLogLog.h"
#include "../include/HashFuncGen.h"
#include "../include/RandomStreamGen.h"

using namespace std;

static double mean(const vector<double>& v) {
    double s = 0;
    for (double x : v) s += x;
    return s / (double)v.size();
}

static double stdev(const vector<double>& v) {
    if (v.size() < 2) return 0.0;
    double m = mean(v);
    double s2 = 0;
    for (double x : v) s2 += (x - m) * (x - m);
    return sqrt(s2 / (double)(v.size() - 1));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const size_t N = 300000;
    const double STEP = 0.1;
    const int STREAMS = 30;
    const int B = 9; // Лучший выбор
    const int K_POLY = 5;

    RandomStreamGen tmp(1, N);
    vector<size_t> pts = tmp.timePoints(STEP);

    int T = (int)pts.size();

    vector<vector<double>> all_est(T);

    vector<size_t> x_axis = pts;
    vector<uint64_t> exact_first(T, 0);
    vector<double> est_first(T, 0);

    for (int s = 0; s < STREAMS; ++s) {
        uint64_t seed = 12345 + 1000ull * s;

        RandomStreamGen gen(seed, N);
        HashFuncGen hf(K_POLY, seed);
        HyperLogLog hll(B);

        unordered_set<string> seen;
        seen.reserve(N * 2);

        int t_idx = 0;
        size_t next_len = pts[t_idx];

        for (size_t i = 0; i < N; ++i) {
            const string& str = gen.at(i);

            seen.insert(str);
            hll.add(hf(str));

            if (i + 1 == next_len) {
                uint64_t exact = (uint64_t)seen.size();
                double est = hll.estimate();

                all_est[t_idx].push_back(est);

                if (s == 0) {
                    exact_first[t_idx] = exact;
                    est_first[t_idx] = est;
                }

                t_idx++;
                if (t_idx >= T) break;
                next_len = pts[t_idx];
            }
        }
    }

    vector<double> mean_est(T), std_est(T);
    for (int i = 0; i < T; ++i) {
        mean_est[i] = mean(all_est[i]);
        std_est[i] = stdev(all_est[i]);
    }

    {
        ofstream out("graph1.csv");
        out << "prefix_len,exact,estimate\n";
        for (int i = 0; i < T; ++i) {
            out << x_axis[i] << "," << exact_first[i] << "," << est_first[i] << "\n";
        }
    }

    {
        ofstream out("graph2.csv");
        out << "prefix_len,mean_est,std_est,upper,lower\n";
        for (int i = 0; i < T; ++i) {
            out << x_axis[i] << ","
                << mean_est[i] << ","
                << std_est[i] << ","
                << (mean_est[i] + std_est[i]) << ","
                << (mean_est[i] - std_est[i]) << "\n";
        }
    }

    return 0;
}
