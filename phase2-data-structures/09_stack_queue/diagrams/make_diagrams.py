"""09 ring buffer 圖解（手機看）。uv run --with matplotlib python make_diagrams.py"""
import math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrowPatch, Rectangle, Wedge

plt.rcParams["font.family"] = "Noto Sans CJK TC"
INK, FILL, EMPTY, HEAD, NEW, BAD = "#1e2523", "#9fd3c7", "#f2f4f3", "#b23a3a", "#f4b942", "#e8a0a0"


def row(ax, y, cells, cap, head=None, label=None, colors=None, x0=0.6):
    for i in range(cap):
        v = cells[i] if i < len(cells) else ""
        c = (colors or {}).get(i, FILL if v else EMPTY)
        ax.add_patch(Rectangle((x0 + i, y), 0.92, 0.92, fc=c, ec=INK, lw=1.5))
        ax.text(x0 + i + 0.46, y + 0.46, v, ha="center", va="center", fontsize=17, weight="bold")
        ax.text(x0 + i + 0.46, y - 0.25, str(i), ha="center", va="center", fontsize=11, color="#666")
    if head is not None:
        ax.annotate("head", (x0 + head + 0.46, y + 0.95), (x0 + head + 0.46, y + 1.6),
                    ha="center", fontsize=13, color=HEAD, weight="bold",
                    arrowprops=dict(arrowstyle="-|>", color=HEAD, lw=2))
    if label:
        ax.text(x0 - 0.2, y + 0.46, label, ha="right", va="center", fontsize=13)


def canvas(h, title):
    fig, ax = plt.subplots(figsize=(9, h))
    ax.set_xlim(-2.6, 9.4)
    ax.set_ylim(0, h * 1.0)
    ax.axis("off")
    ax.set_title(title, fontsize=19, weight="bold", loc="left")
    return fig, ax


# 1 什麼是 ring buffer
fig, ax = canvas(8.6, "① ring buffer 是什麼：一排格子，頭尾接起來當圓圈用")
ax.text(-2.4, 7.7, "存三個數：data（格子）、head（排頭在哪格）、size（排了幾個）", fontsize=13)
row(ax, 5.2, ["", "", "", "", "", "A", "B", "C"], 8, head=5, label="data")
ax.text(-2.4, 4.2, "head=5、size=3 → 排隊順序是 A→B→C（從 head 往右數 size 格）", fontsize=13)
# circle view
cx, cy, R = 3.9, 1.85, 1.55
for i in range(8):
    a0 = 90 - (i + 1) * 45
    v = ["", "", "", "", "", "A", "B", "C"][i]
    ax.add_patch(Wedge((cx, cy), R, a0, a0 + 45, width=0.75, fc=FILL if v else EMPTY, ec=INK, lw=1.5))
    am = math.radians(a0 + 22.5)
    ax.text(cx + (R - 0.38) * math.cos(am), cy + (R - 0.38) * math.sin(am), v, ha="center", va="center",
            fontsize=15, weight="bold")
    ax.text(cx + (R + 0.28) * math.cos(am), cy + (R + 0.28) * math.sin(am), str(i), ha="center", va="center",
            fontsize=10, color="#666")
ax.text(cx, cy, "7 的下一格\n是 0", ha="center", va="center", fontsize=11)
ax.text(-2.4, 2.6, "把它想成圓：\n格子用完就繞回 0\n\nJava 的 ArrayDeque\n裡面就是這個", fontsize=12, va="top")
fig.savefig("ring1_what.png", dpi=110, bbox_inches="tight")

