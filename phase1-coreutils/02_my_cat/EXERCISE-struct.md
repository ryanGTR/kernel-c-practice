# 延伸練習 — 用 struct 打包狀態

> 建立：2026-07-13（`02_my_cat` 通關當天）
> 預估：15 分鐘
> 前置：`02_my_cat` 已通關（`make` / `make test` / `make asan` / `make style` 全過）

**這份自足。不需要記憶、不需要前一個 session 的上下文，照著做即可。**

---

## 為什麼做這個

起點是 Ryan 的一個問題：

> 「為什麼 C 語言巢狀判斷看起來比其他語言難？Java 沒有這件事的感覺，我都能在很小範圍處理。」

答案是兩件 C 沒有、Java 有的東西：

| Java 有 | C 沒有 | 後果 |
|---|---|---|
| **exception** | 錯誤沒有出口 | 每個可能失敗的呼叫都要**就地 `if`** → 縮排往右長 |
| **object（field）** | 狀態沒地方住 | 只能**當參數傳**或開全域 → 參數列爆炸、指標穿透 |

Linus 對第一件事的立場寫在 `Documentation/process/coding-style.rst:34`：

> "...if you need more than 3 levels of indentation, you're screwed anyway,
>  and should fix your program."

**縮排太深在 kernel 是「程式有問題」，不是風格見仁見智。** 8 字元 Tab 就是故意的——縮排太深時你會被推出 80 欄，逼你重寫。

kernel 的三招解法：

1. **Guard clause / early return** —— 錯的先擋掉，happy path 留在最外層
2. **`goto` 錯誤階梯**（`coding-style.rst:526`，第 7 節 "Centralized exiting of functions"）—— C 版的 `try-finally`，倒序解構，清理集中在底部
3. **struct 打包狀態** —— C 的「物件」：沒有方法，但至少狀態有地方住

**這份練習是招 3。** 它同時是 Phase 2（資料結構）的入場券——那三關全部都在玩 struct + 指標。

---

## 這個練習要治的東西

現在的 `cat_stream` 簽名：

```c
static void cat_stream(FILE *fp, int number, long *lineno, int *at_line_start)
```

四個參數，但**只有 `fp` 是「這個檔案」的事**。另外三個（`number` / `lineno` / `at_line_start`）
都是**整條串流**的狀態——它們必須跨檔案存活，所以被迫用指標傳。

而 2026-07-13 那個 bug 正是這麼來的：

```c
if (number && at_line_start)      /* ❌ 判斷的是「位址」，恆為真 */
if (number && *at_line_start)     /* ✅ 判斷的是「內容」 */
```

在 Java 這個 bug **不可能存在**——`atLineStart` 是個 field，`if (atLineStart)` 直接讀值，
沒有 `*` 可以漏。C 把「狀態住哪裡」這個問題丟回給你，於是它變成指標，於是你有機會寫錯。

**struct 不會讓 C 長出 GC，但它把狀態從「散落的指標參數」變回「一個有名字的東西」。**
`ctx->at_line_start` 讀起來就是「內容」，你不會再把它跟指標本身搞混。

---

## 任務

把 `my_cat.c` 改成 struct 版。要求：

1. 定義一個 context struct，把三個串流狀態收進去：

   ```c
   struct cat_ctx {
           long lineno;
           int at_line_start;
           int number;
   };
   ```

2. `cat_stream` 的簽名縮成**兩個參數**：

   ```c
   static void cat_stream(FILE *fp, struct cat_ctx *ctx)
   ```

3. `main` 裡初始化一次 ctx，然後整條串流共用它。
   > ⚠️ `at_line_start` 的初值是 **1**，不是 0——串流的第一個字元本來就在行首。

4. 行為**完全不變**。這是純重構，不改任何對外行為。

---

## 驗收（四個裁判，缺一不可）

```bash
cd ~/Documents/kernel-c-practice/phase1-coreutils/02_my_cat

make          # gcc：零 warning（-Wall -Wextra -Werror）
make test     # 與 /usr/bin/cat 行為完全一致，含所有邊界
make asan     # ASan/LSan/UBSan：零報告
make style    # checkpatch：零 warning
```

**四個都過才算完成。** 這是 2026-07-13 學到的：一個裁判說 OK，只證明「它量的那件事」OK。

> 💡 **對照組一律用 `/usr/bin/cat`（絕對路徑）。**
> 本機 shell 的 `cat` 是 `bat --paging=never` 的 alias，缺檔時 exit code 回 **0**
> 而不是 GNU cat 的 **1**，錯誤訊息格式也不同——拿它當「正確答案」等於用錯的尺量。
> `Makefile` 裡已經用 `CAT = /usr/bin/cat` 綁死了，但你手動比對時要自己記得。

重構完，`git diff` 應該只動 `my_cat.c`，`make test` 的輸出一個字都不變。

---

## 想清楚再動手（先答再寫）

1. `number` 該不該進 struct？它跟 `lineno` / `at_line_start` 有什麼不同？
   （提示：哪一個在整條串流跑完之前**不會變**？把它跟會變的東西放同一個 struct，是好還是壞？
   兩種答案都可以，但你要說得出理由。）

2. `cat_stream` 拿到的是 `struct cat_ctx *ctx`。那 `ctx->at_line_start` 和 `(*ctx).at_line_start`
   是同一件事嗎？`->` 到底是什麼？

3. 如果 struct 改成傳值（`struct cat_ctx ctx` 而不是 `struct cat_ctx *ctx`），會發生什麼事？
   （這題直指 2026-07-13 那個 bug 的本質——先猜，再改成傳值跑跑看 `make test`。
   **看到它壞掉**比看到它會動更有價值。）

---

## 做完之後

- 這關的 checklist 已經全綠，這份是**加分題**，不影響 `02_my_cat` 的通關狀態。
- 手感帶去 **Phase 2 `06_linked_list`**——那關開始，struct + 指標就是主角，不再是配角。
- 想更進一步：招 2（`goto` 錯誤階梯）在 `my_cat` 用不太到（沒有多重資源要清），
  但 Phase 2 的 `07_hash_table`（malloc 失敗要回收已配置的部分）會**逼你**用上。到時再練。
