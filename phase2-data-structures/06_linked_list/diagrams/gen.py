#!/usr/bin/env python3
"""Generate step-by-step diagrams for 06_linked_list (SVG -> PNG via rsvg-convert)."""
import subprocess
from pathlib import Path

OUT = Path(__file__).parent
W, H = 960, 440
NW, NH = 120, 54          # node box: [data | next]
FONT = "Noto Sans CJK TC"
MONO = "Noto Sans Mono"
C = dict(bg="#ffffff", ink="#1f2937", mute="#9ca3af", red="#dc2626",
         blue="#2563eb", green="#059669", orange="#d97706", new="#fef3c7",
         freed="#f3f4f6", hit="#dcfce7")
VARC = {"head": C["blue"], "list": C["blue"], "cur": C["green"],
        "prev": C["orange"], "new": C["red"]}


def col(i):
    return 60 + i * 190


def svg_frame(title, code, note, nodes, edges, vars_):
    """nodes: id -> dict(v, x, y, style); edges: (src, dst, color, curve);
    dst may be ('NULL', x, y). vars_: (name, node_id)."""
    s = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" '
         f'viewBox="0 0 {W} {H}">',
         '<defs>']
    for k, v in C.items():
        s.append(f'<marker id="a-{k}" viewBox="0 0 10 10" refX="9" refY="5" '
                 f'markerWidth="8" markerHeight="8" orient="auto-start-reverse">'
                 f'<path d="M0,0 L10,5 L0,10 z" fill="{v}"/></marker>')
    s.append('</defs>')
    s.append(f'<rect width="{W}" height="{H}" fill="{C["bg"]}"/>')
    s.append(f'<text x="24" y="38" font-family="{FONT}" font-size="22" '
             f'font-weight="bold" fill="{C["ink"]}">{title}</text>')
    s.append(f'<rect x="24" y="52" width="{W-48}" height="34" rx="6" '
             f'fill="#111827"/>')
    s.append(f'<text x="36" y="75" font-family="{MONO}" font-size="16" '
             f'fill="#f9fafb">{code}</text>')

    # edges first (under nodes)
    for src, dst, color, curve in edges:
        n = nodes[src]
        x1, y1 = n["x"] + NW * 0.75, n["y"] + NH / 2
        if isinstance(dst, tuple):
            x2, y2 = dst[1], dst[2] + NH / 2
        else:
            d = nodes[dst]
            if d["y"] > n["y"] + 5:        # target below: enter from top
                x2, y2 = d["x"] + NW / 2, d["y"]
            elif d["y"] < n["y"] - 5:
                x2, y2 = d["x"] + NW / 2, d["y"] + NH
            else:
                x2, y2 = d["x"], d["y"] + NH / 2
        w = 3.5 if color == "red" else 2.2
        if curve:
            cy = min(y1, y2) - curve
            path = f'M{x1},{y1} C{x1+40},{cy} {x2-40},{cy} {x2},{y2}'
        else:
            path = f'M{x1},{y1} L{x2},{y2}'
        s.append(f'<path d="{path}" fill="none" stroke="{C[color]}" '
                 f'stroke-width="{w}" marker-end="url(#a-{color})"/>')

    for e in edges:
        dst = e[1]
        if isinstance(dst, tuple):
            s.append(f'<text x="{dst[1]+6}" y="{dst[2]+NH/2+6}" '
                     f'font-family="{MONO}" font-size="17" '
                     f'fill="{C["mute"]}">NULL</text>')

    for nid, n in nodes.items():
        fill = {"new": C["new"], "freed": C["freed"],
                "hit": C["hit"]}.get(n.get("style"), "#ffffff")
        dash = ' stroke-dasharray="6 4"' if n.get("style") == "freed" else ""
        ink = C["mute"] if n.get("style") == "freed" else C["ink"]
        x, y = n["x"], n["y"]
        s.append(f'<rect x="{x}" y="{y}" width="{NW}" height="{NH}" rx="6" '
                 f'fill="{fill}" stroke="{ink}" stroke-width="2"{dash}/>')
        s.append(f'<line x1="{x+NW/2}" y1="{y}" x2="{x+NW/2}" y2="{y+NH}" '
                 f'stroke="{ink}" stroke-width="1.5"{dash}/>')
        s.append(f'<text x="{x+NW/4}" y="{y+NH/2+7}" text-anchor="middle" '
                 f'font-family="{MONO}" font-size="20" font-weight="bold" '
                 f'fill="{ink}">{n["v"]}</text>')
        s.append(f'<circle cx="{x+NW*0.75}" cy="{y+NH/2}" r="4" '
                 f'fill="{ink}"/>')
        s.append(f'<text x="{x+NW/4}" y="{y-6}" text-anchor="middle" '
                 f'font-family="{MONO}" font-size="11" fill="{C["mute"]}">'
                 f'data</text>')
        s.append(f'<text x="{x+NW*0.75}" y="{y-6}" text-anchor="middle" '
                 f'font-family="{MONO}" font-size="11" fill="{C["mute"]}">'
                 f'next</text>')
        if n.get("style") == "freed":
            s.append(f'<text x="{x+NW/2}" y="{y+NH+22}" text-anchor="middle" '
                     f'font-family="{FONT}" font-size="14" fill="{C["red"]}">'
                     f'已 free（別再碰）</text>')

    # pointer variables: stacked under their node
    stack = {}
    for name, nid in vars_:
        n = nodes[nid]
        k = stack.get(nid, 0)
        stack[nid] = k + 1
        cx = n["x"] + NW / 4
        top = n["y"] + NH + 8
        ly = n["y"] + NH + 58 + k * 30
        color = VARC.get(name, C["ink"])
        if k == 0:
            s.append(f'<path d="M{cx},{ly-20} L{cx},{top}" stroke="{color}" '
                     f'stroke-width="2.5" marker-end="url(#a-ink)"/>')
        s.append(f'<text x="{cx}" y="{ly}" text-anchor="middle" '
                 f'font-family="{MONO}" font-size="18" font-weight="bold" '
                 f'fill="{color}">{name}</text>')

    for i, line in enumerate(note.split("\n")):
        s.append(f'<text x="24" y="{H-50+i*26}" font-family="{FONT}" '
                 f'font-size="17" fill="{C["ink"]}">{line}</text>')
    s.append('</svg>')
    return "\n".join(s)


