# 17_panic_notifier -- 自訂 panic notifier + 觸發 soft lockup

## 目標

寫一個 module：

1. 註冊一個**自訂 panic notifier**——掛到 `panic_notifier_list`，讓 kernel panic 時跑到你的
   callback 印一行（之後可在 panic log / oops 裡看到自己留的痕跡）。
2. 提供一個 `lockup=1` 選項：故意 `while(1)` 不讓出 CPU，**觸發 soft lockup detector**。

Java 類比：panic notifier 像 JVM 的 shutdown hook（`Runtime.addShutdownHook`）或
`UncaughtExceptionHandler`——在「整個世界要結束」的最後一刻，跑你註冊的收尾邏輯。差別是這裡是
整個 kernel 要 panic，而且 callback 跑在極受限的 context（不能睡、要極簡）。

## 🛑🛑 安全（這條最重要）

- **只在 QEMU / VM 裡 insmod，絕對不要在主機（Framework 16）跑。**
- `lockup=1` 會讓一顆 CPU 永遠卡死；soft lockup detector 會在 dmesg 連續尖叫，嚴重時整機沒反應。
- panic 路徑可能直接把整台機器帶走，你會被迫硬關機、可能掉資料。
- 沙箱在 `wargame/qemu-sandbox/`：崩了重開 VM 就好，這正是它存在的理由。

## 需要的 CONFIG / 前置

- soft lockup detector 需 **`CONFIG_SOFTLOCKUP_DETECTOR=y`**（多數發行版有開）。
- 確認門檻：`cat /proc/sys/kernel/watchdog_thresh`（預設約 10 秒，超過才報）。
- 要看到 panic notifier 真的跑，得**真的 panic** 一次（例如在 QEMU 裡 `echo c > /proc/sysrq-trigger`，
  需 `CONFIG_MAGIC_SYSRQ=y` 且 sysrq 開啟）。
- 先做完 Phase 4。

## 步驟

1. 補完 skeleton：`module_param(lockup)`、`my_panic_handler`、`nb.notifier_call`、
   init 的 `atomic_notifier_chain_register`、exit 的 `..._unregister`。
2. 先**不要**填 lockup 那段、或先 `insmod ... lockup=0`，驗證 notifier 註冊沒問題。
3. `make` 編出 `panic_notifier.ko`。
4. **在 QEMU 裡**：
   - 測 panic notifier：`insmod panic_notifier.ko` → 觸發一次 panic
     （`echo c > /proc/sysrq-trigger`）→ 在 panic log 找到你 handler 印的那行。
   - 測 soft lockup（補完 lockup TODO 後）：`insmod panic_notifier.ko lockup=1`
     → `dmesg` 出現 `BUG: soft lockup - CPU#N stuck for Ns!`。
5. 把 soft lockup / panic log 拿去對照 wiki，練習讀懂每一段（呼叫堆疊、暫存器、tainted flags）。

## 完成 checklist

- [ ] `make` 編譯成功，產生 `panic_notifier.ko`，零 warning
- [ ] `scripts/checkpatch.pl -f panic_notifier.c` 零警告
- [ ] 全程在 QEMU/VM 操作（**沒有**在主機 insmod）
- [ ] `insmod`（lockup=0）成功註冊 notifier，`rmmod` 乾淨卸載
- [ ] 觸發一次 panic，panic log 出現你 handler 印的那行
- [ ] `lockup=1` 觸發 `BUG: soft lockup - CPU#N stuck`
- [ ] 能說出 soft lockup 與 hard lockup 的差別（提示：中斷有沒有被關）
- [ ] 能對照 wiki 讀懂 soft lockup / panic 報告的主要欄位

## 對應 wiki

- [[kernel-panic-lockup-hang]] — `linux_kernel/wiki/kernel-panic-lockup-hang.md`
- [[oops-decoding]] — `linux_kernel/wiki/oops-decoding.md`
