# Day 19 — 動態記憶體 + Valgrind 練習骨架

📅 預定 2026-05-01。詳細教學：[wiki/daily/2026-04/day-19.md](../../../linux_kernel/wiki/daily/2026-04/day-19.md)

## 檔案對照表

| 檔案 | 對應步驟 | 你要做什麼 |
|------|----------|------------|
| `basic.c` | Step 1 最小 malloc/free | 配 int、寫值、印、free |
| `arrays.c` | Step 2 配置陣列 + sizeof(*p) | 配 N 個 int、寫值、印、free |
| `calloc_demo.c` | Step 3 calloc vs malloc | 看未初始化 vs 全零的差別 |
| `strbuf.c` | Step 4 ★ 主菜 動態字串 | 實作 init/append/free 三件式 |
| `errors_leak.c` | Step 5a definitely lost | 故意不 free（不用改 code）|
| `errors_uaf.c` | Step 5b use-after-free | 拿掉註解觸發 UAF |
| `errors_dfree.c` | Step 5c double free | 拿掉註解觸發 double free |

## 指令

```bash
cd ~/Documents/kernel-c-practice/c-basics/day-19

# 編譯全部
make

# 跑某一個
./strbuf

# 編譯 + 全部跑 valgrind
make valgrind

# 跑單支 valgrind 看完整報告
valgrind --leak-check=full --show-leak-kinds=all ./strbuf

# 清掉產出
make clean
```

## 通關條件

- [ ] `make` 零 warning（-Wall -Wextra）
- [ ] `basic.c` valgrind 0 leaks 0 errors
- [ ] `arrays.c` valgrind 0 leaks 0 errors
- [ ] `calloc_demo.c` 看到 valgrind 抓 malloc 那邊的「uninitialised value」
- [ ] **`strbuf.c` valgrind 0 leaks 0 errors**（主菜通關線）
- [ ] `errors_leak.c` 看到 `definitely lost`
- [ ] `errors_uaf.c` 拿掉註解後看到 `Invalid read ... free'd`
- [ ] `errors_dfree.c` 拿掉註解後看到 `Invalid free` 或 glibc `double free or corruption`

## 看 valgrind 報告（4 區拆解）

valgrind 報告格式跟 ASan 不同，但結構類似：

### 區 1：錯誤類型 + 位置

```
==XXXX== Invalid read of size 4
==XXXX==    at 0x... main (file.c:LINE)
```

讀法：
- 錯誤類別（`Invalid read` / `Invalid free` / `definitely lost` / `Conditional jump on uninitialised value`）
- 大小（`size 4` = 一個 int）
- ★ **行號 = 你寫的 code 在哪出事**

### 區 2：記憶體狀態描述

```
==XXXX==  Address 0x... is 0 bytes inside a block of size N free'd
==XXXX==    at 0x... free
==XXXX==    by 0x... main (file.c:LINE-WHERE-FREED)
```

讀法：valgrind 告訴你那塊記憶體現在的狀態：
- `free'd` → 已被 free（典型 UAF 訊號）
- `not stack'd, malloc'd or recently free'd` → 完全野指標
- `alloc'd` → 在 malloc 過的範圍內（OOB read/write）

### 區 3：HEAP SUMMARY

```
==XXXX== HEAP SUMMARY:
==XXXX==     in use at exit: 40 bytes in 1 blocks
==XXXX==   total heap usage: 1 allocs, 0 frees, 40 bytes allocated
```

讀法：總共配了多少、釋了多少。**`in use at exit` 的就是 leak**。

### 區 4：LEAK SUMMARY（leak 分類）

```
==XXXX== LEAK SUMMARY:
==XXXX==    definitely lost: 40 bytes in 1 blocks   ← ★ 這個是真 bug
==XXXX==    indirectly lost: 0 bytes in 0 blocks    ← 因為別人 leak 連帶找不到
==XXXX==      possibly lost: 0 bytes in 0 blocks    ← valgrind 不確定
==XXXX==    still reachable: 0 bytes in 0 blocks    ← 還有 pointer 但沒 free
==XXXX==         suppressed: 0 bytes in 0 blocks
```

