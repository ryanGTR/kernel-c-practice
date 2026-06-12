# Day 21 — Struct + Pointer + Linked List（Week C 收尾）

📅 預定 2026-05-03（你 2026-05-05 才開始做也沒關係）。詳細教學：[wiki/daily/2026-04/day-21.md](../../../linux_kernel/wiki/daily/2026-04/day-21.md)

主軸：把 Day 17-20 的 pointer / malloc / 字串技能整合進一個 student linked list，**Week C 結業作品**。

## 檔案對照表

| 檔案 | 對應步驟 | 你要做什麼 |
|------|----------|------------|
| `student.h` | 共用 header | 填 struct + function 宣告 |
| `student.c` | 共用實作 | 填所有 student / list 函式 |
| `step1.c` | Step 1 value student | 用 stack 上 struct + `&` |
| `step2.c` | Step 2 heap student | malloc/free，valgrind 0 leaks |
| `list_demo.c` | Step 3 ★ 主菜 linked list | 三節點 list、走訪、安全釋放 |

## 指令

```bash
cd ~/Documents/kernel-c-practice/c-basics/day-21

# 編譯全部
make

# 跑某一個
./step1
./step2
./list_demo

# valgrind 全部跑（步驟 2、3 必須 0 leaks）
make valgrind

# 清掉產出
make clean
```

## 通關條件

- [ ] `make` 零 warning（-Wall -Wextra）
- [ ] `step1` 印出 `Alice  92`（格式對）
- [ ] `step2` valgrind **0 leaks 0 errors**
- [ ] `list_demo` 印出三個學生 + valgrind **0 leaks 0 errors** ★
- [ ] 三檔都用 `sizeof(*ptr)` 慣用法（不寫 `sizeof(int)`）
- [ ] free 完置 NULL（Day 19 養的習慣）
- [ ] linked list destroy 用「先存 next，再 free cur」防 UAF

## 觀念整合對照

這支檔案幾乎用上 Week C 全部技能：

| 觀念 | Day | 用在哪 |
|------|-----|--------|
| function 拆檔 | Day 15 | student.h / student.c 分離 |
| Makefile 多目標 | Day 16 | 編 3 個 binary |
| pointer 傳遞 | Day 17 | `student_fill(&s, ...)` |
| malloc/free | Day 19 | `student_create` / `student_destroy` |
| sizeof(*ptr) | Day 19 | `malloc(sizeof(*s))` |
| 字串複製安全 | Day 20 | `student_fill` 裡 strncpy + 手動補 \0 |
| ownership | Day 19 | 誰 create、誰 destroy |
| UAF 預防 | Day 19 | list_destroy 先存 next |

## 挑戰題（做完三步驟後）

不看答案試試：

1. **list_len** — 回傳節點數
2. **list_find_max** — 回傳分數最高的節點 pointer（空 list 回 NULL）
3. **list_prepend(struct student_node \*\*head, ...)** — 把節點加到 list 頭，**注意 `**`**

第 3 題的 `**` 是「呼叫端的指標變數要被改」的進階用法（Day 17 swap 是 `*`，這裡是改 pointer 變數，所以 `**`）。寫得出來代表 pointer 真的通了。

## 與 kernel 的連結

你今天寫的是「struct 包 next pointer」風格。kernel 用的是**反過來**：`list_head` 內嵌在資料結構裡，靠 `container_of` macro 從 list 節點拿回原 struct。

```c
/* 你這份 */
struct student_node {
    struct student data;
    struct student_node *next;
};

/* kernel 風格 */
struct student {
    char name[32];
    int score;
    struct list_head list;   /* 內嵌 */
};
```

好處：同一個 struct 可以同時掛多條 list（多加幾個 `list_head` 欄位）。Week D 會接這條。

`kernel-c-practice/exercises/01-linked-list/` 是 kernel 風格版本，做完今天可以回頭重做那個。

## 完成後

在 Telegram 回：
```
Day 21 / Week C 結業報告。
三檔結構 valgrind: ___
挑戰題完成到第幾題: ___
Week C 最有感的一天: Day __
```
