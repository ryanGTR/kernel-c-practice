# LV0.7 ──「A window into the kernel」（/proc 節點）

對應 LKMPG：**The /proc File System**
狀態：🔒（通關 LV0.5 後解鎖）

## 情境

`/proc` 是 kernel 對外開的一扇窗——userspace 直接 `cat` 它就能把 kernel 內部的即時狀態讀出來，不用寫 syscall。

**Java 類比**：`/proc` 節點像 kernel 開的一個唯讀 REST endpoint，`cat` 就是去 GET 它。

## 任務

建一個 `/proc/wargame_lv07`，每次被讀取就印出：

```
LV0.7: read count = N
```

其中 `N` 每讀一次 +1（讀第一次是 1，第二次是 2…）。

## 過關條件（機器可驗證）

```bash
sudo insmod proc_info.ko
cat /proc/wargame_lv07     # → LV0.7: read count = 1
cat /proc/wargame_lv07     # → LV0.7: read count = 2
sudo rmmod proc_info
cat /proc/wargame_lv07     # → No such file or directory（節點消失）
```

計數會遞增、卸載後節點消失 = 通關。

## 🔑 提示（先自己撞再看）

- Hint1：骨架已經把 `proc_ops` 樣板寫好了（kernel 5.6 起 `/proc` 用 `proc_ops` 取代 `file_operations`）。你只要填三個 TODO。
- Hint2：TODO 1 在 `lv07_show()` 裡——先 `read_count++`，再用 `seq_printf(m, "...", ...)` 印那行。`seq_file` 會幫你處理 buffer 跟 user-space 複製，比自己 `copy_to_user` 安全。
- Hint3：TODO 2 用 `proc_create(PROC_NAME, 0444, NULL, &lv07_ops)`，回傳值存進 `entry`。
- Hint4：TODO 3 用 `proc_remove(entry)`（或 `remove_proc_entry(PROC_NAME, NULL)`）。沒移除的話卸載後節點還在 → 變殭屍。

## 解鎖

通關 → LV0.9（char device）。
