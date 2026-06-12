# 📌 LKMPG / 舊書範例 → kernel 7.0 API 對照

打 wargame、讀 LKMPG 或任何 kernel module 教學時，**舊範例常用已被改掉的 API，在 7.0 會直接編不過或行為不同**。這頁是「踩雷地圖」。

> 以下每條都已對照本機 `linux-headers 7.0.10` 的標頭驗證過（非憑印象）。
> 來源路徑：`/lib/modules/7.0.10-arch1-1/build/include/...`

## 一眼對照表

| 主題 | 舊書常見寫法（會壞） | kernel 7.0 正確寫法 | 會在哪關咬你 |
|---|---|---|---|
| /proc 操作表 | `struct file_operations` + `proc_create(...,&fops)` | `struct proc_ops` + `.proc_open/.proc_read/.proc_lseek/.proc_release` | LV0.7 |
| 取 /proc 私有資料 | `PDE_DATA(inode)` | `pde_data(inode)`（小寫） | LV3 |
| 建 device class | `class_create(THIS_MODULE, "name")` | `class_create("name")`（**砍掉第一個 owner 參數**） | LV0.9 進階 / BOSS |
| user 位址檢查 | `access_ok(VERIFY_WRITE, addr, size)` | `access_ok(addr, size)`（**砍掉第一個 type 參數**） | 任何碰 copy 的關 |
| 字串複製 | `strcpy` / `strlcpy` | `strscpy(dst, src, size)`（**`strlcpy` 在 7.0 已移除**） | LV3 / BOSS |
| timer | `setup_timer(&t, fn, data)`、callback 收 `unsigned long` | `timer_setup(&t, fn, flags)`、callback 收 `struct timer_list *`，用 `from_timer()` 取回你的結構 | 之後做計時器時 |
| ioctl | `.ioctl` | `.unlocked_ioctl`（早就改了） | BOSS 進階 |

## 細節與「為什麼」

### 1. /proc：`file_operations` → `proc_ops`（kernel 5.6 起）
7.0 的 `struct proc_ops`（`include/linux/proc_fs.h:36`）欄位是 `proc_open / proc_read / proc_write / proc_lseek / proc_release`。
舊書用 `file_operations` 掛 `proc_create` 在 7.0 型別不符。**LV0.7 的骨架已經幫你用對了。**

### 2. `class_create` 砍掉 owner 參數
7.0：`struct class *class_create(const char *name);`（`include/linux/device/class.h:226`）
舊書：`class_create(THIS_MODULE, name)` → 在 7.0 變成「參數太多」編譯錯誤。
**Java 類比**：就像某個 library 改版把 constructor 的第一個參數拿掉了，你照舊文件呼叫就 compile error。

### 3. `access_ok` 砍掉 type 參數（kernel 5.0 起）
7.0：`access_ok(addr, size)`（`include/asm-generic/access_ok.h:45`）
舊書：`access_ok(VERIFY_WRITE, addr, size)` → `VERIFY_WRITE` 這個巨集早就不存在。
（多數情況你用 `copy_to_user`/`copy_from_user` 就好，它們內部自己檢查，不必手動 `access_ok`。）

### 4. `strlcpy` 已移除，用 `strscpy`
7.0 的 `string.h` 只有 `strscpy`（會保證 NUL 結尾、回傳複製長度，溢位回 `-E2BIG`），`strlcpy` 找不到了。
在 kernel 裡**永遠別用 `strcpy`**（沒有邊界）——LV3 寫密碼比對、BOSS 處理 buffer 時都用 `strscpy`。

### 5. `PDE_DATA` → `pde_data`（小寫）
7.0：`pde_data(const struct inode *)`（`include/linux/proc_fs.h:123`）。舊書大寫 `PDE_DATA` 會找不到符號。

### 6. timer：`setup_timer` → `timer_setup`
7.0 的 `timer_setup(timer, callback, flags)`（`include/linux/timer.h:120`）。callback 簽章變成收 `struct timer_list *`，再用 `from_timer(my, t, member)`（container_of 的包裝）拿回你的外層結構——不再用舊的 `unsigned long data` 傳值。

## 心法

看任何 kernel 範例時，先問三件事：
1. **它是哪個 kernel 版本寫的？** 沒標版本的最危險。
2. **編不過先別慌**——多半是 API 改名/改簽章，來這頁或 `git log` 查該函式的演進。
3. **以標頭為準，不以教學為準**：`grep` 你 `linux-headers` 裡的真實簽章，那才是 7.0 的事實。這也呼應 wargame 核心循環的「驗證」。

---
打到對應關卡若撞到沒列在這的差異，跟我說，我查標頭後補進這頁。
