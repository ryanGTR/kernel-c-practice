# 15_kprobe_trace -- 用 kprobe 攔截 kernel 函式

## 目標

寫一個 **kprobe** module，動態攔截某個 kernel 函式（建議 syscall 進入點，如
`__x64_sys_openat` 或 `do_sys_openat2`），在它每次被呼叫時印一行 log（並嘗試印出參數）。

Java 類比：kprobe 就像 JVM 的 bytecode instrumentation / agent——不改原始碼、不重編，
在 runtime 動態插一段 hook 進去攔截方法呼叫。差別是 kprobe 直接改 kernel 的指令流。

## ⚠️ 安全

學習階段建議在 QEMU/VM 跑（`wargame/qemu-sandbox/`）。pre_handler 寫錯（例如亂解
`pt_regs`、或在 handler 裡睡眠）可能搞掛 kernel。

## 需要的 CONFIG / 前置

- Kernel 需 **`CONFIG_KPROBES=y`**（多數發行版預設有開）。
- **`CONFIG_KALLSYMS=y`**：要能查符號。
- 確認你要 probe 的符號名稱**真的存在於這顆 kernel**：
  ```
  grep -E 'sys_openat|do_sys_openat' /proc/kallsyms
  ```
  若 skeleton 預設的 `__x64_sys_openat` 查不到，改成你查到的名字。
- 先做完 Phase 4。

## 步驟

1. 用 `/proc/kallsyms` 確認/修正 `kp.symbol_name`。
2. 補完 `handler_pre`（印 symbol 名 + `current->comm`，進階再從 `regs` 取參數）。
3. 補完 init（接 `pre_handler` → `register_kprobe` → 檢查回傳值）與 exit（`unregister_kprobe`）。
4. `make` 編出 `kprobe_trace.ko`。
5. 在 QEMU 裡 `insmod kprobe_trace.ko`。
6. 開另一個視窗 `dmesg -w`，然後隨便跑會開檔的指令（`ls`、`cat`），看 log 跳出來。
7. `rmmod kprobe_trace` 確認乾淨卸載（再跑指令不應再有 log）。

## 完成 checklist

- [ ] `make` 編譯成功，產生 `kprobe_trace.ko`，零 warning
- [ ] `scripts/checkpatch.pl -f kprobe_trace.c` 零警告
- [ ] 已用 `/proc/kallsyms` 驗證 symbol_name 正確
- [ ] `insmod` 成功（`register_kprobe` 回傳 0）
- [ ] 跑開檔指令時，`dmesg` 出現 pre_handler 的 log
- [ ] （進階）成功從 `pt_regs` 取出至少一個參數印出
- [ ] `rmmod` 後 probe 確實移除（不再有 log）

## 對應 wiki

- [[kprobes]] — `linux_kernel/wiki/kprobes.md`
