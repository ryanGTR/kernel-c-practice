# A3 — Malloc Lab（CS:APP 改版）

## 目標

自己實作 `malloc` / `free` / `realloc` / `mm_init`。不用標準 libc，從一塊固定 heap 區域自己管理。

這是 CMU CS:APP 第 9 章的經典 lab，做完對「記憶體是什麼」會有全新認識。

## 介面

```c
/* 初始化 heap（測試 harness 會呼叫一次） */
int mm_init(void);

/* 分配 size bytes，回傳對齊的 pointer（8-byte align） */
void *mm_malloc(size_t size);

/* 釋放之前 malloc 回傳的 pointer */
void mm_free(void *ptr);

/* 標準 realloc 語意 */
void *mm_realloc(void *ptr, size_t size);
```

你的 heap 底層用 `mem_sbrk(int incr)` 向系統要更多空間（harness 提供模擬版）。

## 實作難度階梯

從簡單到難，每階都要能跑過測試：

### Level 1 — Implicit Free List（隱式空閒鏈結）
- 每塊前後各一個 header/footer 記錄 size + alloc bit
- `malloc` 做 first-fit 線性搜尋
- `free` 做 coalescing（合併相鄰 free block）
- 預期 throughput 低、utilization 中等

### Level 2 — Explicit Free List（顯式空閒鏈結）
- 只把 free block 串成雙向鏈結（用 payload 區塞 prev/next pointer）
- `malloc` 只走 free list，速度大幅上升
- 預期 throughput 提升 5-10x

### Level 3 — Segregated Free List（分離空閒鏈結）
- 按 size class 分多個 free list（例：16, 32, 64, 128, ..., 4KB+）
- best-fit within class
- 預期達到 glibc ptmalloc 的 50-70% 效能

## 測試

你需要寫 `mdriver.c` 模擬 malloc/free 的 trace：

```c
/* trace file 格式
a <id> <size>    # malloc
f <id>           # free
r <id> <size>    # realloc
*/
```

寫 4-5 個 trace：
- `tiny.rep` — 10 次 malloc/free，驗證正確性
- `random.rep` — 隨機 size、隨機順序
- `realloc.rep` — 大量 realloc
- `coalesce.rep` — 故意製造 coalescing 情境

## 評分（自評）

| 指標 | 目標 |
|------|------|
| 正確性 | 100% trace 通過（所有 malloc 的 pointer 互不重疊、8-byte align） |
| Utilization | 60% 以上（實際用掉的記憶體 / 總 heap 消耗） |
| Throughput | 每秒 500k+ ops（參考用） |

## 工具

- `gdb` — block header 出錯時幾乎一定要用
- `valgrind` — 你的 mm_malloc 要能通過
- ASan 不能用（它會攔截 malloc，跟你打架）
- 自己寫 `mm_checkheap()`：走一遍 heap，驗證所有 block header/footer 一致、free list 正確

## 完成清單

- [ ] Level 1 implicit list 跑過 4 個 trace
- [ ] Level 2 explicit list throughput 提升
- [ ] Level 3 segregated list utilization 60%+
- [ ] `mm_checkheap` 能在任何時刻被呼叫
- [ ] 白紙畫得出 block header/footer 的 bit layout

## 學習重點

- 為什麼 kernel 的 `slab allocator` 長那樣（你自己踩過一次就懂了）
- Alignment 為什麼重要
- `container_of` pattern 的雛形（從 payload 回推 header）
- Fragmentation（internal vs external）的實際感受

## 參考

CS:APP 第 3 版第 9 章（Virtual Memory）最後一節「Dynamic Memory Allocation」有完整講解。CMU 原版 lab 在網路上找得到，可以直接用他們的 trace file 當測試。
