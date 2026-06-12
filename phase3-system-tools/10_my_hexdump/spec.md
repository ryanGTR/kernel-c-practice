# 10_my_hexdump -- 重新實作 hexdump -C

## 目標

重新實作 `hexdump -C` 的簡化版。讀取檔案（或 stdin），以十六進位格式顯示每個位元組，旁邊附上可列印的 ASCII 字元。

## 行為規格

| 情境 | 行為 |
|------|------|
| 單一檔案 | 讀取檔案內容，以 hex + ASCII 格式印出 |
| 無引數 | 從 stdin 讀取 |
| 檔案不存在 | 印錯誤到 stderr，exit code 非零 |

### 輸出格式

每行 16 個位元組：偏移量（hex）| 前 8 bytes hex | 後 8 bytes hex | ASCII 可列印字元

```
00000000  48 65 6c 6c 6f 2c 20 77  6f 72 6c 64 21 0a 00 00  |Hello, world!...|
```

- 偏移量：8 位十六進位，靠右補零
- Hex 欄位：每 byte 用兩位十六進位表示，前後各 8 bytes 之間多一個空格
- ASCII 欄位：可列印字元原樣顯示，不可列印字元用 `.` 代替
- 最後一行如果不足 16 bytes，hex 部分用空格補齊

### 最後一行

檔案結尾印出總位元組數：

```
00000010
```

## 練習技能

- `fread()` — 以固定大小讀取位元組
- 十六進位格式化（`%02x`）
- `isprint()` — 判斷是否為可列印字元
- 偏移量追蹤
- 處理最後不完整的一行

## 工具

- `gcc -Wall -Wextra -Werror` 編譯
- `make` 建置
- `valgrind --leak-check=full` 記憶體檢查
- `diff <(./my_hexdump test.bin) <(hexdump -C test.bin)` 與系統 hexdump 比對
- `man 3 fread`、`man 3 isprint`

## 完成清單

- [ ] `make` 零警告編譯通過
- [ ] `./my_hexdump test.bin` 輸出與 `hexdump -C test.bin` 一致
- [ ] `./my_hexdump < test.bin` (stdin) 輸出一致
- [ ] 最後不完整行格式正確（hex 空格補齊、ASCII 長度正確）
- [ ] 檔案不存在印錯誤到 stderr
- [ ] `valgrind` 零錯誤、零洩漏
- [ ] 原始碼少於 100 行

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. 每次 `fread` 讀 16 bytes，回傳實際讀取數量
2. 偏移量每行 +16，用 `%08x` 格式化
3. hex 部分：迴圈印 16 次，第 8 個之後多印一個空格。不足 16 的用 `   `（三個空格）補
4. ASCII 部分：迴圈印實際讀到的 bytes，`isprint()` 判斷，不可列印印 `.`
5. 最後印一行只有偏移量（總 bytes 數）

</details>
