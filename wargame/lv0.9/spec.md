# LV0.9 ──「Talking to a device」（char device）

對應 LKMPG：**Char Device Files**
狀態：🔒（通關 LV0.7 後解鎖）

## 情境

`/dev/xxx` 這類裝置檔背後就是一個 char device（字元裝置）。userspace `open` 它拿到一條串流，`read` 把 bytes 拉出來，讀到底回傳 0（EOF）。

**Java 類比**：char device 像 kernel 開的一個 `InputStream`——`open()` 給你串流，`read()` 一直拉到 EOF（回傳 0）為止。

## 任務

註冊一個字元裝置，當 userspace 讀 `/dev/lv09` 時，吐出這一行：

```
LV0.9: you are talking to a char device
```

## 過關條件（機器可驗證）

```bash
sudo insmod chardev.ko
sudo dmesg | tail            # 找到 kernel 配給的 major number（你在 init 裡 pr_info 它）
sudo mknod /dev/lv09 c <major> 0   # 用那個 major 建裝置節點
cat /dev/lv09                # → LV0.9: you are talking to a char device
sudo rm /dev/lv09
sudo rmmod chardev
```

`cat` 能讀到那行、且**不會無限重複/卡住**（代表你正確回傳了 EOF）= 通關。

## 🔑 提示（先自己撞再看）

- Hint1：骨架把 `file_operations`、open/release 都寫好了，你填三個 TODO：`dev_read`、註冊、反註冊。
- Hint2（最關鍵的 TODO 1）：char device 的 read 要處理「分段讀」。最簡單的單次模型：
  - 若 `*off > 0` → 已經讀過了，回傳 `0`（EOF），`cat` 才會停。
  - 否則用 `copy_to_user(buf, msg, len)` 把訊息複製給 user，把 `*off` 往前推，回傳實際送出的 byte 數。
  - `copy_to_user` 回傳「沒成功複製的 byte 數」，0 才是全成功——別忘了檢查。
- Hint3：TODO 2 用 `major = register_chrdev(0, DEVICE_NAME, &fops);`（第一個參數 0 = 讓 kernel 自己挑 major）。挑完一定要 `pr_info("major = %d\n", major);`，不然你不知道 mknod 要填什麼。
- Hint4：TODO 3 用 `unregister_chrdev(major, DEVICE_NAME);`。

> 進階（之後 BOSS 會用到）：可以改用 `alloc_chrdev_region` + `cdev_add` + `class_create`/`device_create` 讓 `/dev/lv09` 自動出現，不必手動 `mknod`。這關先用簡單的 `register_chrdev` 就好。

## 解鎖

通關 → 架 QEMU 沙盒（見 `qemu-sandbox/SETUP.md`），之後才開始打會弄崩 kernel 的關。