# 2 enqueue / dequeue
fig, ax = canvas(9.6, "② enqueue／dequeue：只動 head 和 size，資料不搬家")
ax.text(-2.4, 8.8, "enqueue(D)：寫到 (head + size) % capacity，再 size++", fontsize=13)
row(ax, 6.6, ["", "", "", "", "", "A", "B", "C"], 8, head=5, label="之前")
ax.text(8.9, 7.0, "size=3", fontsize=12)
row(ax, 4.2, ["D", "", "", "", "", "A", "B", "C"], 8, head=5, label="之後", colors={0: NEW})
ax.text(8.9, 4.6, "size=4", fontsize=12)
ax.text(0.6, 3.6, "(5+3)%8 = 0 → D 繞回最前面那格", fontsize=12, color="#8a5a00")
ax.text(-2.4, 2.85, "dequeue()：拿走 data[head]，head = (head+1) % capacity，size--", fontsize=13)
row(ax, 0.75, ["D", "", "", "", "", "", "B", "C"], 8, head=6, label="之後", colors={5: EMPTY})
ax.text(8.9, 1.15, "size=3", fontsize=12)
ax.text(0.6, 0.1, "A 被拿走；B、C、D 原地不動（不像陣列版要整排往左搬，那是 O(n)）", fontsize=12)
fig.savefig("ring2_ops.png", dpi=110, bbox_inches="tight")

# 3 resize trap
fig, ax = canvas(7.8, "③ 最大的坑：滿了要擴容，不能直接 realloc")
ax.text(-2.4, 7.0, "capacity=4 全滿，head=2，排隊順序是 A→B→C→D", fontsize=13)
row(ax, 4.9, ["C", "D", "A", "B"], 4, head=2, label="舊")
ax.text(-2.4, 3.9, "直接 realloc 成 8 格：舊資料原封不動放在前 4 格", fontsize=13)
row(ax, 1.6, ["C", "D", "A", "B", "", "", "", ""], 8, head=2, label="realloc 後",
    colors={4: BAD, 5: BAD})
ax.text(0.6, 0.75, "從 head=2 往右數 4 格：A、B、空、空 → C、D 不見了（順序斷在舊的尾巴）", fontsize=12, color=HEAD)
ax.text(0.6, 0.15, "那該怎麼搬？＝先猜 Q2，先別看答案", fontsize=12, weight="bold")
fig.savefig("ring3_resize_trap.png", dpi=110, bbox_inches="tight")

# 4 checklist
fig, ax = canvas(8.4, "④ 要寫哪些函式、要注意什麼")
items = [
    ("aqueue_create", "配置 struct＋一開始的格子（容量自己定，例如 4）"),
    ("aqueue_enqueue", "滿了先擴容（見③），再寫到 (head+size)%cap；成功回 0"),
    ("aqueue_dequeue", "空的回 -1；否則值經 *out 帶出，head 前進一格"),
    ("aqueue_peek", "跟 dequeue 一樣讀 data[head]，但不動 head／size"),
    ("aqueue_size", "直接回 size"),
    ("aqueue_destroy", "free(data) 再 free(q)；傳 NULL 進來要安全"),
]
y = 7.4
for name, desc in items:
    ax.add_patch(Rectangle((-2.4, y - 0.15), 4.1, 0.7, fc="#e3eeea", ec="none"))
    ax.text(-2.25, y + 0.2, name, fontsize=13, family="monospace", weight="bold", va="center")
    ax.text(1.9, y + 0.2, desc, fontsize=12, va="center")
    y -= 0.85
ax.text(-2.4, 2.1, "測試會專門抓的三件事", fontsize=14, weight="bold", color=HEAD)
ax.text(-2.4, 1.5, "1. 先塞 4、取 2、再塞到 50 → head 不在 0 時擴容，順序要對", fontsize=12)
ax.text(-2.4, 0.95, "2. 空 queue dequeue／peek 要回 -1，不能讀到垃圾值", fontsize=12)
ax.text(-2.4, 0.4, "3. make asan 零報告：擴容後舊格子要 free、不能讀已釋放的記憶體", fontsize=12)
fig.savefig("ring4_checklist.png", dpi=110, bbox_inches="tight")
print("ok")
