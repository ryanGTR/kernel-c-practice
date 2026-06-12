# 09_my_shell -- 建一個迷你 shell

## 目標

實作一個最小化的互動式 shell。能讀取指令、fork 子程序執行、等待結束。

## 行為規格（分階段）

### Stage 1：基本執行

| 情境 | 行為 |
|------|------|
| 輸入 `ls` | fork + execvp 執行，印出結果 |
| 輸入 `pwd` | 同上 |
| 指令不存在 | 印錯誤訊息到 stderr |
| EOF (Ctrl-D) | 結束 shell |

### Stage 2：帶引數

| 情境 | 行為 |
|------|------|
| `ls -la /tmp` | 正確切割引數，傳給 execvp |
| 多餘空白 | `  ls   -la  ` 應等同 `ls -la` |

### Stage 3：內建指令

| 情境 | 行為 |
|------|------|
| `cd /tmp` | 用 `chdir()` 切換目錄 |
| `cd` (無引數) | 切到 `$HOME` |
| `exit` | 正常結束 shell，exit code 0 |

### Stage 4：管道（bonus，可跳過）

| 情境 | 行為 |
|------|------|
| `ls \| wc` | 建立 pipe，fork 兩個子程序，用 dup2 接起來 |

## 練習技能

- `fork()` — 建立子程序
- `execvp()` — 執行外部程式（自動搜尋 PATH）
- `waitpid()` — 等待子程序結束
- `pipe()` / `dup2()` — 管道與檔案描述符重導向
- `strtok()` — 字串切割（用空白分割指令與引數）
- 訊號基礎概念（Ctrl-C 不該殺掉你的 shell）

## 工具

- `gcc -Wall -Wextra -Werror` 編譯
- `make` 建置
- `valgrind --leak-check=full` 記憶體檢查
- `strace -f ./my_shell` — 追蹤你的 shell 的系統呼叫，和 bash 比對
- `man 2 fork`、`man 2 execvp`、`man 2 waitpid`、`man 2 pipe`、`man 2 dup2`

## 完成清單

- [ ] `make` 零警告編譯通過
- [ ] 能執行基本指令（`ls`、`pwd`、`echo hello`）
- [ ] 帶引數的指令正確（`ls -la /tmp`）
- [ ] `cd` 能切換目錄，`cd` 無引數回到 `$HOME`
- [ ] `exit` 正常結束
- [ ] 不存在的指令印錯誤到 stderr
- [ ] `strace -f ./my_shell` 顯示正確的 fork/execvp/waitpid 系統呼叫

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. Stage 1 只需要 ~40 行：main loop 印提示 → `fgets` 讀一行 → `strtok` 切開 → `fork` → 子程序 `execvp` → 父程序 `waitpid`
2. 不要嘗試解析複雜語法，用 `strtok` 以空白切就夠了
3. `cd` 和 `exit` 必須在 fork 之前處理（它們是 shell 內建指令，不是外部程式）
4. `execvp` 的第一個引數是程式名，第二個引數是 `argv[]` 陣列（以 NULL 結尾）
5. 管道需要兩次 fork：左邊寫入 pipe，右邊從 pipe 讀取

</details>
