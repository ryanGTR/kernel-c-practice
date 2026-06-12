# Day 18 — 陣列 vs Pointer 練習骨架

📅 預定 2026-04-30，但你進度超前，隨時可以開工。

詳細教學：[wiki/daily/2026-04/day-18.md](../../../linux_kernel/wiki/daily/2026-04/day-18.md)

## 檔案對照表

| 檔案 | 對應步驟 | 你要做什麼 |
|------|----------|------------|
| `basics.c` | Step 1 陣列 = 連續記憶體 | 照 wiki 步驟 1 填空 |
| `sizeof_trap.c` | Step 2 sizeof 陷阱 | 照 wiki 步驟 2 填空 |
| `sum.c` | Step 3 正確簽名 + ARRAY_SIZE | 照 wiki 步驟 3 填空 |
| `max_of.c` | 🎯 今日挑戰 | **不看 sum.c 自己寫** |
| `oob.c` | Step 4 越界 + valgrind | 照 wiki 步驟 4 填空 |

## 指令

```bash
cd ~/Documents/kernel-c-practice/c-basics/day-18

# 編譯全部
make

# 跑某一個
./sum

# 編譯 + 全部跑 valgrind
make valgrind

# oob 用 AddressSanitizer 跑（valgrind 抓不到 stack 越界）
make asan

# 清掉產出
make clean
```

## 通關條件

- [ ] `make` 零 warning（-Wall -Wextra）
- [ ] `basics.c` 看出「位址等差 4」
- [ ] `sizeof_trap.c` 看到 outside=20 / inside=8
- [ ] `sum.c` 答案 = 31
- [ ] **`max_of.c` 自己寫出來**（戒律 #2：寫得出來 > 看得懂）
- [ ] `oob.c` 被 ASan 抓到 stack-buffer-overflow（valgrind 對 stack 越界看不到，要用 `make asan`）

## 看 ASan 報告（oob.c 必看）

`make asan` 會噴一大坨輸出。**新手只要會看 4 區就夠**：

### 區 1：頭部 — 錯誤類型 + 操作

```
==XXXXX==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x...
READ of size 4 at 0x... thread T0
```

讀法：
- `stack-buffer-overflow` ← 錯誤類別（stack 上的越界，不是 heap）
- `READ of size 4` ← 動作 + 寬度（讀 4 bytes = 一個 int）
- `thread T0` ← 主執行緒

⚠️ ASan 抓到第一個錯就 abort，所以 oob.c 兩個越界（`arr[5]` 讀、`arr[10] = 99` 寫）只會看到先發生的那個（READ）。要看 WRITE，得把 READ 那行註解掉再跑一次。

### 區 2：Stack trace — 哪一行

```
#0 0x... in main /.../oob.c:33   ← ★ 你寫的 code 出事的行
#1 in __libc_start_call_main      ← libc 框架
#2 in __libc_start_main_impl      ← libc 框架
#3 in _start                      ← runtime 入口
```

**只看 #0**。剩下三行是 main() 被誰 call 的歷史，標準 C 程式都長這樣，跳過。

`oob.c:33` 對應到：
```c
printf("Damn it: %d\n", arr[5]);
```

### 區 3：Object info — 越界的是誰（最重要）

```
Address 0x... is located in stack of thread T0 at offset 52 in frame
  #0 in main /.../oob.c:29

This frame has 1 object(s):
  [32, 52) 'arr' (line 30) <== Memory access at offset 52 overflows this variable
```

讀法：
- `stack of thread T0` — 在 stack 上（不是 heap）
- `at offset 52` — 你踩的位址在 main frame 裡 byte 52
- `[32, 52) 'arr' (line 30)` — `arr` 的合法範圍是 byte 32 到 51（**半開區間**，52 不在裡面）
- 32 到 52 = 20 bytes = 5 個 int → 對應 `int arr[5]`
- 你讀的位址 = 52 = 緊鄰 `arr` 之後的第一個 byte，也就是 `arr[5]`，正好越界一格

📌 翻譯成人話：
> 你宣告了 `arr[5]`（合法 index 0~4），但你存取了 `arr[5]` 這個不存在的格子。

### 區 4：Shadow memory（影子記憶體圖）

```
=>0x...: f1 f1 f1 f1 00 00 [04] f3 f3 f3 f3 f3 ...
         └ 前禁區 ┘ └arr┘ └你踩這┘└── 後禁區 ──┘
```

ASan 在每塊真記憶體旁邊存一張「影子地圖」，標記哪可以碰：

| 標記 | 意思 |
|------|------|
| `f1` | stack 左紅區（buffer 前面的禁區） |
| `00` | 可碰（每個 shadow byte = 8 真 bytes，所以 20 bytes 的 arr 顯示為「兩個 00 + 一個 04」） |
| `[04]` | 第 5 個 byte 開始不可碰 ← **你 `arr[5]` 踩這** |
| `f3` | stack 右紅區（arr 之後到 main frame 邊界） |

💡 入門訣竅：90% 的時候只看 **#0 行號** + **`[start, end) 'name' overflows`** 那行就夠了，shadow map 是進階 debug 才用。

### Telegram 回報答案範本

```
AddressSanitizer: stack-buffer-overflow at oob.c:33
[32, 52) 'arr' overflows — arr[5] 越界一個 int (4 bytes)
```

---

## 完成後

在 Telegram 回：
```
Day 18 / C Day 4 完成。
sizeof 內外差 = ___
valgrind 抓到越界長什麼樣: ___
```
