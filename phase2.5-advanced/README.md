# Phase 2.5 — 進階 C 訓練（擴充池）

> **不在主線週程內**。Phase 1-4 已經是完整學習路徑。這裡的題目是**把同一個基礎題加難**，適合以下情境：
>
> - Phase 2 做完覺得太簡單（戒律 #3：三題白紙 checklist 全 yes）
> - Week H 第一個 patch 寄出後，等 review 的空窗期訓練
> - 想在正式寫 kernel code 前，把 C 再磨一層

這不是「你必須做」，是「你想挑戰時的題庫」。

## 難度提升手法

Phase 1-2 教你把事情做出來，Phase 2.5 教你把事情**做得像樣**：

1. **加約束**（同題不同工）— 不准用 stdio、不准用 malloc、buffer 固定大小
2. **加嚴格度** — 把警告拉到極致，把所有 UB 跑 sanitizer 抓出來
3. **加規模** — 自己寫 malloc、處理 GB 級檔案、多 thread 競爭
4. **加驗證** — fuzzing、property test、assertion 密度

## 四個擴充題

| # | 題目 | 對應 | 難度 | 學什麼 |
|---|------|------|------|--------|
| A1 | `A1_my_wc_syscall` | Phase 1 `01_my_wc` 加約束 | ★★★ | syscall vs libc、buffer 管理、UTF-8 |
| A2 | `A2_strict_warnings` | Phase 2 `06_linked_list` 加嚴 | ★★☆ | 警告 flag 全開、ASan/UBSan |
| A3 | `A3_malloc_lab` | CS:APP 改版 | ★★★★ | 自己實作 malloc/free/realloc |
| A4 | `A4_pthread_basics` | 新題（並行） | ★★★ | mutex/condvar、thread-safe 容器 |

延伸方向（不寫 spec，自己挑）：

- **A5** — 對 Phase 2 `07_hash_table` 跑 libFuzzer / AFL++
- **A6** — SPSC lock-free ring buffer（讀 Linux kernel `kfifo` 前的熱身）
- **A7** — 讀 musl libc `src/string/` 任一函式，抄一遍 + 寫逐行註解

## 建議順序

1. **A2 最便宜**（半天），先做。把你 Phase 2 寫好的 linked_list 用嚴格 flag 重 compile，看出一堆問題 → 修 → 跑 ASan/UBSan。這會瞬間提升你的「C 美感」。
2. **A1 次之**（1-2 天），教你看清 libc 幫你做了多少事。
3. **A4 第三**（2-3 天），並行是 kernel 日常，早練早好。
4. **A3 最後**（3-5 天），Malloc Lab 是功力檢驗。

## 工具鏈要求

所有 Phase 2.5 題目必須過這套 flag：

```
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic \
         -Wconversion -Wshadow -Wcast-align \
         -Wstrict-prototypes -Wmissing-prototypes \
         -Wdouble-promotion -Wnull-dereference \
         -fstack-protector-strong -D_FORTIFY_SOURCE=2
```

Debug build 加：`-fsanitize=address,undefined -g -O1`

## 完成標準（通用）

- [ ] 上述 CFLAGS 零 warning
- [ ] ASan + UBSan 零 error
- [ ] `valgrind --leak-check=full --show-leak-kinds=all` 乾淨
- [ ] 白紙能重寫核心邏輯
