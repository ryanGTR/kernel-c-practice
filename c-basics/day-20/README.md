# Day 20 — C 字串函式家族（strcpy → strncpy → strscpy）

📅 預定 2026-05-02。詳細教學：[wiki/daily/2026-04/day-20.md](../../../linux_kernel/wiki/daily/2026-04/day-20.md)

主軸：用契約分析法（Pre / Post / Fail / Owns）追三代字串函式的演化。

## 檔案對照表

| 檔案 | 對應步驟 | 你要做什麼 |
|------|----------|------------|
| `bad_strcpy.c`  | Step 1 strcpy 越界 | 用 strcpy 灌 29 bytes 進 8-byte buf，看 UB |
| `bad_strncpy.c` | Step 2 strncpy 兩坑 | 示範「沒補 \0」陷阱，再寫修正版對照 |
| `my_strscpy.c`  | Step 4 ★ 主菜 | 親手實作 strscpy（每行對到契約四欄）|
| `my_strlen.c`   | Step 4 加碼 | 從 0 走到 `\0`，不准呼叫 strlen |

## 指令

```bash
cd ~/Documents/kernel-c-practice/c-basics/day-20

# 編譯全部
make

# 跑某一個
./my_strscpy

# valgrind（bad_strcpy / bad_strncpy 會抓到 Invalid write / 讀越界）
make valgrind

# ASan 對 stack overflow 的訊號比 valgrind 更直接
make asan
./bad_strcpy_asan

# 清掉產出
make clean
```

## 通關條件

- [ ] `make` 零 warning（-Wall -Wextra）
- [ ] `bad_strcpy` 在 valgrind 或 ASan 看到 stack write 越界
- [ ] `bad_strncpy` 印出來的 dst 後段有亂碼（沒 \0）；修正版乾淨
- [ ] **`my_strscpy` 三組輸出全對**（短字串、截斷回 -E2BIG、空字串）
- [ ] `my_strlen` 4 組 case 都跟 `strlen` 一致（`OK`）
- [ ] my_strscpy 每行寫個 comment 註明它支援契約四欄哪一條

## 契約透鏡（Day 19 帶過來的肌肉）

每寫一個函式前先填這四欄：

| 欄 | 問什麼 |
|----|--------|
| **Pre**  | caller 必須保證什麼？（語言不會檢查的義務）|
| **Post** | 成功時什麼**一定**為真？|
| **Fail** | Pre 不滿足或 runtime 出錯時，會發生什麼？UB / 錯誤碼 / 靜默？|
| **Owns** | 誰配記憶體？誰釋放？誰借用？|

寫 code 不是從 0 想 corner case — 是把契約欄翻成 1-3 行 code。**Code 是契約的副產品**。

## 三代演化重點

| 代 | 契約問題 |
|----|----------|
| **strcpy**   | Fail 欄空白 → UB，buffer overflow 的元兇 |
| **strncpy**  | Pre 加 `n`，但 Post 變兩個爛條款（不補 \0、浪費填 0）|
| **strscpy**  | 四欄都寫滿，caller 用回傳值就能機械判斷 |

## 與 kernel 的連結

```bash
# 看 kernel 真實版本
grep -n 'ssize_t strscpy' /home/ryan/Documents/linux_kernel/lib/string.c

# kernel 明確禁用 strcpy / strncpy
grep -n 'strcpy\|strncpy' /home/ryan/Documents/linux_kernel/Documentation/process/deprecated.rst
```

你今天寫的版本是邏輯骨架；kernel 版本多了 word-by-word（一次讀 8 bytes）+ SWAR 偵測 `\0`。

## 收工白紙小考

不看任何資料，紙上寫：
1. `my_strscpy` 的契約四欄（Pre / Post / Fail / Owns）
2. minimal 實作（不用優化）

寫不出來的圈起來，diff 回這份 README。

## 完成後

在 Telegram 回：
```
Day 20 / C Day 6 完成。
my_strscpy 有沒有一次寫對: ___
最不直覺的邊界: ___
```
