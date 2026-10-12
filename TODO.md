# Kernel C Practice — 進度表

> 真實進度只認「親手寫完 + 過 `-Wall -Wextra -Werror` + 過 valgrind + 對照真實輸出」。
> 骨架/spec 不算通關。斷 session 先看這份。

最後更新：2026-09-29（06_linked_list 通關；練習總佇列改看 `~/Documents/PRACTICE.md`）

---

## 🔜 下次開工先看這裡（2026-07-13 交接）

**主線任務：`02_my_cat` 的 struct 重構練習。** 完整題目在
[`phase1-coreutils/02_my_cat/EXERCISE-struct.md`](phase1-coreutils/02_my_cat/EXERCISE-struct.md) —— 那份自足，照著做即可。

一句話背景：Ryan 問「為什麼 C 的巢狀判斷比 Java 難」，答案是
**C 沒有 exception（錯誤沒出口 → 就地 if → 縮排變深）+ 沒有 object（狀態沒地方住 →
參數列爆炸、指標穿透）**。kernel 的三招解法：early return / `goto` 錯誤階梯 / **struct 打包狀態**。
這個練習是招 3，也是 Phase 2（資料結構）的入場券。

**還欠的債（想清就清，不清也不擋下一關）：**
- [ ] `01_my_wc` 4 個 style warning、`03_my_echo` 2 個（純風格：宣告後缺空行 ×3、單行 block 多餘大括號 ×3）
- [ ] 白紙 recall 兩題沒過，下次抽考會再問（答案已寫在
      `~/Documents/linux_kernel/wiki/daily/2026-04/day-31.md` 的「白紙 recall」段）：
      1. **為什麼不能 `fclose(stdin)`？**（關鍵字：所有權、fd 0 被 kernel 回收給下一個 open）
      2. **`if (ptr)` 這種 bug，為什麼 `-Wall -Wextra -Werror` 不警告？**
         （關鍵字：gcc 分不出 `if (!fp)` 和 `if (ptr)`；型別安全 ≠ 意圖安全）

**⚠️ 給下個 session 的行為提醒**：Ryan 的信心值跟正確率是**反過來**的 ——
他說「我寫對了」時通常沒跑過（今天連編譯都沒過）；他說「我在亂答」時通常正在真的推理（今天答對了）。
**任何「我懂了 / 我修好了 / 測試過了」一律先驗再信**，這是他本人要求的把關機制。

## Phase 1 — Coreutils（重寫 Unix 工具）
- [x] 01_my_wc      ✅ 已通關（2026-06-28）— ⚠️ 欠 4 個 style warning（通關時規矩還沒第 5 條）
- [x] 02_my_cat     ✅ 已通關（2026-07-13）— **四個裁判全過**（gcc / test / asan / checkpatch）
- [x] 03_my_echo    ✅ 已通關 — ⚠️ 欠 2 個 style warning（同上）
- [ ] 04_my_grep    ← **目前這關**
- [ ] 05_my_sort

## Phase 2 — 資料結構（手動 malloc/free）
> ⚠️ **「下一個做哪個」只看 `~/Documents/PRACTICE.md`**（2026-09-29 起的單一佇列；2026-10-01 已把 09_stack_queue 提到第 1）。下面 ①～⑦ 是 2026-08-23 的原始路線，只記勾選狀態，**不代表順序**。每站先猜題在 `phase2-data-structures/README.md`。
- [ ] ① 08_dynamic_array（resize / amortized）
- [x] ② 06_linked_list（單向）✅ 已通關（2026-09-29）— 四個裁判全過；bonus list_reverse 跳過。雙向/環狀＝**lab0-c**（已完成 7 個，下一個 q_delete_mid）
- [ ] ③ 09_stack_queue（陣列版 + 鏈結版各一，填對照表）
- [ ] ④ 10_bst（插入 / 刪除三情況 / 走訪）
- [ ] ⑤ 11_heap（陣列存，不用指標）
- [ ] ⑥ 07_hash_table（chaining）
- [ ] ⑦ 12_graph（鄰接串列 + BFS/DFS）

## Phase 3 — 系統工具（碰 syscall）
- [ ] 09_my_shell
- [ ] 10_my_hexdump

## Phase 4 — Kernel modules（進 kernel space）
- [ ] 11_hello_module
- [ ] 12_proc_info

## Phase 5 — Kernel 除錯（QEMU/VM 內）
- [ ] 13_kasan_uaf
- [ ] 14_kmemleak
- [ ] 15_kprobe_trace
- [ ] 16_ftrace_lab
- [ ] 17_panic_notifier

---

## 旁支練習（已碰過的基礎）
- c-basics/day-20：字串安全（bad_strcpy / bad_strncpy / **my_strscpy 自刻**）——對應 kernel 移除 strncpy() 的主題
- c-basics/day-21：把 strscpy 安全寫法套進 struct
- leetcode/set1-pointer-array：02_my_strcpy、03_my_strscpy
- wargame：lv0 ~ lv4（kernel module / oops 除錯）

