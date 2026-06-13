# 14_kmemleak -- 用 kmemleak 抓記憶體洩漏

## 目標

寫一個故意**洩漏記憶體**的 module：在 init 裡 `kmalloc` 一塊、只存到區域變數就 `return`，
exit 也不 `kfree`。然後用 **kmemleak** 掃描，看到那塊 `unreferenced object`，並讀懂它的
alloc backtrace 指回你的 module。

Java 類比：kmemleak 像個保守式（conservative）的洩漏偵測器，概念上類似你用 heap dump 找
「沒人 reference 但沒被回收」的物件。差別是 kernel 沒有 GC——沒人指向它，它就永遠卡在那。
kmemleak 的判準正是「從已知指標走不到 = 洩漏」。

## ⚠️ 安全

**只在 QEMU/VM 裡 insmod**（`wargame/qemu-sandbox/`）。

## 需要的 CONFIG / 前置

- Kernel 需 **`CONFIG_DEBUG_KMEMLEAK=y`**。
- 需掛載 debugfs：`mount -t debugfs none /sys/kernel/debug`（多數發行版已掛）。
- 開機時 kmemleak 可能預設關閉；必要時加 kernel cmdline `kmemleak=on`。
- 確認檔案存在：`ls /sys/kernel/debug/kmemleak`。

## 步驟

1. 補完 init 的 TODO（kmalloc 到區域變數、不存全域、return）；exit 保持不 free。
2. `make` 編出 `kmemleak_demo.ko`。
3. 在 QEMU 裡 `insmod kmemleak_demo.ko`。
4. 觸發掃描（kmemleak 是延遲偵測，要主動掃）：
   ```
   echo scan > /sys/kernel/debug/kmemleak
   ```
   （可能要等幾秒、或掃兩次讓「最短存活時間」過關。）
5. 讀結果：
   ```
   cat /sys/kernel/debug/kmemleak
   ```
   找 `unreferenced object 0x...`，看它的 `backtrace`：應指回你的 `kmalloc`。
6. （選做）`echo clear > /sys/kernel/debug/kmemleak` 清掉已回報的，再掃一次比對。

## 完成 checklist

- [ ] `make` 編譯成功，產生 `kmemleak_demo.ko`，零 warning
- [ ] `scripts/checkpatch.pl -f kmemleak_demo.c` 零警告
- [ ] kernel 確實是 `CONFIG_DEBUG_KMEMLEAK=y`（已驗證）
- [ ] `insmod` 後 `echo scan` + `cat` 看到 `unreferenced object`
- [ ] backtrace 指回你 module 的 `kmalloc` 那行
- [ ] 能解釋：為什麼「存到全域指標」就不會被回報成洩漏

## 對應 wiki

- [[kernel-memory-checkers]] — `linux_kernel/wiki/kernel-memory-checkers.md`