def row(vals, y=130, styles=None):
    styles = styles or {}
    return {str(v): dict(v=v, x=col(i), y=y, style=styles.get(v))
            for i, v in enumerate(vals)}


def chain(vals, colors=None, null=True):
    colors = colors or {}
    e = [(str(a), str(b), colors.get(a, "ink"), 0)
         for a, b in zip(vals, vals[1:])]
    if null:
        last = vals[-1]
        e.append((str(last), ("NULL", col(len(vals) - 1) + NW + 30, 130),
                  colors.get(last, "mute"), 0))
    return e


FRAMES = []


def frame(name, *a):
    FRAMES.append((name, svg_frame(*a)))


# ---------- list_append(head=10->20, 30) ----------
n = row([10, 20]); n["30"] = dict(v=30, x=col(2), y=225, style="new")
frame("01-append-1", "list_append：① 先做出新節點",
      "new = malloc(...); new->data = 30; new->next = NULL;",
      "新節點（黃色）還沒接上任何人，它自己的 next 是 NULL。\n問：如果 head 是 NULL，函式在這裡就 return new——為什麼？",
      n, chain([10, 20], null=False) +
      [("30", ("NULL", col(2) + NW + 30, 225), "mute", 0),
       ("20", ("NULL", col(1) + NW + 30, 130), "mute", 0)],
      [("head", "10"), ("new", "30")])
frame("02-append-2", "list_append：② cur 從 head 出發",
      "cur = head;",
      "cur 只是「手指」：指到 10，不複製任何東西（Java 類比：Node cur = head）。",
      n, chain([10, 20], null=False) +
      [("30", ("NULL", col(2) + NW + 30, 225), "mute", 0),
       ("20", ("NULL", col(1) + NW + 30, 130), "mute", 0)],
      [("head", "10"), ("cur", "10"), ("new", "30")])
frame("03-append-3", "list_append：③ 走到最後一個節點",
      "while (cur->next != NULL) cur = cur->next;",
      "停在 20：因為 20->next == NULL。注意條件是 cur->next，不是 cur——\n要停在「最後一個」才能改它的 next。",
      n, chain([10, 20], null=False) +
      [("30", ("NULL", col(2) + NW + 30, 225), "mute", 0),
       ("20", ("NULL", col(1) + NW + 30, 130), "mute", 0)],
      [("head", "10"), ("cur", "20"), ("new", "30")])
n2 = row([10, 20, 30], styles={30: "new"})
frame("04-append-4", "list_append：④ 改一條箭頭接上",
      "cur->next = new;   return head;",
      "紅色＝這一步改的唯一一條箭頭。head 沒變，所以 return 原本的 head。",
      n2, chain([10, 20, 30], colors={20: "red"}),
      [("head", "10"), ("cur", "20"), ("new", "30")])

