# 05_my_sort -- 重新實作簡化版 sort

## 目標

實作簡化版的 `sort`，讀取所有行、排序、輸出。這是 Phase 1 最難的專案，需要動態記憶體管理。

## 行為規格

| 情境 | 行為 |
|------|------|
| 基本用法 | `my_sort [FILE]` -- 讀取所有行，依字母順序排序後印出 |
| 無引數 | 從 stdin 讀取 |
| `-r` 旗標 | 反向排序（Z 到 A） |
| `-n` 旗標 | 依數值排序（非字母順序） |
| 檔案不存在 | 印錯誤到 stderr，exit code 非零 |

### 排序規則

- **預設（字母排序）**：使用 `strcmp` 比較
- **`-n`（數值排序）**：每行開頭的數字部分用數值比較，無數字開頭視為 0
- **`-r`（反向）**：反轉排序方向，可與 `-n` 組合

### 範例

```
$ cat test.txt
banana
apple
cherry
$ ./my_sort test.txt
apple
banana
cherry
$ ./my_sort -r test.txt
cherry
banana
apple
```

## 練習技能

- 動態記憶體管理（`malloc`、`realloc`、`free`）
- `qsort` 搭配函式指標
- `getline` 或手動行讀取
- 記憶體洩漏防範（這題最容易漏 `free`）

## 工具

- `gcc -Wall -Wextra -Werror` 編譯
- `make` 建置
- `valgrind --leak-check=full` 記憶體檢查（**這題必須通過，很容易洩漏**）
- `diff <(./my_sort test.txt) <(sort test.txt)` 與系統 sort 比對
- `man 3 qsort`、`man 3 getline`、`man 3 strtol`

## 完成清單

- [ ] `make` 零警告編譯通過
- [ ] `./my_sort test.txt` 輸出與 `sort test.txt` 一致
- [ ] `./my_sort -r test.txt` 輸出與 `sort -r test.txt` 一致
- [ ] `./my_sort -n test.txt` 輸出與 `sort -n test.txt` 一致
- [ ] `./my_sort < test.txt` (stdin) 正確運作
- [ ] `valgrind` **零錯誤、零洩漏**（重點！）
- [ ] 原始碼少於 150 行

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. 資料結構：`char **lines` 陣列，用 `realloc` 動態成長（每次 ×2）
2. `getline(&line, &len, fp)` 會自動 malloc，記得每行都要 `free`
3. `qsort(lines, count, sizeof(char *), compare_func)` -- compare 函式收到的是 `const void *`，要轉成 `const char **`
4. 反向排序：在 compare 函式裡把回傳值取負
5. 數值排序：用 `strtol` 取得行首的數字
6. **清理流程**：先 `free` 每個 `lines[i]`，再 `free(lines)` 本身
7. `getline` 讀的行包含 `'\n'`，排序時這沒問題，但要注意最後一行可能沒有 `'\n'`

</details>
