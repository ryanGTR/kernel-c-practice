# LV3 ──「上鎖的抽屜 The Locked Drawer」（VFS / 藏 flag）

對應 LKMPG：/proc、char device、syscall 章節綜合
狀態：🔒（在 QEMU 沙盒裡打）

## 情境

這關你要自己當「出題者」的對手：寫一個 kernel 介面，它**藏著一個 flag**，但只在 userspace 用「正確方式互動」時才吐出來。這逼你同時處理 read 跟 write、以及在 kernel 裡保存狀態。

**Java 類比**：像一個有狀態的 servlet——先 POST 對的密碼（write），session 狀態改變，之後 GET（read）才回傳機密；密碼錯就回傳「拒絕」。

## 任務

建一個 `/proc/wargame_lv03`（或 char device）：
- 一開始 `cat` 它 → 回傳 `LOCKED`
- 往它 `echo "<密碼>" >` 寫入正確密碼後
- 再 `cat` → 回傳 flag：`LV3{...}`
- 密碼錯 → 維持 `LOCKED`

## 過關條件（機器可驗證）

```bash
cat /proc/wargame_lv03                 # LOCKED
echo "open-sesame" > /proc/wargame_lv03
cat /proc/wargame_lv03                 # LV3{...}  ← flag
echo "wrong" > /proc/wargame_lv03
cat /proc/wargame_lv03                 # LOCKED 回去
```

能依互動正確切換鎖定/解鎖 = 通關。

## 🔑 提示（卡住才看）

- Hint1：你需要一個 module 內的狀態變數（`static bool unlocked;`），write handler 比對密碼來改它，read handler 依它決定回傳什麼。
- Hint2：write 用 `copy_from_user` 把 user 的字串拉進 kernel buffer，記得處理結尾換行 `\n` 跟長度上限。
- Hint3：想想多個 process 同時讀寫這個狀態會不會出問題——這就接到 BOSS 的鎖了。

## 解鎖

通關 → LV4（panic 逆推）。

---
打到這關時跟我說，我放含 TODO 的骨架進來。
