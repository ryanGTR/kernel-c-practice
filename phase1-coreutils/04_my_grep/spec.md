# 04_my_grep -- 重新實作簡化版 grep

## 目標

實作簡化版的 `grep`，搜尋固定字串（非正規表達式）。支援多檔案、行號、大小寫忽略、計數模式。

## 行為規格

| 情境 | 行為 |
|------|------|
| 基本用法 | `my_grep PATTERN FILE` -- 印出包含 PATTERN 的行 |
| 無檔案引數 | 從 stdin 讀取 |
| 多個檔案 | 每行前面加 `檔名:` 前綴 |
| `-i` 旗標 | 大小寫不敏感比對 |
| `-n` 旗標 | 印出行號（`檔名:行號:` 或 `行號:`） |
| `-c` 旗標 | 只印出匹配行數（`檔名:數字` 或 `數字`） |
| 無匹配 | exit code 為 1 |
| 檔案不存在 | 印錯誤到 stderr，繼續處理其餘檔案 |

### 輸出格式範例

```
$ ./my_grep fox test.txt
The quick brown fox jumps over the lazy dog.

$ ./my_grep -n fox test.txt
1:The quick brown fox jumps over the lazy dog.

$ ./my_grep -i FOX test.txt test2.txt
test.txt:The quick brown fox jumps over the lazy dog.

$ ./my_grep -c fox test.txt
1
```

## 練習技能

- 字串比對（`strstr`、`strcasestr`）
- `argv` 選項解析（多旗標組合）
- 多檔案處理邏輯
- exit code 語意

## 工具

- `gcc -Wall -Wextra -Werror` 編譯
- `make` 建置
- `valgrind --leak-check=full` 記憶體檢查
- `diff` 與系統 grep 比對（使用 `grep -F` 做固定字串模式）
- `man 3 strstr`、`man 3 strcasestr`

## 完成清單

- [ ] `make` 零警告編譯通過
- [ ] 基本搜尋與 `grep -F` 輸出一致
- [ ] `-i` 大小寫忽略正確
- [ ] `-n` 行號正確
- [ ] `-c` 計數正確
- [ ] 多檔案有檔名前綴
- [ ] 無匹配時 exit code 為 1
- [ ] `valgrind` 零錯誤、零洩漏
- [ ] 原始碼少於 120 行

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. 先把旗標和檔案引數分開：掃一遍 argv，`-` 開頭的是旗標，第一個非旗標是 pattern，其餘是檔名
2. `strcasestr` 是 GNU 擴充，需要 `#define _GNU_SOURCE` 在檔案最頂端
3. 多檔案時才加檔名前綴：用 `file_count > 1` 判斷
4. `fgets` 讀的行尾會包含 `'\n'`，比對時不需要去掉
5. 記得移除 `fgets` 讀到的換行符再印出，或者直接用 `fputs` 保留原始格式
6. exit code：用一個變數追蹤是否有任何匹配

</details>
