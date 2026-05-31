# Day 27 — Segfault + Core Dump（GDB Week Day 6）

📅 GDB Week Day 6 / 7。詳細教學：[wiki GDB-userspace §Day 27](../../../linux_kernel/wiki/tools/GDB-userspace.md) ・ [wiki/daily day-27](../../../linux_kernel/wiki/daily/2026-04/day-27.md)

主軸：故意寫一個 NULL deref，讓它 crash 出 **core dump**，再用 GDB 從「屍體」反推是哪一行、哪個變數出事。最後練 `gdb -p` attach 到正在跑的 process。

## 檔案

| 檔案 | 用途 |
|------|------|
| `segfault.c` | 故意 NULL deref；`crash()` 拆成獨立函式,讓 `bt` 看得到呼叫鏈 |
| `loop.c` | 無窮迴圈,給 `gdb -p <PID>` attach 用 |
| `Makefile` | `make` 編兩支(已帶 `-g -O0`) |

```bash
cd ~/Documents/kernel-c-practice/c-basics/day-27
make
```

---

## ⚠️ Arch 重點:core dump 預設被 systemd 接走

在 Arch + systemd 上,`ulimit -c unlimited` 後 crash **不會**在當前目錄生出 `./core`,因為 `/proc/sys/kernel/core_pattern` 預設是 `|/usr/lib/systemd/systemd-coredump …` —— core 被丟進 systemd 的 coredump 儲存。兩條路都示範一次:

### 路線 A(Arch 原生,推薦)— 用 `coredumpctl`

```bash
./segfault                       # 直接跑,讓它 crash
coredumpctl list segfault        # 看剛剛的 core
coredumpctl debug segfault       # 直接開 gdb 載入最新的 core(等同 gdb ./segfault core)
```

### 路線 B(經典 `./core`)— 暫時改 core_pattern

```bash
ulimit -c unlimited                              # 允許 core(只影響當前 shell)
sudo sysctl -w kernel.core_pattern=core          # 暫時改成在當前目錄生 core
./segfault                                        # → Segmentation fault (core dumped)
gdb ./segfault core                               # 載入 core
# 玩完還原:sudo sysctl -w kernel.core_pattern='|/usr/lib/systemd/systemd-coredump %P %u %g %s %t %c %h'
```

---

## 任務（通關前逐項打勾）

- [ ] 開好 core dump(路線 A 或 B 擇一)
- [ ] 編譯跑出 crash
- [ ] gdb 載入 core 後 `bt` —— 看到 `crash() <- main()`,指出是 `crash()` 第幾行 deref
- [ ] `print p` 確認真的是 `0x0`(NULL)
- [ ] `frame` 切到 `main`,`print p` 看傳進去前也是 NULL
- [ ] **attach 模式**:`./loop &` 然後 `gdb -p $(pgrep loop)`,Ctrl-C 停下,`bt` / `print tick`,`detach` 離開

## 通關標準

不看提示,能從 core dump 自己講出:**segfault 發生在哪一行、哪個指標是 NULL、呼叫鏈怎麼走到那裡**。

> 💡 GDB TUI:進 gdb 後 `Ctrl-X A`(或 `layout src`)開原始碼面板,crash 行會直接高亮。Java 工程師對照表見 [wiki GDB-userspace §4.5](../../../linux_kernel/wiki/tools/GDB-userspace.md)。

通過 → Day 28:pwndbg + 整週小測,GDB Week 收尾。
