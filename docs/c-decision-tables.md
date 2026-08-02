# C 決策速查表

寫 C 卡住時，多半不是不會語法，是**不知道這裡有得選、也不知道選項各自的代價**。
這份就是那張缺的表。搭配 `phase1-coreutils/02_my_cat/DECISIONS.md` 讀。

---

## 表一：讀輸入的五種方式

| 方式 | 一次拿到 | 誰配置記憶體 | 長行怎麼辦 | 什麼時候用 |
|---|---|---|---|---|
| `getc(fp)` / `fgetc` | 一個字元（回傳 **int**） | 不用 | 無此問題 | **原封不動搬資料**（cat）、或只需要偵測換行 |
| `fgets(buf, n, fp)` | 一行，最多 `n-1` 字元 | 你（通常是 stack 陣列） | ⚠️ **被截斷且不報錯**，要自己檢查結尾有沒有 `\n` | 行長度有明確上限的場合 |
| `getline(&p, &n, fp)` | 完整一行，自動長大 | **它幫你 malloc，你負責 free** | 自動處理 | **要處理任意長度的行**（grep、sort）——POSIX 2008 起標準 |
| `fread(buf, sz, n, fp)` | 固定大小區塊 | 你 | 無行概念 | 二進位檔、大量搬運 |
| `read(fd, buf, n)` | 原始 bytes（syscall） | 你 | 無行概念 | **無緩衝**、要精確控制 syscall；⚠️ **可能只讀到一部分，要迴圈** |

**三條規則**
1. `getc` 的接收變數**一律 `int`**（EOF 需要第 257 個值）。
2. `fgets` 的回傳值要檢查是不是 `NULL`（EOF 或錯誤），且**不會告訴你行被截斷**——自己檢查最後一個字元是不是 `\n`。
3. `getline` 的 buffer **在迴圈外宣告、迴圈外 free**，中間讓它自己長大：
   ```c
   char *line = NULL;
   size_t cap = 0;
   while (getline(&line, &cap, fp) != -1) { ... }
   free(line);          /* 只 free 一次，不是每圈 */
   ```

---

## 表二：字串處理的邊界陷阱

| 你想做 | 危險寫法 | 該用 | 為什麼 |
|---|---|---|---|
| 複製字串 | `strcpy(dst, src)` | `snprintf(dst, sizeof dst, "%s", src)` | strcpy 不知道 dst 多大——最經典的溢位 |
| 「安全」複製 | `strncpy(dst, src, n)` | 同上 | ⚠️ **strncpy 在 src 太長時不會補 `\0`**，反而製造沒結尾的字串 |
| 串接 | `strcat` | `snprintf` 一次組完 | 同樣不知道空間夠不夠 |
| 求長度 | — | `strlen(s)` | ⚠️ **回傳值不含結尾的 `\0`**，配置空間要 `strlen(s) + 1` |
| 找子字串 | — | `strstr(hay, needle)` | 回傳**指向 hay 內部的指標**或 NULL；不要 free 它 |
| 切割 | `strtok` | 盡量避開 | ⚠️ **會改寫原字串**、且有內部狀態不可重入 |
| 比較 | `s1 == s2` | `strcmp(s1, s2) == 0` | `==` 比的是指標位址 |

**Java 類比**：Java 的 `String` 知道自己多長且不可變；C 的字串只是 **一段 bytes ＋ 一個結尾的 `\0` 約定**。
沒有人幫你檢查，長度是「數到 `\0` 為止」——**所有 C 字串 bug 的根源都是這一句。**

**最常見的 off-by-one**
```c
char buf[8];
strcpy(buf, "12345678");   /* 8 個字 + '\0' = 9 bytes → 溢位 */
```

---

## 表三：記憶體歸屬三原則

### 原則 1：誰配置，誰釋放

| 誰配置的 | 誰釋放 |
|---|---|
| 你 `malloc` / `calloc` / `strdup` | **你** `free` |
| `getline` 幫你配置 | **你** `free` |
| `fopen` 開的 FILE* | **你** `fclose` |
| `stdin` / `stdout` / `stderr` | **不是你開的 → 絕不 close** |
| `strstr` / `strchr` 回傳的指標 | 指向別人的記憶體 → **絕不 free** |

### 原則 2：三種生命週期，先想清楚東西該住哪

| 住哪 | 怎麼產生 | 活多久 | 陷阱 |
|---|---|---|---|
| **stack** | 區域變數、陣列 | **函式回傳就消失** | ⚠️ **回傳區域陣列的位址＝回傳垃圾**（最常見的初學者錯誤） |
| **heap** | `malloc` 家族 | 直到你 `free` | 忘了 free ＝洩漏；free 兩次＝崩潰 |
| **static / global** | `static` 變數、全域 | 整個程式 | 多執行緒共用會出事；kernel 尤其危險 |

```c
char *bad(void)  { char buf[64]; sprintf(buf, "hi"); return buf; }  /* ❌ buf 已消失 */
char *ok(void)   { return strdup("hi"); }                            /* ✅ 但呼叫端要 free */
```

### 原則 3：所有權要寫在文件上

C 沒有型別能表達「這個指標歸誰」，所以**用註解宣告**：

```c
/* 回傳的字串由呼叫端負責 free() */
char *build_message(const char *name);
```

kernel code 到處是這種註解，因為那是唯一的所有權標示方式。
👉 **Java 有 GC，所以你從來不用想這件事。C 的一半心智負擔就在這裡——這不是你笨，是這門語言把它交給你了。**

---

## 卡住時的四個提問（照順序問自己）

1. **這一行有幾個選項？** 翻上面三張表，先確認自己在做選擇，而不是在猜語法。
2. **我配置了什麼？誰負責釋放？** 每個 `malloc` / `fopen` / `getline` 都要找到它的對應。
3. **失敗會怎樣？** 每個回傳值都要問「它會不會是 NULL / -1 / EOF」。
4. **我放棄了什麼？** 選最笨的做法沒關係，但要知道代價（見 my_cat 決策點 5）。

---

## See Also

- `phase1-coreutils/02_my_cat/DECISIONS.md` — 用你自己寫過的 code 走一遍這些決定
- `~/Documents/linux_kernel/wiki/guides/c-mistake-pitfall.md` — 踩雷紀錄
