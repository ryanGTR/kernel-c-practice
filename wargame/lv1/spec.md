# LV1 ──「沉睡的鎖 The Sleeping Lock」（spinlock）

對應 LKMPG：同步 / 並行相關章節
狀態：🔒（需先架好 QEMU 沙盒；在 VM 裡打）

## 情境

有一個 module 看起來人畜無害，卻犯了同步的頭號大忌：**在持有 spinlock 的時候做了會睡眠的事**。持 spinlock 時 CPU 不能被排程器換走，一旦睡著就再也醒不來 → 死鎖。

**Java 類比**：像在 `synchronized` 區塊裡呼叫一個會阻塞、且要等「同一把鎖」才能解開的操作——自己把自己鎖死。

## 任務

① 重現罪行 → 貼出 dmesg 的 BUG
② 修好它，讓工作照做、但合法
③ 證明罪行消失 + flag 印出來

## 過關條件（機器可驗證）

- 重現：`dmesg` 出現 `BUG: sleeping function called from invalid context`
- 通關：insmod 後 dmesg 有 `lv1: work done`，且**完全沒有**上面那行 BUG

> 需要 kernel 開 `CONFIG_DEBUG_ATOMIC_SLEEP=y`（裁判），否則抓不到——見 `qemu-sandbox/SETUP.md`。

## 🔑 提示（卡住才看）

- Hint1：持有 spinlock 時，CPU 還能不能被排程器換走？`kmalloc(GFP_KERNEL)` 拿不到記憶體時會幹嘛？
- Hint2：看 `might_sleep()`。GFP flag 有沒有「我不准睡」的版本？
- Hint3：最乾淨的修法不是換 flag，是換「時機」——把會睡的操作移出臨界區。

## 解鎖

通關 → LV2（race condition）。

---
打到這關時跟我說，我放 buggy 的 `lv1.c` 進來（提早給會破壞「自己重現」的關卡設計）。
