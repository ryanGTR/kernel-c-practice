# BOSS ──「守門人 The Gatekeeper」（char device + 正確的鎖 + 取 flag）

對應 LKMPG：char device + 並行 + uaccess 綜合
狀態：🔒（最終關，在 QEMU 沙盒裡打）

## 情境

把前面學的全部縫起來：寫一個**有內部狀態、會被多個 process 同時存取**的 char device。flag 只在「並行存取下狀態仍然正確」時才拿得到——鎖加錯或漏加，壓力測試就會讓它崩掉或吐出壞掉的值。

**Java 類比**：寫一個 thread-safe 的服務元件——多個 client 同時打，內部計數/狀態不能錯亂。你在 Java 用 `synchronized` / `ReentrantLock` / 並發容器解決的問題，這次用 kernel 的 spinlock / mutex 在 kernel space 重做一遍。

## 任務

寫 `/dev/lv_boss` 字元裝置：
- 維護一個內部狀態（例：一個受保護的計數器，或一段需要 read-modify-write 的緩衝）
- 提供 `open` / `read` / `write`，內部臨界區用 **mutex 或 spinlock** 正確保護
- 設計成：只有在並行存取下狀態完全正確時，read 才回傳 `LV_BOSS{...}`；有 race 就回傳壞值或拒絕

## 過關條件（機器可驗證）

```bash
# 用多個 process 同時壓它（userspace 測試程式或一堆 background cat/echo）
for i in $(seq 1 50); do (echo hit > /dev/lv_boss &) ; done; wait
cat /dev/lv_boss     # 鎖正確 → LV_BOSS{...}；鎖錯 → 壞值/崩潰
```

- 通關：高並行壓力測試下，狀態**每次都正確**、無 oops、最終 read 拿到 flag
- 拔掉你的鎖再跑一次，應該要能**看到它壞掉**——證明你的鎖真的在做事（不是裝飾）

## 🔑 提示（卡住才看）

- Hint1：選鎖——臨界區裡會睡眠（如 `copy_from_user` 可能 page fault）就用 `mutex`；純短臨界區、不可睡就用 `spinlock`。想想 LV1 的教訓。
- Hint2：`/dev` 節點自動出現：用 `alloc_chrdev_region` + `cdev_init`/`cdev_add` + `class_create` + `device_create`，省去手動 `mknod`。
- Hint3：先寫「沒有鎖」的版本，用壓測證明它會壞；再加鎖，證明它好了——這就是你親手驗證「鎖有沒有用」的方式。
- Hint4：卸載時記得反向拆乾淨（device_destroy → class_destroy → cdev_del → unregister_chrdev_region），順序錯會洩漏或 oops。

## 通關 = 出師 🎓

打贏 BOSS，你已經親手走過：模組生命週期、參數、/proc、char device、同步（spinlock/mutex/atomic）、race、oops 解讀、QEMU 除錯。接下來就能往 LKMPG 後面章節（中斷處理、kprobe、排程）或成大《Linux 核心設計》的「為什麼」層繼續爬。

---
打到這關時跟我說，我放含 TODO 的骨架進來。
