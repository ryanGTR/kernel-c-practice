# 🛡️ QEMU 沙盒 ── 打 LV1+ 之前的必經關卡

狀態：🔒（通關 LV0.9 後處理）

## 為什麼需要這關

LV0 ~ LV0.9 都是「乖模組」，弄不壞東西，本機跑沒事。
**LV1 起的模組會故意讓 kernel 出包**——deadlock、race、NULL deref、panic。在你的 Framework 本機跑，輕則噴 oops、重則整台機器 hang 到只能硬關機（沒存的東西就掰了）。

所以從這裡開始，module 一律丟進 **QEMU 虛擬機**跑：崩的是 VM，你的本機毫髮無傷，重開 VM 即可。

**Java 類比**：就像你不會把還在 debug、可能無限迴圈或 OOM 的程式直接跑在正式機上——你開一個隔離的測試 JVM / 容器。QEMU 就是 kernel 的那個隔離沙盒。

## 兩條路線

### 路線 A：virtme-ng（推薦先用，最快）── ✅ 已裝好並驗證（2026-06-01）

`virtme-ng` 能用「你現有的 kernel」瞬間開一個 QEMU VM，共用你的檔案系統，幾秒就進去，適合快速測模組。

```bash
# 安裝：在官方 extra 庫（不是 AUR），直接 pacman 即可
sudo pacman -S virtme-ng        # 已裝 1.41

# ── 互動玩法（打關時用這個）──────────────
cd ~/Documents/kernel-c-practice/wargame/lv0
vng -r                  # -r 不帶參數 = 開「跟主機同款」的 kernel；會進到 VM 的 shell
#   在 VM 裡：make / insmod hello.ko / dmesg / rmmod hello
#   打 Ctrl+D 離開 → VM 蒸發，本機不受影響

# ── 一次性跑指令（驗證 / 腳本用）─────────
vng -r --quiet --exec "insmod /abs/path/xxx.ko; dmesg | grep TAG; rmmod xxx"
```

> ⚠️ 一定要加 `-r`：不加的話 vng 會把「當前目錄」當成 kernel 源碼樹去找 bzImage 而報錯。
> ✅ 已驗證：KVM 可用（`/dev/kvm` 可讀寫）、開機 ~1.5s、VM 內 `uname -r` = `7.0.10-arch1-1`、insmod/rmmod hello.ko 成功、dmesg 見得到 `LV0` 訊息。

> 限制：virtme-ng 預設用你**現有的 kernel**，而你現在的 Arch kernel **不一定開了 lockdep / DEBUG_ATOMIC_SLEEP**（LV1 抓 bug 要靠這些）。先確認：
> ```bash
> zcat /proc/config.gz | grep -E 'DEBUG_ATOMIC_SLEEP|PROVE_LOCKING|DEBUG_SPINLOCK'
> ```
> 若沒有 `=y`，要嘛走路線 B 自己編 debug kernel，要嘛裝 `linux-hardened` / 自訂 config 的 kernel。

### 路線 B：自編 debug kernel ── ✅ 已備好，一鍵可建（virtme-ng 版）

LV1~LV4 需要「開滿除錯選項」的 kernel，所有 BUG/lockdep/oops 才會完整噴出。
**好消息**：virtme-ng 的 `-f`（套 config 片段）+ `-b`（編譯）省掉了手刻 initramfs、寫 qemu 指令的苦工。config 片段我已備好：`debug.config`（同目錄）。

#### Step 1 — 編 debug kernel（一次性，約 20~40 分）

你的 `~/Documents/linux_kernel/` 本身就是 kernel 7.0 源碼樹，直接在那編：
```bash
cd ~/Documents/linux_kernel
vng -f ~/Documents/kernel-c-practice/wargame/qemu-sandbox/debug.config -b
```
`-f` 把 `debug.config` 的選項合併進 base config，`-b` 編譯。編好的 kernel 在 `./arch/x86/boot/bzImage`。

#### Step 2 — 跑這顆 debug kernel

```bash
cd ~/Documents/linux_kernel
vng -r .        # -r 帶「目錄」= 跑這個樹裡編好的 kernel（不是主機 kernel）
```

#### ⚠️ 關鍵雷：模組要對著 debug 樹編，不是對 `/lib/modules`

debug kernel 的 vermagic（lockdep/preempt 等旗標）跟你**已安裝**的 7.0 不同，拿平常 `make` 出來的 `.ko` 去 insmod 會被擋（vermagic mismatch）。打 LV1+ 時，模組改成對 debug 樹編：
```bash
cd <某關目錄>
make -C ~/Documents/linux_kernel M=$PWD modules
# 然後在 vng -r . 開的 VM 裡 insmod 這顆
```

> 想加 gdb 驗屍（LV4）：開 VM 時加 `--gdb`，另一個終端機 `gdb vmlinux` 接上去下中斷點。
> KASAN 讓編譯/開機變慢；想快一點把 `debug.config` 裡 KASAN 兩行註解掉。

## 過關條件

能在 QEMU 裡：
1. 開機進到 shell
2. `insmod` 一個你寫的模組（拿 LV0 的 `hello.ko` 來試）
3. `dmesg` 看到它的訊息
4. 故意 `insmod` 一個會 panic 的模組，**VM 崩了但你的本機沒事**

做到第 4 點 = 沙盒驗證成功，正式解鎖 LV1。

## 解鎖

通關 → LV1（spinlock 持鎖睡眠）。

---
打到這關時跟我說，我幫你選路線並逐步帶完整流程。
