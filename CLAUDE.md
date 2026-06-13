# CLAUDE.md — kernel-c-practice

繼承 `~/.claude/CLAUDE.md` 的全域慣例（繁中、Java 類比、kernel coding style）。本檔補充本專案專屬資訊。

## 這個專案是什麼

用 Linux kernel coding style 練習寫 C。從 userspace 基礎一路到 kernel module，分成階段式練手專案 + 已附解答的 kernel 模式範例（`exercises/`）。

**重要：練手專案（`phase1`～`phase4`）是 Ryan 自己要寫的。** 除非他明確要求，不要直接幫他把 `.c` 答案寫出來——先給提示、講概念、指出 bug 方向。`exercises/` 才是含解答的參考區。

## 目錄結構

```
exercises/              ← Kernel 模式範例（含解答，參考用）
phase1-coreutils/       ← 重寫 Unix 工具（wc/cat/echo/grep/sort）
phase2-data-structures/ ← linked list / hash table / dynamic array
phase3-system-tools/    ← my_shell / my_hexdump（碰 syscall）
phase4-kernel-modules/  ← hello_module / proc_info（kernel space）
phase5-kernel-debugging/ ← 故意埋 bug，用 KASAN/kmemleak/kprobes/ftrace/kgdb 抓（13-17，QEMU 內跑）
c-basics/               ← 每日 C 基礎練習（day-NN）
leetcode/               ← C 解題，分 pointer/memory/kernel-idiom 三組
scripts/                ← check-c.sh（深度檢查）、checkpatch.pl
```

每個練手子專案內含 `spec.md`（規格 + 完成 checklist）、`Makefile`、`*.c`、測試資料。

## 常用指令

```bash
# 在子專案內
make            # 編譯，零 warning 才算過（-Wall -Wextra -Werror）
make test       # 跟系統工具比對
make valgrind   # 記憶體檢查，須零 error（Phase 1-3）

# 在 repo 根目錄
make all        # 編譯所有 userspace 專案
make check      # kernel style 檢查（checkpatch）

# 深度檢查單一檔案 / 目錄（5 層檢查 + scorecard）
./scripts/check-c.sh c-basics/day-18/basics.c
```

## 完成標準（每個練手專案）

- [ ] `make` 零 warning
- [ ] `make test` 通過
- [ ] `make valgrind` 零 error（Phase 1-3）
- [ ] `spec.md` 裡的 checklist 全打勾

## Coding Style

Linux kernel coding style：Tab=8 字元、80 欄、K&R brace、`scripts/checkpatch.pl` 把關。設定檔：`.clang-format`、`.editorconfig`。

## 搭配的 Wiki / 筆記

- 教學筆記：`~/Documents/linux_kernel/wiki/guides/c-basic-learn.md`
- 常見錯誤：`~/Documents/linux_kernel/wiki/guides/c-mistake-pitfall.md`
- 主教材：K&R（The C Programming Language 2nd Ed）

當練習中出現值得沉澱的概念或踩雷，可提議更新到 `linux_kernel/wiki/`（依該 wiki 的 SCHEMA.md 慣例）。
