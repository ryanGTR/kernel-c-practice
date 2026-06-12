# 01_my_wc -- 重新實作 wc

## 目標

重新實作 GNU coreutils 的 `wc` 指令。計算行數、字數、位元組數。

## 行為規格

| 情境 | 行為 |
|------|------|
| 無引數 | 從 stdin 讀取，印出行數、字數、位元組數 |
| 單一檔案 | 印出行數、字數、位元組數 + 檔名 |
| 多個檔案 | 每個檔案一行 + 最後一行 `total` |
| 檔案不存在 | 印錯誤到 stderr，繼續處理其餘檔案，exit code 非零 |
| 欄位對齊 | 數字靠右對齊，欄寬 8 |

### 定義

- **行 (line)**: 以 `'\n'` 結尾的序列，每遇到一個 `'\n'` 計數 +1
- **字 (word)**: 被空白字元分隔的非空白序列（空白 = `isspace()` 為真）
- **位元組 (byte)**: 原始位元組數

### 輸出格式

```
       5      17     113 test.txt
```

每個欄位寬度 8，靠右對齊，以空格分隔，最後接檔名。

## 練習技能

- `argc` / `argv` 命令列引數處理
- `FILE*` 檔案操作 (`fopen`, `fclose`)
- `fgetc` 逐字元讀取
- `perror` 錯誤處理
- `printf` 格式化輸出（欄寬對齊）

## 工具

- `gcc -Wall -Wextra -Werror` 編譯
- `make` 建置
- `valgrind --leak-check=full` 記憶體檢查
- `diff <(./my_wc test.txt) <(wc test.txt)` 與系統 wc 比對
- `man 3 fopen`、`man 3 fgetc`、`man 3 isspace`

## 完成清單

- [ ] `make` 零警告編譯通過
- [ ] `./my_wc test.txt` 輸出與 `wc test.txt` 一致
- [ ] `./my_wc < test.txt` (stdin) 輸出與 `wc` 一致
- [ ] 多檔案模式有 total 行
- [ ] 不存在的檔案印錯誤到 stderr
- [ ] `valgrind` 零錯誤、零洩漏
- [ ] 原始碼少於 100 行

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. 用一個函式處理單一 FILE*，回傳 lines/words/bytes 結構
2. 判斷「在字中」或「不在字中」的狀態機來計算字數
3. stdin 就是不帶檔名的特殊情況，邏輯可以共用
4. `%8d` 可以處理靠右對齊
5. 記得 `fclose` 每個開啟的檔案（但不要 fclose stdin）

</details>