**通常只看 `definitely lost`**。其他幾類經常是 library / runtime 自己的，不是你的問題。

### 結尾

```
==XXXX== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

`0 errors` = 通關。

## valgrind vs ASan（Day 18 用的）

|              | valgrind        | ASan (Day 18)        |
|--------------|-----------------|----------------------|
| 機制         | 跑時用 VM 監視  | 編譯時插檢查碼       |
| 速度         | 慢 10-50 倍     | 快（2-3 倍 overhead）|
| 抓 stack 越界| ❌ 看不到        | ✅ 強                 |
| 抓 heap 越界 | ✅ 強            | ✅ 強                 |
| 抓 leak      | ✅ 強            | 🟡 LSan 才行          |
| 抓未初始化   | ✅ 強            | 🟡 MSan 才行          |
| 改編譯       | ❌ 不用          | ✅ 要重編             |

**Day 19 主場是 valgrind**（因為這次重點 = leak / UAF / double free / uninitialised，全是 valgrind 拿手）。
Day 18 主場是 ASan（因為那次是 stack 越界）。

工具沒有「最強」，只有「對的場景」。

## 完成後

在 Telegram 回：
```
Day 19 / C Day 5 完成。
strbuf valgrind clean: yes/no
三個錯誤裡最難抓的是: ___（通常是 UAF 或 double free）
```

---

## 🔄 待辦（2026-05-05 留下，之後回來收尾）

> Code review 後的 punch list。主菜 strbuf.c 已完美過關（valgrind 0 leaks/0 errors，且超過 spec），剩下的是 polish + 沒走完的步驟。優先做第 1 項。

### 1. ❗ errors_uaf.c / errors_dfree.c — 拿掉註解跑 valgrind（最重要）

兩個檔案內的 TODO 註解還在：
- `errors_uaf.c` 第 41-44 行
- `errors_dfree.c` 第 43-46 行

不拿掉就不會觸發 UAF / double free，valgrind 報不出來。通關條件最後兩條沒滿足（要看到 `Invalid read ... free'd` / `Invalid free`）。

5 分鐘做完。**這個不只練 code，是練「讀 valgrind 報告」的技能**。

### 2. arrays.c — 輸出格式 + 收尾

目前輸出 `0  ,1  ,4  ,...,81 `，預期 `0, 1, 4, ..., 81\n`。

問題在 separator 順序：
```c
/* 現在的 (錯) */
printf("%d ", arr[i]);
if (i != n - 1)
    printf(" ,");

/* 慣用寫法 (對) */
for (size_t i = 0; i < n; i++) {
    if (i > 0)
        printf(", ");
    printf("%d", arr[i]);
}
printf("\n");
```

順便修：
- 第 37 行 `return -1` → `return 1`（main 慣例）
- 第 55 行 `(void)n;` 多餘（n 已被用過），刪掉
- 加 `free(arr); arr = NULL;` 對齊 basic.c 的習慣

### 3. basic.c — 輸出 label

目前 `42 `（多空格、沒 prefix），預期 `*p = 42`。

```c
printf("*p = %d\n", *p);
```

小事但 spec 寫了就照做，肌肉記憶要養。

### 4. calloc_demo.c — gcc warning 處理

`make clean && make` 會看到：
```
calloc_demo.c:42: warning: '*a' may be used uninitialized
```

跟「make 零 warning」通關條件衝突，但 spec 故意要讀 uninit 給 valgrind 抓。

解法：
```c
volatile int probe = a[0];   /* volatile 阻止 gcc 靜態分析優化 */
printf("malloc[0] = %d (垃圾值)\n", probe);
```

gcc 不報 warning，valgrind 還是會抓 `Conditional jump or move depends on uninitialised value(s)`——兩邊都滿足。

### 完成判定

- `make clean && make` 零 warning
- `make valgrind` 只有 errors_leak / errors_uaf / errors_dfree 三支「故意」噴錯，其它全綠

預估時間：30 分鐘以內全收尾。