# ---------- list_prepend(10->20->30, 5) ----------
n = row([5, 10, 20, 30], styles={5: "new"})
frame("05-prepend-1", "list_prepend：① 新節點指向舊 head",
      "cur = malloc(...); cur->data = 5; cur->next = head;",
      "5 先指向 10（紅）。這時 list 變數還是指著 10——外面的人還看不到 5。",
      n, chain([5, 10, 20, 30], colors={5: "red"}),
      [("head", "10"), ("cur", "5")])
n2 = row([5, 10, 20, 30], styles={5: "new"})
frame("06-prepend-2", "list_prepend：② 回傳新的頭",
      "return cur;          /* main: list = list_prepend(list, 5); */",
      "頭換人了，所以函式一定要回傳新頭，而且呼叫端要接住。\n問：main 裡如果只寫 list_prepend(list, 5); 沒有 list = ，會怎樣？",
      n2, chain([5, 10, 20, 30]), [("list", "5")])

# ---------- list_find(5->10->20->30, 20) ----------
for i, (at, msg) in enumerate([
        ("5", "5 != 20，往下一個走。"),
        ("10", "10 != 20，繼續。"),
        ("20", "找到了：回傳 cur（節點的位址），不是 20 這個數字。\n找 99 的話 cur 會一路走到 NULL，迴圈結束 return NULL。")]):
    n = row([5, 10, 20, 30], styles={20: "hit"} if at == "20" else None)
    frame(f"{7+i:02d}-find-{i+1}", f"list_find(list, 20)：第 {i+1} 圈",
          "while (cur != NULL) { if (cur->data == value) return cur; cur = cur->next; }",
          msg, n, chain([5, 10, 20, 30]), [("head", "5"), ("cur", at)])

# ---------- list_delete(5->10->20->30, 20) ----------
n = row([5, 10, 20, 30])
frame("10-delete-1", "list_delete(list, 20)：① 兩根手指",
      "prev = head; cur = head->next;",
      "prev 永遠在 cur 的前一格。單向串列回不了頭，\n所以要刪 cur，必須有人抓著它前面那個節點。",
      n, chain([5, 10, 20, 30]),
      [("head", "5"), ("prev", "5"), ("cur", "10")])
frame("11-delete-2", "list_delete(list, 20)：② 一起往前走",
      "prev = cur; cur = cur->next;      /* 10 != 20 */",
      "兩根手指同步前進一格。現在 cur->data == 20，中了。",
      row([5, 10, 20, 30], styles={20: "hit"}), chain([5, 10, 20, 30]),
      [("head", "5"), ("prev", "10"), ("cur", "20")])
frame("12-delete-3", "list_delete(list, 20)：③ 繞過它",
      "prev->next = cur->next;",
      "紅色：10 直接指到 30。20 的 next 還指著 30（灰線），但已經沒有人能從 head 走到 20。\n順序很重要：先接好，再 free。",
      row([5, 10, 20, 30], styles={20: "hit"}),
      [("5", "10", "ink", 0), ("20", "30", "mute", 0),
       ("10", "30", "red", 70),
       ("30", ("NULL", col(3) + NW + 30, 130), "mute", 0)],
      [("head", "5"), ("prev", "10"), ("cur", "20")])
frame("13-delete-4", "list_delete(list, 20)：④ 釋放",
      "free(cur);   return head;",
      "如果先 free(cur) 再做 prev->next = cur->next，就是 use-after-free（make asan 會抓）。\n結果：5 → 10 → 30 → NULL",
      row([5, 10, 20, 30], styles={20: "freed"}),
      [("5", "10", "ink", 0), ("10", "30", "ink", 70),
       ("30", ("NULL", col(3) + NW + 30, 130), "mute", 0)],
      [("head", "5"), ("prev", "10")])

# ---------- challenge: delete the head ----------
n = row([5, 10, 30], styles={5: "hit"})
frame("14-challenge-delete-head", "挑戰（先猜再跑）：list_delete(list, 5)",
      "prev = head; cur = head->next;   /* 然後呢？ */",
      "照著你現在的程式，在紙上一步一步畫：5 這個節點有被比對到嗎？函式回傳什麼？\n先寫下預測，再把 main.c 裡「delete head」那個 TODO 補成 assert 跑跑看。",
      n, chain([5, 10, 30]), [("head", "5"), ("prev", "5"), ("cur", "10")])

for name, svg in FRAMES:
    p = OUT / f"{name}.svg"
    p.write_text(svg)
    subprocess.run(["rsvg-convert", "-z", "1.5", "-o",
                    str(OUT / f"{name}.png"), str(p)], check=True)
    p.unlink()
print(f"{len(FRAMES)} frames")