## 通關規矩（每關）
1. `make` 零警告（`-Wall -Wextra -Werror`）— gcc 這關只證明「跑得對」
2. ~~valgrind 零 error、零 leak~~ → 本機 valgrind 跑不起來（Arch 把 ld.so strip、debuginfod 缺 bleeding-edge glibc 符號）。改用 `make asan`（ASan/LSan/UBSan）代驗，零報告即過。
3. 與系統工具 `diff` 比對輸出一致 — **且測資要真的蓋到邊界**（見下方「驗收清單本身會有洞」）
4. **`make style` 零 warning**（checkpatch，kernel coding style）← 2026-07-13 新增
5. 過了就 commit + push（remote: ryanGTR），並回來勾這份 TODO

### ⚠️ 驗收清單本身會有洞（2026-07-13 的教訓，讀完再開下一關）

規矩 4 是這天補的。補之前，01_my_wc 和 03_my_echo 都掛著「✅ 已通關」，
但從來沒被 checkpatch 看過 —— **不是它們沒過，是根本沒人問過這一題**。
清單漏一個維度，那個維度上的債就默默累積兩關才被發現。

同一天在 02_my_cat 上，同一個病發作了三次：

| # | 用了什麼指標 | 它說 | 真相 |
|---|---|---|---|
| 1 | `make test` 全綠 | 程式對了 | **錯的**。test.txt 每行都有 `\n`，測不到「檔尾無換行」那條路徑 |
| 2 | `make` 零 warning | 沒問題 | **style 有 1 個 warning**。gcc 不管 coding style |
| 3 | 從父 repo 跑 `git ls-files wiki/` 回 0 | wiki 沒進 git、有蒸發風險 | **錯的**。wiki 是巢狀 repo，有自己的 remote，父 repo 本來就不追蹤它 |

**通則：一個指標說 OK，只證明「它量的那件事」OK。**
綠燈不是證據，「有覆蓋到那條路徑」才是證據。新加測試要先確認它會**紅**，
否則你分不出「測試通過」和「測試根本沒測到」——一條從沒紅過的測試，
跟 `assertTrue(true)` 沒有區別。

## 心得 / 踩雷
- 01_my_wc：`while (ch = getc(fp) != EOF)` 少括號 → `!=` 優先序高於 `=`，ch 只拿到 0/1（Java 因 `==` 回傳 boolean 不會中招）。正解 `while ((ch = getc(fp)) != EOF)`。
- 01_my_wc：GNU wc 欄寬是**動態**的（= 最大數字 bytes 的位數），不是固定寬度；無檔名時結尾不留空格。
- 02_my_cat：**指標當條件恆為真**。`at_line_start` 改成 `int *` 後，`if (number && at_line_start)` 判斷的是「位址」不是「內容」，有效位址永不為 0 → 每個字元都被當行首編號。`-Wall -Wextra -Werror` **不會警告**，因為 `if (ptr)` 就是合法的 NULL 檢查慣用寫法（同一支程式的 `if (!fp)` 正是它）。編譯器擋型別錯，不擋「想要內容卻寫成地址」。Java 寫 `if (someObject)` 直接編譯錯誤，所以這個 bug 在 Java 不可能存在——手感救不了你。
- 02_my_cat：`-n` 的行號與行首旗標都是**串流層級**狀態，不是檔案層級。`lineno` 記得跨檔案傳，`at_line_start` 也要（漏了它 → 前一檔沒有換行結尾時，下一檔的第一個字元會被誤判成新行）。初值必須是 1（串流第一個字元本來就在行首）。
- 02_my_cat：`fopen` 失敗要 `fprintf(stderr, ...) + status = 1 + continue`，**不是 return**。GNU cat 會印錯誤到 stderr、繼續處理其餘檔案、最後才 exit 1。
- 02_my_cat：**綠燈不是證據，覆蓋率才是**。原 `make test` 全過但程式是錯的——測資（`test.txt`）每行都有換行結尾，測不到壞掉的路徑。新測試先確認會紅（red）再去修，否則你分不出「測試通過」和「測試根本沒測到」。
- 02_my_cat：對照組會被 shell alias 汙染。Ryan 的 `cat` 是 `bat --paging=never` 的 alias，缺檔時 exit code 回 **0**（GNU cat 回 1）、錯誤訊息格式也不同。Makefile 一律用 `CAT = /usr/bin/cat` 絕對路徑。
- 06_linked_list：**刪 head 是特例**。原版從 `head->next` 開始比，head 永遠比不到；改成先判 head 後，又 `return head`（剛被 free 的那顆）→ ASan 報 heap-use-after-free。頭換人就要回傳新頭，呼叫端要接住。
- 06_linked_list：`list_free_all` 用 `while (cur->next != NULL)` → 第一顆 free 兩次（double free）、最後一顆沒 free。條件要看 `cur` 本身；條件對了，開頭的 `if (!head)` 也跟著變多餘。
- 06_linked_list：**又一次綠燈不是證據**。`list_print` 是無窮迴圈（cur 只在 if 裡前進），但四個裁判全綠——因為 main.c 根本沒呼叫它。圖解在 `phase2-data-structures/06_linked_list/diagrams/`。
- 通用：本機 valgrind 無法用，記憶體一律走 `make asan`（見各專案 Makefile 的 asan target）。
