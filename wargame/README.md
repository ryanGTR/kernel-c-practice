# Kernel Wargame — 作戰地圖

用闖關（CTF 風格）的方式學 Linux kernel module。**AI 當關主，QEMU/本機當競技場，jserv 的 LKMPG 當主幹教材。**

## 核心循環

每一關都跑同一個循環，不是「讀過就算」：

1. **讀** 對應的 LKMPG 章節 + 跑它的範例（spine／教材）
2. **打** 關主把範例改成關卡：埋 bug／藏 flag／要你擴充 → 逼你輸出
3. **驗** 拿 `dmesg` / `ftrace` / `bpftrace` 證明過關條件成立（機器可驗證，不靠嘴說）
4. **寫** 收斂寫進 `~/Documents/linux_kernel/wiki/`（附 `file:line` + kernel 版本）

> 三個動詞記住就好：**先猜 → 驗證 → 寫回。**

## 三條鐵律（讓它真的是遊戲）

1. **過關條件必須機器可驗證**：flag 出現在 dmesg、指定 trace 出現、bug 不再復現。客觀、可重跑。
2. **先打再給提示**：卡住喊 hint，關主只給「往哪看」，不給答案。
3. **升級制**：過了才解鎖下一關，難度遞增，每關聚焦一條執行路徑。

## 基本操作：編譯 → 載入 → 觀察 → 卸載

kernel module **不是可執行程式**（沒有 `main()`，不能 `./xxx`），它是「注入到正在運行的 kernel」的程式碼。所以流程不是「編譯→執行」，而是這四步（每一關都會用到）：

| 步驟 | 指令 | 在幹嘛 | Java 類比 |
|---|---|---|---|
| ① 編譯 | `make` | 產出 `xxx.ko`（**可被掛進 kernel 的模組**，不是執行檔） | `javac` 產出 class |
| ② 載入 | `sudo insmod xxx.ko` | 把模組塞進 kernel，**此刻** `module_init` 被呼叫 | 熱掛 jar 進運行中的 JVM |
| ③ 觀察 | `sudo dmesg \| tail` | 看 kernel log（`pr_info` 寫到 kernel ring buffer，不在你的終端機） | 看 server log，非 `System.out` |
| ④ 卸載 | `sudo rmmod xxx` | 把模組移出 kernel，`module_exit` 被呼叫 | `@PreDestroy` / `destroy()` |

```bash
# 一輪完整循環（以模組名 xxx 為例）
make                        # ① 編譯
sudo insmod xxx.ko          # ② 載入（init 觸發）
sudo dmesg | tail -n 5      # ③ 觀察輸出
sudo rmmod xxx              # ④ 卸載（exit 觸發）
sudo dmesg | tail -n 5      # 再看一次，確認 exit 的訊息
```

**常用輔助指令：**
```bash
lsmod | grep xxx       # 模組現在有沒有被載入（refcount 為 0 才能安全 rmmod）
modinfo xxx.ko         # 看模組 metadata（license / description / 參數）
sudo dmesg -w          # 即時跟看 kernel log（Ctrl+C 離開）
sudo dmesg -C          # 清空 dmesg，讓下一輪輸出乾淨好讀
```

**常見雷：**
- `dmesg` 看不到輸出？Arch 預設 `kernel.dmesg_restrict=1`，要加 `sudo`。
- 同名模組**不能載入兩次**——重載前先 `sudo rmmod xxx`。
- `make` 報 `*** missing separator`：Makefile 的指令行縮排必須是 **Tab**，不是空白。
- 編譯要零 warning（`-Wall -Wextra -Werror`）；用不到的變數會被 `-Werror` 擋下。

## 關卡 ↔ LKMPG 章節對照

