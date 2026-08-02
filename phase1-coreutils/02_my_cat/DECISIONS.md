# my_cat 逐行決策標註

這份不是教你 cat 怎麼寫——**這份是把你自己寫過的 code 攤開，指出你當時做了哪些決定**。
0→1 難的原因不是語法，是不知道「這一行有得選」。看完這份，你會知道每一行背後有幾個選項。

用法：對照 `my_cat.c` 讀。寫 `my_grep` 時，同樣的決定點會再出現一次。

---

## 決策點 1：`static void cat_stream(...)` — 為什麼是 static

**選項**：`void cat_stream()`／`static void cat_stream()`

**你選的**：static。

**理由**：C 的函式預設是「整個程式都看得見」（external linkage）。加 `static` 讓它只在這個 `.c` 檔內可見。
**Java 類比**：不加 static ≈ `public`，加了 ≈ **package-private / private**。

**為什麼重要**：kernel code 幾乎所有非導出的函式都是 static。這是預設習慣，不是可選裝飾。
👉 **規則：除非別的檔案要呼叫它，一律 static。**

---

## 決策點 2：`int ch` 而不是 `char ch` — 這是最容易死人的一個

```c
int ch;
while ((ch = getc(fp)) != EOF) {
```

**選項**：`char ch`／`int ch`

**你選的**：`int`。**這個選對了，而且它是新手最常錯的地方。**

**理由**：`getc()` 回傳的是 **0–255 的字元值，或 EOF（通常是 -1）**——總共 257 種可能，`char` 裝不下。
用 `char` 會發生兩種災難：
- `char` 有號時，讀到 byte `0xFF` 會變成 -1，被誤判成 EOF → **檔案讀到一半就停**
- `char` 無號時，EOF 的 -1 會被截成 255 → **迴圈永遠不結束**

**Java 類比**：`InputStream.read()` 也是回傳 `int` 而不是 `byte`，理由一模一樣——要留一個「檔案結束」的哨兵值。你其實早就見過這個設計。

👉 **規則：所有 `getc` / `fgetc` / `getchar` 的接收變數，一律 `int`。**

---

## 決策點 3：讀取的粒度 — 為什麼是 `getc` 一次一個字元

**選項**：
| 方式 | 一次拿到 | 代價 |
|---|---|---|
| `getc()` | 一個字元 | 最簡單、不用管 buffer 大小；呼叫次數多但有 stdio 緩衝 |
| `fgets(buf, n, fp)` | 一行（但最多 n-1 字元） | 要自己開 buffer、**長行會被切斷**、要判斷有沒有讀到 `\n` |
| `getline(&p, &n, fp)` | 完整一行，自動配置 | GNU 擴充；**回傳的記憶體要你 free** |
| `read(fd, buf, n)` | 原始 bytes | syscall 層，沒有緩衝，要自己處理部分讀取 |

**你選的**：`getc`。

**理由**：cat 的工作是「原封不動搬」，不需要理解行的結構——只有 `-n` 需要知道換行在哪，而那用一個 `at_line_start` 旗標就解決了。
**選最笨的工具，因為問題不需要更聰明的。**

👉 **寫 my_grep 時這個決定會不一樣**：grep 要比對「一整行」，所以你會需要 `fgets` 或 `getline`。那時候就會撞上「長行怎麼辦」和「誰 free」——那才是下一關。

---

## 決策點 4：`long *lineno, int *at_line_start` — 為什麼傳指標

```c
static void cat_stream(FILE *fp, int number, long *lineno, int *at_line_start)
```

**選項**：傳值／傳指標／用 global 變數

**你選的**：傳指標。

**理由**：C **只有傳值**。函式裡改 `lineno` 的副本，外面看不到。要讓呼叫端看到改變，只能傳位址。

**Java 類比**（這裡最容易混淆）：Java 傳物件時你「感覺」是傳參照，其實傳的是**參照的複本**——所以你能改物件內容，但不能讓呼叫端的變數指向新物件。C 把這件事攤在檯面上：**要改呼叫端的東西，就明寫 `*`。**

**為什麼不用 global**：因為行號要跨檔案累加（`cat -n a.txt b.txt` 的編號是連續的），global 也做得到——但 global 會讓這個函式不能被重複使用、也不能測。
👉 **規則：狀態能用參數傳就別用 global。kernel 尤其如此（多執行緒）。**

---

## 決策點 5：`-n` 只認第一個位置 — 你做了一個「限制」的決定

```c
if (argc > 1 && strcmp(argv[1], "-n") == 0) {
	number = 1;
	start = 2;
}
```

**選項**：手寫只認 `argv[1]`／自己寫迴圈掃所有 `-` 開頭的參數／用 `getopt()`

