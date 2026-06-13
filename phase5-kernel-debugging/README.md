# Phase 5：Kernel 除錯實戰

進到這一階段，重點不再是「寫出功能」，而是**故意把東西搞壞，再用對的工具抓出來、看懂報告**。
每個 lab 都是同一個循環：

> 埋 bug（或操作 tracefs）→ 用對應工具觸發/抓取 → 讀懂 dmesg / trace 報告 → 對照 wiki 頁理解原理

對 Java 工程師的類比：這就像在 JVM 裡學會看 `NullPointerException` 的 stack trace、用
async-profiler 抓火焰圖、用 `-verbose:gc` 看記憶體洩漏——只是這裡的工具是 KASAN、kmemleak、
kprobes、ftrace，舞台是 kernel space，崩掉的是整台機器。

## Lab 一覽

| #  | 名稱            | 練哪把工具                          | 對應 wiki 頁 |
|----|-----------------|-------------------------------------|--------------|
| 13 | kasan_uaf       | KASAN（use-after-free 偵測）        | `linux_kernel/wiki/kernel-memory-checkers.md` |
| 14 | kmemleak        | kmemleak（記憶體洩漏掃描）          | `linux_kernel/wiki/kernel-memory-checkers.md` |
| 15 | kprobe_trace    | kprobes（動態攔截 kernel 函式）     | `linux_kernel/wiki/kprobes.md` |
| 16 | ftrace_lab      | ftrace / tracefs / trace-cmd（純操作，無 module） | `linux_kernel/wiki/ftrace.md` |
| 17 | panic_notifier  | panic notifier、soft lockup detector | `linux_kernel/wiki/kernel-panic-lockup-hang.md`、`linux_kernel/wiki/oops-decoding.md` |

## ⚠️ 安全警語（務必先讀）

這一階段的 module **會故意搞壞記憶體、觸發 lockup、甚至讓 kernel panic**。

- **絕對不要在你的主機（Framework 16 本機）`insmod` 這些 module。**
- 一律在 **QEMU / VM 裡跑**——用 `wargame/qemu-sandbox/` 那套沙箱環境。
- Lab 17 的 soft lockup 會卡死整顆 CPU；在主機跑可能直接讓你被迫硬關機。
- 觀念：這些 bug 在沙箱裡崩掉沒差，重開 VM 就好；在主機崩掉你會掉資料。

把它想成：你不會在 production 跑 `kill -9` 壓測，只會在隔離環境做。QEMU 就是你的隔離環境。

## 完成標準（每個 lab）

- [ ] 能在 QEMU/VM 裡 `insmod` 觸發（lab 16 是 tracefs 操作）
- [ ] 在 `dmesg` / `trace` 看到**預期的報告**（KASAN 報告、unreferenced object、kprobe log…）
- [ ] 能對照對應的 wiki 頁，**讀懂報告每一段在講什麼**
- [ ] `spec.md` 裡的完成 checklist 全打勾

## 前置

- **先做完 Phase 4**（`11_hello_module`、`12_proc_info`）——你要會寫、會 `insmod`/`rmmod`、會看 `dmesg`。
- 大部分 lab 需要**重編帶特定 CONFIG 的 kernel**（KASAN、kmemleak…），各 lab 的 `spec.md` 會註明要開哪些。
- QEMU 沙箱：`wargame/qemu-sandbox/`。

## 各 lab 內含

| 檔案 | 說明 |
|------|------|
| `spec.md`   | 目標、需要的 CONFIG / 前置、步驟、完成 checklist、對應 wiki |
| `Makefile`  | phase4 同款 kbuild（lab 16 為 no-op 提示） |
| `*.c`       | 帶 TODO 的 skeleton——**bug 與 handler 內容要你自己填**，這是練習 |

> skeleton 故意只給骨架。把「故意埋的 bug」「handler 要印什麼」當成練習自己補完，
> 不要去找完整解答——抓 bug 的肌肉記憶是這階段的重點。
