# LV4 ──「驗屍報告 The Autopsy」（panic / oops 逆推）

對應 LKMPG：除錯 / oops 解讀
狀態：🔒（在 QEMU 沙盒裡打——這關真的會崩）

## 情境

給你一個 module，`insmod` 下去 kernel 就 oops（甚至 panic）。你的工作不是寫碼，是**當法醫**：讀 kernel 噴出的 call trace，逆推「哪一行、為什麼爆」，然後修好它。

**Java 類比**：你拿到一份 `NullPointerException` 的 stack trace，要從最上面那幾個 frame 推回「哪個物件是 null、在哪一行被 deref」。kernel 的 oops 就是 kernel 版的 stack trace，只是要自己對 symbol / 行號。

## 任務

① 重現：`insmod` → dmesg 噴出 oops（`BUG: kernel NULL pointer dereference` 或類似）
② 解讀 call trace：找出爆掉的函式 + 對應原始碼行
③ 修好它，讓模組能乾淨載入並印出 flag

## 過關條件（機器可驗證）

- 重現：dmesg 出現 `Oops` / `BUG: kernel NULL pointer dereference`，且 call trace 指向模組函式
- 通關：修好後 insmod 乾淨無 oops，dmesg 出現 `LV4: survived, flag = LV4{...}`
- 加分：說得出「`RIP` 指向哪、為什麼那個位址會爆」

## 🔑 提示（卡住才看）

- Hint1：oops 裡的 `RIP:` 那行就是「出事的指令位址」；`Call Trace:` 由上往下是呼叫堆疊。先找你模組的函式名。
- Hint2：要把位址對到原始碼行，用 `CONFIG_DEBUG_INFO=y` 編的模組 + `addr2line -e xxx.ko <offset>`，或 `gdb xxx.ko` 後 `list *(函式+偏移)`。
- Hint3：最常見的兇手就是「用了一個沒初始化 / 是 NULL 的指標」。看 trace 指的那行在 deref 誰。

## 解鎖

通關 → BOSS。

---
打到這關時跟我說，我放會 oops 的 `oops.c` 進來。
