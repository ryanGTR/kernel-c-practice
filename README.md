# Kernel C Practice

用 Linux kernel coding style 練習寫 C。從 userspace 基礎到 kernel module，12 個階段式專案 + 5 個 kernel 模式範例。

## 環境需求

- GCC (`sudo apt install gcc`)
- Make (`sudo apt install make`)
- Valgrind (`sudo apt install valgrind`)
- Perl (`sudo apt install perl`) — for checkpatch
- Linux kernel headers (`sudo apt install linux-headers-$(uname -r)`) — Phase 4 需要

## 專案結構

```
kernel-c-practice/
├── exercises/              ← Kernel 模式範例（含解答，參考用）
│   ├── 01-linked-list
│   ├── 02-container-of
│   ├── 03-error-handling
│   ├── 04-syscall-trace
│   └── 05-context-switch
│
├── phase1-coreutils/       ← 重寫 Unix 工具（你來寫）
│   ├── 01_my_wc
│   ├── 02_my_cat
│   ├── 03_my_echo
│   ├── 04_my_grep
│   └── 05_my_sort
│
├── phase2-data-structures/ ← 資料結構從頭寫
│   ├── 06_linked_list
│   ├── 07_hash_table
│   └── 08_dynamic_array
│
├── phase3-system-tools/    ← 系統程式（碰 syscall）
│   ├── 09_my_shell
│   └── 10_my_hexdump
│
└── phase4-kernel-modules/  ← 進入 kernel space
    ├── 11_hello_module
    └── 12_proc_info
```

## 使用方式

```bash
# 進入某個專案
cd phase1-coreutils/01_my_wc

# 看規格
cat spec.md

# 寫 code
nvim my_wc.c

# 編譯（零 warning 才算過）
make

# 跑測試（跟系統工具比對）
make test

# 記憶體檢查（必須零 error）
make valgrind

# Kernel style 檢查（全部 exercises）
make check
```

## 練手專案路線圖

### Phase 1：重寫 Coreutils（K&R Ch 4-5）

| # | 專案 | 難度 | 練到什麼 |
|---|------|------|----------|
| 01 | my_wc | ★☆☆ | argc/argv, FILE*, printf 格式化 |
| 02 | my_cat | ★☆☆ | 檔案 I/O, -n flag 解析 |
| 03 | my_echo | ★☆☆ | 最簡單的暖身，argv 迭代 |
| 04 | my_grep | ★★☆ | 字串搜尋, 多 flag (-i/-n/-c), 多檔案 |
| 05 | my_sort | ★★★ | malloc/realloc/free, qsort + 函數指標 |

🔧 工具：gcc -Wall, make, valgrind, diff, man

### Phase 2：資料結構（K&R Ch 5-6）

| # | 專案 | 難度 | 練到什麼 |
|---|------|------|----------|
| 06 | linked_list | ★★☆ | 指標操作, malloc/free 配對 |
| 07 | hash_table | ★★★ | hash 函數, chaining, strdup 記憶體管理 |
| 08 | dynamic_array | ★★☆ | realloc 成長策略, capacity vs size |

🔧 新增工具：gdb, -fsanitize=address, assert()

### Phase 3：系統工具（K&R Ch 8）

| # | 專案 | 難度 | 練到什麼 |
|---|------|------|----------|
| 09 | my_shell | ★★★ | fork/exec/waitpid/pipe, 4 階段漸進 |
| 10 | my_hexdump | ★★☆ | byte-level I/O, hex 格式化 |

🔧 新增工具：strace, man 2 (syscall man pages)

### Phase 4：Kernel Modules

| # | 專案 | 難度 | 練到什麼 |
|---|------|------|----------|
| 11 | hello_module | ★☆☆ | module_init/exit, pr_info, module_param |
| 12 | proc_info | ★★★ | proc_create, proc_ops, atomic, copy_to_user |

🔧 新增工具：insmod/rmmod, dmesg, checkpatch.pl, make C=1

## Kernel 模式範例（exercises/）

已含解答，直接跑看效果，再讀 code 理解：

| # | 主題 | 學到什麼 |
|---|------|----------|
| 01 | linked-list | `list_head`, `list_add`, `list_for_each_entry`, `container_of` |
| 02 | container-of | 結構體嵌入模式、從 member 回推 parent |
| 03 | error-handling | `goto` cleanup pattern、負數 errno 回傳 |
| 04 | syscall-trace | syscall 追蹤 |
| 05 | context-switch | context switch 概念 |

```bash
# 編譯全部範例
make all

# 跑全部範例測試
make test

# 檢查 coding style
make check
```

## 完成標準（每個專案）

- [ ] `make` 零 warning（-Wall -Wextra -Werror）
- [ ] `make test` 通過
- [ ] `make valgrind` 零 error（Phase 1-3）
- [ ] 讀 spec.md 裡的完成 checklist 全打勾

## Coding Style

遵循 Linux kernel coding style：
- Tab = 8 字元（不用空格）
- 80 column line limit
- K&R brace style
- `scripts/checkpatch.pl` 檢查

## 搭配閱讀

- **K&R**（The C Programming Language 2nd Ed）— 主教材
- **kernel-c-practice 教學筆記** — `~/Documents/linux_kernel/wiki/guides/c-basic-learn.md`
- **常見錯誤** — `~/Documents/linux_kernel/wiki/guides/c-mistake-pitfall.md`
