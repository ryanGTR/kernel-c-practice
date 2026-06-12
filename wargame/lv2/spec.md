# LV2 ──「看不見的賽跑 The Invisible Race」（race condition）

對應 LKMPG：並行 / 同步章節
狀態：🔒（在 QEMU 沙盒裡打）

## 情境

兩個 kernel thread 同時對一個共享變數 `counter++`，沒有任何保護。`counter++` 不是原子操作（讀→改→寫三步），兩條 thread 交錯就會「丟更新」——最後的值比應有的少。

**Java 類比**：兩個 thread 對沒加 `synchronized` / 非 `AtomicInteger` 的 `int` 做 `i++`，典型 lost update。你在 Java 早就遇過，只是這次發生在 kernel space。

## 任務

① 重現：載入後 `counter` 的最終值 **小於** 預期（`2 × ITERS`），而且每次跑數字還不一樣
② 用工具觀察競態（`bpftrace` 加分）
③ 修好它，讓 `counter` **每次都剛好等於** 預期值

## 過關條件（機器可驗證）

- 重現：dmesg 的 `LV2: counter = X (expected Y)` 出現 `X < Y`
- 通關：修好後重跑數次，**每次** `X == Y`

## 🔑 提示（卡住才看）

- Hint1：`counter++` 在組語裡是幾條指令？兩條 thread 同時做會怎樣？
- Hint2：兩種修法方向——(a) 用 `spinlock` 把 `counter++` 包成臨界區；(b) 把 `counter` 換成 `atomic_t`，用 `atomic_inc()`。想想哪個開銷小。
- Hint3：bpftrace 可以掛在函式上看兩條 thread 交錯進入的時序，肉眼確認「賽跑」真的發生。

## 解鎖

通關 → LV3（VFS 藏 flag）。

---
打到這關時跟我說，我放 racy 的 `race.c` 進來。
