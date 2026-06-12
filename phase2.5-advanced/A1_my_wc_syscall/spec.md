# A1 — my_wc syscall 版（加約束）

## 目標

重新實作 `my_wc`，但**禁用 `stdio.h` 全家**。只能用 POSIX syscall：`open`、`read`、`write`、`close`、`lseek`。

這題逼你看清：libc 的 `fgetc` / `printf` 底下到底做了什麼。

## 硬約束（沒達到就不算過）

| 約束 | 原因 |
|------|------|
| 不准 `#include <stdio.h>` | 只能用 syscall |
| buffer 固定 4096 bytes | 不能一次讀整個檔 |
| 要正確處理「word 跨越 buffer 邊界」 | 這是真的系統程式設計會碰到的問題 |
| 支援 stdin（fd 0） | 跟 Phase 1 版一樣 |
| 不准 `malloc` | stack + 固定 buffer 就夠了 |
| 自己寫 `putuint`（印十進位整數） | `printf` 不准用 |

## 擴充題（加分）

- [ ] 加 `-c` / `-w` / `-l` flag（只印某欄）
- [ ] 支援 UTF-8：多算一個 `chars` 欄位（不是 byte，是 Unicode code point）
- [ ] 用 `mmap` 版本做比較，量 `read` vs `mmap` 的效能差異

## 練習技能

- POSIX syscall（`man 2 read`、`man 2 write`）
- `errno` 直接處理（沒有 `perror` 可用，你要自己 `strerror` 或寫 `write(2, ...)`）
- Buffer 狀態機（處理 partial word 跨讀取邊界）
- 手刻整數格式化

## 工具

- `strace ./my_wc_sys test.txt` — 看你的程式打了哪些 syscall，該只有 open/read/close/write
- `time` — 跟 Phase 1 版比效能
- `diff <(./my_wc_sys test.txt) <(wc test.txt)` — 結果比對

## 完成清單

- [ ] `grep -c '#include' my_wc_sys.c` 輸出的 header 不含 stdio.h
- [ ] `strace` 只看到 syscall（無 fread/fwrite 類 libc 符號）
- [ ] 各種 buffer 邊界情況測試（word 剛好切在 4096 邊界、檔案只有空白、空檔案）
- [ ] 大檔案（>100MB）仍正確
- [ ] `valgrind` 乾淨
- [ ] 零 warning（參考 README 的 CFLAGS）

---

<details>
<summary>提示</summary>

1. 用 `enum { IN_WORD, OUT_WORD }` 當狀態機，跨 buffer 時狀態要保留
2. `putuint` 可以遞迴，也可以先填 stack buffer 再反轉
3. 錯誤訊息：`const char msg[] = "error\n"; write(2, msg, sizeof msg - 1);`
4. `read()` 回傳 0 = EOF；回傳 -1 看 errno（可能被 signal 打斷，要 retry EINTR）
5. 為什麼 stdio 要存在？做完這題你就懂了

</details>
