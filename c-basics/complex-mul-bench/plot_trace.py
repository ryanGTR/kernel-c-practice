"""plot_trace.py — 畫 cmul_trace 的逐輪時間，看暖機爬升。
用法：python plot_trace.py trace_43.csv trace_34.csv → trace.png
"""
import csv
import sys

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402

plt.rcParams["font.sans-serif"] = ["Noto Sans CJK TC", "Noto Sans TC", "DejaVu Sans"]
files = sys.argv[1:]
fig, axes = plt.subplots(len(files), 1, figsize=(10, 3.6 * len(files)), sharey=True)
colors = {"mul4": "#1f6feb", "mul3": "#d1242f"}
for ax, f in zip(axes, files):
    rows = list(csv.reader(open(f)))
    for i, (v, _r, ns) in enumerate(rows):
        ax.scatter(i, float(ns), s=4, color=colors[v])
    first = rows[0][0]
    ax.set_title(f"{f}：先跑 {first}（每點＝一輪 {4096 * 50:,} 次乘法）")
    ax.set_ylabel("ns / 次")
    ax.axvline(len(rows) / 2, color="gray", lw=0.8, ls="--")
    for v, c in colors.items():
        ax.scatter([], [], color=c, label=v)
    ax.legend(loc="upper right")
    ax.grid(alpha=0.3)
axes[-1].set_xlabel("執行順序（輪）")
fig.tight_layout()
fig.savefig("trace.png", dpi=120)
print("saved trace.png")
