# 13_kasan_uaf -- 用 KASAN 抓 use-after-free

## 目標

寫一個故意 **use-after-free（UAF，釋放後再用）** 的 module：`kmalloc` 一塊記憶體、
`kfree` 掉、然後故意再去存取它。載入後在 `dmesg` 看 **KASAN（Kernel Address SANitizer）**
報告，並讀懂它的 `Allocated by` / `Freed by` 兩段 stack。

Java 類比：這就像 ASan 之於 C，或像你在已被 GC 回收的物件上還握著一個失效 reference——
只是 kernel 沒有 GC，UAF 會默默讀到別人的資料或當機，KASAN 就是幫你當場抓包的那道 sanitizer。

## ⚠️ 安全

**只在 QEMU/VM 裡 insmod**（`wargame/qemu-sandbox/`）。這個 module 會踩壞 freed 記憶體。

## 需要的 CONFIG / 前置

- Kernel 需以 **`CONFIG_KASAN=y`** 重編（generic KASAN 即可）。
  - 通常還會連帶開 `CONFIG_KASAN_GENERIC=y`、`CONFIG_KASAN_INLINE=y`。
- 確認方式：`zcat /proc/config.gz | grep KASAN` 或 `grep KASAN /boot/config-$(uname -r)`。
- 先做完 Phase 4（會寫 module、會看 dmesg）。

## 步驟

1. 把 skeleton 的三個 TODO 補完（kmalloc → kfree → free 後故意存取）。
2. `make` 編出 `kasan_uaf.ko`。
3. 在 QEMU 裡 `insmod kasan_uaf.ko`。
4. `dmesg | tail -n 50` 看 KASAN 報告，重點看：
   - 標題那行：`BUG: KASAN: use-after-free in ...`（或 slab-use-after-free）。
   - **`Allocated by task ...`** 那段 stack：指向你 `kmalloc` 的位置。
   - **`Freed by task ...`** 那段 stack：指向你 `kfree` 的位置。
   - 出事位址相對 object 的 offset。
5. 對照這兩段 stack，確認它們分別指回你 code 裡的哪一行。

## 完成 checklist

- [ ] `make` 編譯成功，產生 `kasan_uaf.ko`，零 warning
- [ ] `scripts/checkpatch.pl -f kasan_uaf.c` 零警告
- [ ] kernel 確實是 `CONFIG_KASAN=y`（已驗證）
- [ ] 在 QEMU 裡 `insmod` 後，`dmesg` 出現 `BUG: KASAN: use-after-free`
- [ ] 能在報告裡指出 `Allocated by` / `Freed by` 兩段 stack 各對應你哪一行
- [ ] 能說出 KASAN 是怎麼知道這塊記憶體已經被 free 的（提示：shadow memory / redzone）

## 對應 wiki

- [[kernel-memory-checkers]] — `linux_kernel/wiki/kernel-memory-checkers.md`
