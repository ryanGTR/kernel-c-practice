"""two-sum-hash probing 圖解（手機看）。uv run --with matplotlib python make_diagrams.py"""
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrowPatch, Rectangle

plt.rcParams["font.family"] = "Noto Sans CJK TC"
MONO = ["DejaVu Sans Mono", "Noto Sans CJK TC"]
INK, FILL, EMPTY, HEAD, BAD, OK = "#1e2523", "#9fd3c7", "#f2f4f3", "#b23a3a", "#e8a0a0", "#2f7d5b"


def cell(ax, x, y, txt, fc, ec=INK, ls="-", fs=17):
    ax.add_patch(Rectangle((x, y), 0.92, 0.92, fc=fc, ec=ec, lw=1.8, ls=ls))
    ax.text(x + 0.46, y + 0.46, txt, ha="center", va="center", fontsize=fs, weight="bold")


def arrow(ax, a, b, color, rad=0.0, ls="-"):
    ax.add_patch(FancyArrowPatch(a, b, connectionstyle=f"arc3,rad={rad}", arrowstyle="-|>",
                                 mutation_scale=18, color=color, lw=2.2, ls=ls))


fig, ax = plt.subplots(figsize=(9, 9.6))
ax.set_xlim(-1.6, 10.2)
ax.set_ylim(0, 10)
ax.axis("off")

ax.text(4.3, 9.55, "find(13)：hash(13) & mask = 6，cap = 8（slot 0～7）",
        ha="center", fontsize=15, weight="bold")

# ---- 上：陣列
y = 7.0
keys = {6: "5", 7: "9"}
for i in range(8):
    used = i in keys
    cell(ax, i, y, keys.get(i, ""), FILL if used else EMPTY)
    ax.text(i + 0.46, y - 0.3, str(i), ha="center", fontsize=11, color="#666")
    ax.text(i + 0.46, y + 1.15, "T" if used else "F", ha="center", fontsize=12,
            color=INK if used else "#999")
ax.text(-0.25, y + 1.15, "used", ha="right", fontsize=12)
ax.text(-0.25, y + 0.46, "keys", ha="right", va="center", fontsize=12)
cell(ax, 8.15, y, "8?", BAD, ec=HEAD, ls="--")
ax.text(8.61, y + 1.15, "陣列外", ha="center", fontsize=11, color=HEAD)

arrow(ax, (6.46, y + 0.95), (7.46, y + 0.95), OK, rad=-0.6)
ax.text(6.95, y + 1.85, "① 5≠13 → 下一格", ha="center", fontsize=11, color=OK)
arrow(ax, (7.46, y - 0.05), (8.6, y - 0.05), HEAD, rad=0.6)
ax.text(8.5, y - 1.05, "② i = i + 1 → 8\n讀 used[8] 越界！", ha="center", fontsize=12,
        color=HEAD, weight="bold")
arrow(ax, (7.3, y - 0.05), (0.46, y - 0.05), "#4a6fa5", rad=0.28, ls="--")
ax.text(3.6, y - 0.75, "③ 正確：從 7 繞回 0（環狀）", ha="center", fontsize=12,
        color="#4a6fa5", weight="bold")

ax.text(4.3, 4.6, "ASan：heap-buffer-overflow on address … READ of size 1",
        ha="center", fontsize=11, family=MONO, color=HEAD)

# ---- 下：位元
ax.text(-1.3, 3.1, "用位元看 & mask（mask = cap − 1 = 7）", fontsize=14, weight="bold")
rows = [("i + 1 = 7  ", "0111", INK), ("mask = 7   ", "0111", INK), ("7 & 7 =    ", "0111 = 7 ✓", OK)]
for k, (lab, bits, c) in enumerate(rows):
    ax.text(-1.3, 2.4 - k * 0.45, lab + bits, fontsize=13, family=MONO, color=c)
rows2 = [("i + 1 = 8  ", "1000", INK), ("mask = 7   ", "0111", INK), ("8 & 7 =    ", "???? = ?", HEAD)]
for k, (lab, bits, c) in enumerate(rows2):
    ax.text(4.6, 2.4 - k * 0.45, lab + bits, fontsize=13, family=MONO, color=c)
ax.text(4.3, 0.6, "8 & 7 = ?　再寫：i = ______ 才會 6→7→0", ha="center", fontsize=15,
        weight="bold", color=HEAD)

fig.savefig("probe_wrap.png", dpi=110, bbox_inches="tight")
print("probe_wrap.png")
