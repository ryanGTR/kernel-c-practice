# 12_proc_info -- 建立 /proc 虛擬檔案

## 目標

建立一個 `/proc/my_info` 項目，讀取時回傳系統資訊。這個練習連結了使用者空間（`cat`、`read` 系統呼叫）和核心空間（`proc_ops`、`simple_read_from_buffer`）。

## 行為規格

| 操作 | 行為 |
|------|------|
| `insmod proc_info.ko` | 建立 `/proc/my_info`，dmesg 顯示載入訊息 |
| `cat /proc/my_info` | 顯示系統資訊（見下方格式） |
| 連續 `cat` 多次 | 讀取計數器遞增 |
| `rmmod proc_info` | 移除 `/proc/my_info`，dmesg 顯示卸載訊息 |

### 輸出格式

```
Module loaded at: 4295012345 jiffies
Read count: 3
Kernel version: 6.17.0-20-generic
```

- **Module loaded at**: 模組載入時的 jiffies 值
- **Read count**: 此檔案被讀取的次數（用 `atomic_t` 實作）
- **Kernel version**: `utsname()->release`

## 練習技能

- `proc_create()` — 建立 /proc 項目
- `proc_remove()` — 移除 /proc 項目
- `proc_ops` 結構（新核心）或 `file_operations`（舊核心）
- `simple_read_from_buffer()` — 最簡單的讀取模式
- `snprintf()` — 核心中的格式化字串（不能用 sprintf）
- `atomic_t` / `atomic_inc()` / `atomic_read()` — 原子計數器（避免競爭條件）
- `jiffies` — 核心計時器 tick 數
- `utsname()` — 取得核心版本資訊

## 工具

- `make`（核心建置系統）
- `insmod` / `rmmod` — 載入和卸載模組
- `cat /proc/my_info` — 讀取 proc 項目
- `dmesg` / `dmesg | tail` — 檢視核心日誌
- `scripts/checkpatch.pl -f proc_info.c` — 程式碼風格檢查

## 完成清單

- [ ] `make` 編譯成功，產生 `proc_info.ko`
- [ ] `scripts/checkpatch.pl -f proc_info.c` 零警告
- [ ] `insmod proc_info.ko` 後 `/proc/my_info` 存在
- [ ] `cat /proc/my_info` 顯示正確資訊
- [ ] 連續 `cat` 多次，read count 遞增
- [ ] `rmmod proc_info` 後 `/proc/my_info` 消失
- [ ] `dmesg` 顯示載入和卸載訊息

---

<details>
<summary>提示（先自己想，卡住再看）</summary>

1. 用 `proc_create("my_info", 0444, NULL, &my_proc_ops)` 建立項目
2. `proc_ops` 只需要設 `.proc_read`，其他可以不設
3. read 函式的模式：用 `snprintf` 寫到 buffer → `simple_read_from_buffer` 回傳
4. 用 `static atomic_t read_count = ATOMIC_INIT(0)` 宣告計數器
5. 在 read 函式一開始 `atomic_inc(&read_count)`，然後 `atomic_read(&read_count)` 取值
6. `jiffies` 是全域變數，模組載入時存到 `static unsigned long load_time`
7. `utsname()->release` 取得核心版本字串（需要 `#include <linux/utsname.h>`）

</details>
