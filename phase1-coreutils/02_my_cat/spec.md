# 02_my_cat -- 重新實作 cat

## 目標

重新實作 GNU coreutils 的 `cat` 指令。讀取檔案並印到 stdout，支援 `-n` 行號功能。

## 行為規格

| 情境 | 行為 |
|------|------|
| 無引數 | 從 stdin 讀取，逐行印到 stdout |
| 單一檔案 | 讀取檔案內容，印到 stdout |
| 多個檔案 | 依序串接所有檔案內容 |
| `-n` 旗標 | 每行前面加上行號（靠右對齊，寬度 6，後接 tab） |
| 檔案不存在 | 印錯誤到 stderr，繼續處理其餘檔案 |
| `-` 引數 | 代表 stdin |

### 行號格式（`-n`）

```
     1	First line
     2	Second line
     3	
     4	Fourth line after blank
```

行號寬度 6，靠右對齊，後面接一個 tab 字元。

## 練習技能

- `FILE*` 檔案 I/O（`fopen`、`fgets` 或 `fgetc`、`fclose`）
- `argv` 旗標解析（手動解析 `-n`）
- 行計數邏輯
- stdin 偵測（無引數時從 stdin 讀取）

## 工具

- `gcc -Wall -Wextra -Werror` 編譯
- `make` 建置
- `valgrind --leak-check=full` 記憶體檢查
- `diff <(./my_cat test.txt) <(cat test.txt)` 與系統 cat 比對
- `diff <(./my_cat -n test.txt) <(cat -n test.txt)` 比對行號模式
- `man 3 fgets`

## 完成清單

- [ ] `make` 零警告編譯通過
- [ ] `./my_cat test.txt` 輸出與 `cat test.txt` 一致
- [ ] `./my_cat -n test.txt` 輸出與 `cat -n test.txt` 一致
- [ ] `./my_cat < test.txt` (stdin) 正確運作
- [ ] 多檔案串接正確
- [ ] `valgrind` 零錯誤、零洩漏
- [ ] 原始碼少於 80 行

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. 先處理一個 FILE* 的函式，再用迴圈呼叫多次
2. `-n` 解析：掃描 argv，遇到 `-n` 設旗標，其餘當檔名
3. `fgets` 讀一行比 `fgetc` 方便，但要注意緩衝區大小
4. 行號格式：`"%6d\t"` 即可
5. 記得處理「無引數 = stdin」和「`-` = stdin」兩種情況

</details>