| 關卡 | 主題 | LKMPG 章節 | 狀態 |
|---|---|---|---|
| LV0 | 第一個 module（hello world） | Hello World | ✅ 通關 |
| LV0.5 | 模組參數（載入時傳值進去） | Passing Command Line Arguments | 🟢 進行中 |
| LV0.7 | `/proc` 節點（讀它吐出內容） | The /proc File System | 🔒 |
| LV0.9 | char device（user space 讀寫裝置） | Char Device Files | 🔒 |
| — | **架 QEMU 沙盒**（之後的關會弄崩 kernel） | — | 🔒 |
| LV1 | spinlock：持鎖時睡眠 → 抓 + 修 | 同步 / 並行相關章節 | ❄️ 冰起來，回頭打 |
| LV2 | race condition：用 bpftrace 抓競態 | — | 🔒 |
| LV3 | VFS：假檔案節點藏 flag | — | 🔒 |
| LV4 | panic 逆推：看 oops/call trace 找兇手 | — | 🔒 |
| BOSS | char device + 正確的鎖 + 從 user space 取 flag | 綜合 | 🔒 |

## 兩種教材的分工（都是 jserv 體系）

- **LKMPG**（sysprog21 維護版）→ module「**怎麼動手**」的機制。**現在這條線。** 重點：跟得上現代 kernel 版本，不會拿舊 API 唬人。
- **成大《Linux 核心設計》課程 +《你所不知道的 C 語言》** → 背後「**為什麼**」的認知地基（並行、排程、記憶體模型、C 底層）。手感起來後拿這個拉認知上限。

> ⚠️ 讀 LKMPG / 舊範例時，先看 [`kernel7-api-notes.md`](kernel7-api-notes.md)：舊書常用已被改掉的 API（`class_create`、`access_ok`、`strlcpy`、`proc_ops`…），在 7.0 會編不過。那頁是對照本機 7.0 標頭驗證過的「踩雷地圖」。

## 目錄結構

每一關都有一份 `spec.md`（關卡設計書：情境 / 任務 / 過關條件 / 提示）。可直接打的關附骨架 `.c`（含 TODO）+ Makefile；會弄崩 kernel 的關（LV1+）骨架/buggy 程式碼等解鎖時才放（提早看到會破壞「先重現再修」的設計）。

```
wargame/
├── README.md          ← 作戰地圖
├── lv0/               ← ✅ hello world（hello.c + Makefile）
├── lv0.5/             ← 🟢 模組參數（greet.c 含 TODO + Makefile）
├── lv0.7/             ← 🔓 /proc 節點（proc_info.c 含 TODO + Makefile + spec.md）
├── lv0.9/             ← 🔓 char device（chardev.c 含 TODO + Makefile + spec.md）
├── qemu-sandbox/      ← 🛡️ QEMU 沙盒設定指南（SETUP.md）← LV1 前必經
├── lv1/               ← 🔒 spinlock 持鎖睡眠（spec.md）
├── lv2/               ← 🔒 race condition（spec.md）
├── lv3/               ← 🔒 VFS 藏 flag（spec.md）
├── lv4/               ← 🔒 panic 逆推（spec.md）
└── boss/              ← 🔒 char device + 鎖 + 取 flag（spec.md）
```

## 安全提醒

LV0 ~ LV0.9 在本機跑沒事（弄不壞東西）。
**LV1 起會故意讓 kernel 出包（可能 hang 整台機器）→ 開打前先架好 QEMU 沙盒，崩的是虛擬機不是 Framework。**

## 進度紀錄

- `2026-05-31` 建立 wargame，LV0 開打。環境驗證 OK（linux-headers 7.0.10，build dir 正常，hello.ko 可編出）。
- `2026-05-31` 從 `linux_kernel/` 搬到 `kernel-c-practice/wargame/`，與 phase1~4 並列（同一個 git repo）。
- `2026-05-31` ✅ LV0 通關（完整 insmod→rmmod 生命週期，lsmod 驗證）。開 LV0.5：模組參數。
- `2026-05-31` 補上「編譯→載入→觀察→卸載」基本操作小抄；設好整條後續關卡（每關一份 spec.md，LV0.7/LV0.9 附骨架，QEMU 沙盒指南，LV1~BOSS 設計書）。
