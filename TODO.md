# Kernel C Practice — 進度表

> 真實進度只認「親手寫完 + 過 `-Wall -Wextra -Werror` + 過 valgrind + 對照真實輸出」。
> 骨架/spec 不算通關。斷 session 先看這份。

最後更新：2026-06-28

## Phase 1 — Coreutils（重寫 Unix 工具）
- [x] 01_my_wc      ✅ 已通關（2026-06-28）
- [ ] 02_my_cat     ← **目前這關**
- [x] 03_my_echo    ✅ 已通關
- [ ] 04_my_grep
- [ ] 05_my_sort

## Phase 2 — 資料結構（手動 malloc/free）
- [ ] 06_linked_list
- [ ] 07_hash_table
- [ ] 08_dynamic_array

## Phase 3 — 系統工具（碰 syscall）
- [ ] 09_my_shell
- [ ] 10_my_hexdump

## Phase 4 — Kernel modules（進 kernel space）
- [ ] 11_hello_module
- [ ] 12_proc_info

## Phase 5 — Kernel 除錯（QEMU/VM 內）
- [ ] 13_kasan_uaf
- [ ] 14_kmemleak
- [ ] 15_kprobe_trace
- [ ] 16_ftrace_lab
- [ ] 17_panic_notifier

---

## 旁支練習（已碰過的基礎）
- c-basics/day-20：字串安全（bad_strcpy / bad_strncpy / **my_strscpy 自刻**）——對應 kernel 移除 strncpy() 的主題
- c-basics/day-21：把 strscpy 安全寫法套進 struct
- leetcode/set1-pointer-array：02_my_strcpy、03_my_strscpy
- wargame：lv0 ~ lv4（kernel module / oops 除錯）

## 通關規矩（每關）
1. `make` 零警告（`-Wall -Wextra -Werror`）
2. ~~valgrind 零 error、零 leak~~ → 本機 valgrind 跑不起來（Arch 把 ld.so strip、debuginfod 缺 bleeding-edge glibc 符號）。改用 `make asan`（ASan/LSan/UBSan）代驗，零報告即過。
3. 與系統工具 `diff` 比對輸出一致
4. 過了就 commit + push（remote: ryanGTR），並回來勾這份 TODO

## 心得 / 踩雷
- 01_my_wc：`while (ch = getc(fp) != EOF)` 少括號 → `!=` 優先序高於 `=`，ch 只拿到 0/1（Java 因 `==` 回傳 boolean 不會中招）。正解 `while ((ch = getc(fp)) != EOF)`。
- 01_my_wc：GNU wc 欄寬是**動態**的（= 最大數字 bytes 的位數），不是固定寬度；無檔名時結尾不留空格。
- 通用：本機 valgrind 無法用，記憶體一律走 `make asan`（見各專案 Makefile 的 asan target）。
