# 11_hello_module -- 第一個核心模組

## 目標

撰寫你的第一個 Linux 核心模組。載入時印出 "Hello, kernel!"，卸載時印出 "Goodbye, kernel!"。然後加入模組參數 `name`（字串），讓它印出 "Hello, \<name\>!"。

## 行為規格

### 基本版

| 操作 | 行為 |
|------|------|
| `insmod hello.ko` | dmesg 顯示 `Hello, kernel!` |
| `rmmod hello` | dmesg 顯示 `Goodbye, kernel!` |
| `modinfo hello.ko` | 顯示 license、author、description |

### 加入模組參數

| 操作 | 行為 |
|------|------|
| `insmod hello.ko name=Ryan` | dmesg 顯示 `Hello, Ryan!` |
| `insmod hello.ko` (無參數) | dmesg 顯示 `Hello, kernel!`（預設值） |
| `cat /sys/module/hello/parameters/name` | 顯示目前的 name 值 |

## 練習技能

- `module_init()` / `module_exit()` — 模組進入點和離開點
- `pr_info()` — 核心日誌輸出（相當於 `printk(KERN_INFO ...)`）
- `MODULE_LICENSE()` — 宣告授權（必須是 GPL 相容）
- `MODULE_AUTHOR()` / `MODULE_DESCRIPTION()` — 模組後設資料
- `module_param()` — 宣告可從命令列傳入的參數
- 核心建置系統（`obj-m`、Kbuild）

## 工具

- `make`（使用核心建置系統，不是一般 gcc）
- `insmod` / `rmmod` — 載入和卸載模組
- `dmesg` / `dmesg | tail` — 檢視核心日誌
- `lsmod` — 列出已載入模組
- `modinfo hello.ko` — 檢視模組後設資料
- `scripts/checkpatch.pl -f hello.c` — 核心程式碼風格檢查

## 完成清單

- [ ] `make` 編譯成功，產生 `hello.ko`
- [ ] `scripts/checkpatch.pl -f hello.c` 零警告
- [ ] `insmod hello.ko` 後 `dmesg` 顯示 Hello 訊息
- [ ] `rmmod hello` 後 `dmesg` 顯示 Goodbye 訊息
- [ ] `modinfo hello.ko` 顯示 license、author、description
- [ ] `insmod hello.ko name=Ryan` 參數功能正常
- [ ] `cat /sys/module/hello/parameters/name` 顯示正確值

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. Makefile 用的是核心建置系統：`make -C /lib/modules/$(uname -r)/build M=$(PWD) modules`
2. `module_init(fn)` 指定載入時呼叫的函式，`module_exit(fn)` 指定卸載時的函式
3. init 函式回傳 `int`（成功回傳 0），exit 函式回傳 `void`
4. `MODULE_LICENSE("GPL")` 是必須的，不然會有 taint 警告
5. `module_param(name, charp, 0444)` 宣告一個字串參數，0444 表示在 sysfs 中唯讀

</details>
