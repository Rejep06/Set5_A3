import pandas as pd
import matplotlib.pyplot as plt

# === График №1: Nt vs F0^t ===
df1 = pd.read_csv("graph1.csv")  # prefix_len, exact, estimate

plt.figure()
plt.plot(df1["prefix_len"], df1["exact"], label="Exact F0^t")
plt.plot(df1["prefix_len"], df1["estimate"], label="HLL Nt")
plt.xlabel("Processed prefix length (t)")
plt.ylabel("Unique elements")
plt.title("HyperLogLog estimate vs exact")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# === График №2: mean(Nt) ± stdev ===
df2 = pd.read_csv("graph2.csv")  # prefix_len, mean_est, std_est, upper, lower

plt.figure()
plt.plot(df2["prefix_len"], df2["mean_est"], label="E(Nt)")
plt.fill_between(df2["prefix_len"], df2["lower"], df2["upper"], alpha=0.3, label="E(Nt) ± σ")
plt.xlabel("Processed prefix length (t)")
plt.ylabel("Unique elements")
plt.title("HyperLogLog mean estimate and uncertainty band")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