**你選的**：最簡單的第一種。

**這是一個取捨，不是疏漏**——但你要知道自己放棄了什麼：`my_cat a.txt -n` 不會生效，`-n` 也不能和其他 flag 合併。

👉 真的 cat 用的是第三種。`getopt()` 是 POSIX 標準函式，處理 `-n`、`-abc` 合併、`--` 終止符。
**下一次做 my_grep（有 `-i`、`-n`、`-v` 多個 flag）時，就是導入 `getopt()` 的時機。** 一次只引入一個新東西，這次不學也對。

---

## 決策點 6：`start == argc` → 讀 stdin

```c
if (start == argc) {
	cat_stream(stdin, number, &lineno, &at_line_start);
	return 0;
}
```

**理由**：Unix 慣例——**沒給檔案就讀標準輸入**。這讓工具能接管線：`cat file | my_cat -n`。

**這不是 C 知識，是 Unix 工具的設計慣例**，而 coreutils 練習的一半價值就在學這些慣例。
👉 my_grep 同樣要遵守：`grep pattern`（沒給檔案）＝從 stdin 讀。

---

## 決策點 7：`"-"` 當成 stdin

```c
if (strcmp(argv[i], "-") == 0)
	fp = stdin;
```

同樣是慣例：`-` 代表標準輸入。**你有做，多數人第一次寫會漏。**

---

## 決策點 8：開檔失敗怎麼辦 — 這裡有三個決定疊在一起

```c
fp = fopen(argv[i], "r");
if (!fp) {
	fprintf(stderr, "my_cat: %s: %s\n", argv[i], strerror(errno));
	status = 1;
	continue;
}
```

| 決定 | 選項 | 你選的 | 為什麼 |
|---|---|---|---|
| 訊息印去哪 | `printf`（stdout）／`fprintf(stderr, ...)` | **stderr** | 錯誤訊息不能混進資料流——否則 `my_cat x > out.txt` 會把錯誤寫進 out.txt |
| 錯誤內容 | 自己編訊息／`strerror(errno)` | **strerror** | 系統給你精確原因（No such file / Permission denied），不用自己猜 |
| 遇錯的反應 | `exit(1)` 當場結束／記錄後 `continue` | **continue** | cat 的語意是「盡量做完」：一個檔案壞掉，其他照印，但**離開時回傳非 0** |

**Java 類比**：`errno` ≈ 一個 thread-local 的「上一個錯誤代碼」。C 沒有例外機制，函式用回傳值說「我失敗了」，用 `errno` 說「為什麼失敗」。
👉 **規則：檢查回傳值 → 立刻讀 errno（因為下一個呼叫就會蓋掉它）。**

---

## 決策點 9：`if (fp != stdin) fclose(fp);` — 資源歸屬

**這是全檔最重要的一行。**

**理由**：`stdin` 不是你開的，**所以不是你關的**。關掉它後面就再也讀不到。
你只 close 自己 `fopen` 出來的東西。

👉 **這就是 C 的記憶體／資源三原則第一條：誰配置，誰釋放。**
Java 有 GC 幫你收，C 沒有——所有權（ownership）必須在你腦裡有一張明確的表。
寫 my_grep 用到 `getline()` 時，這條會再考你一次：`getline` 幫你配置了記憶體，**但釋放是你的責任**。

---

## 決策點 10：`return status` 而不是 `return 0`

**理由**：exit code 是給**程式**看的，不是給人看的。`my_cat missing.txt && echo ok` —— 有錯就不該印 ok。
👉 **規則：任何一個檔案處理失敗 → exit code 非 0。**

---

## 你這支程式沒處理的（不是錯，是還沒到）

1. **寫入失敗**：`putchar` 也會失敗（磁碟滿、管線關閉）。真正的 cat 會檢查。
2. **`-n` 的位置限制**：見決策點 5。
3. **二進位檔**：目前逐字元搬，其實是對的；但若改成行處理就要小心 `\0`。

---

## 寫 my_grep 前，先回答這五題

同樣的決策點會換一個面貌出現。**先寫下你的答案，再開始寫 code：**

1. grep 要比對一整行 → 你選 `fgets` 還是 `getline`？各自的代價是什麼？（決策點 3）
2. 如果選 `getline`，配置出來的記憶體誰負責 free、在哪一行 free？（決策點 9）
3. `my_grep pattern`（沒給檔案）應該做什麼？（決策點 6）
4. 檔案開不起來時，印去哪、要不要中止、exit code 是什麼？（決策點 8）
5. 多個 flag（`-i` `-n` `-v`）——這次要不要用 `getopt()`？（決策點 5）

答得出這五題，`my_grep` 的骨架就已經在你腦裡了。剩下的只是敲鍵盤。
