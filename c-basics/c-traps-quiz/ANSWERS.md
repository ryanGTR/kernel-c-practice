# 標準答案

全部用 gcc（x86-64 Linux）`-Wall -Wextra` 跑過，2026-11-10。

| # | 書 § | 程式 | 答案 | 為什麼 | -Wall 抓得到？ |
|---|---|---|---|---|---|
| 01 | 1.3 | `a = b---c;`（b=5, c=2） | `a=3 b=4 c=2` | 斷詞是貪婪的：`b-- - c` | 否 |
| 02 | 1.5 | `sizeof('a')`、`sizeof("a")` | `4 2` | C 的字元常數是 int；字串多一個 `'\0'` | 否 |
| 03 | 2.2 | `if (flags & FLAG != 0)`（都是 4） | `not set` | `!=` 比 `&` 先算：`4 & (4 != 0)` = `4 & 1` = 0 | 是（-Wparentheses） |
| 04 | 2.2/4.9 | `hi << 4 + lo`（hi=1, lo=2） | `64` | `+` 比 `<<` 先算：`1 << 6` | 是（-Wparentheses） |
| 05 | 2.3 | `if (x > max); max = x;` | `3` | 分號是空敘述，`max = x` 一定執行 | 是（-Wempty-body） |
| 06 | 2.4 | switch n=2 沒有 break | `two three other` | case 只是入口，沒 break 會一路往下 | 是（-Wimplicit-fallthrough） |
| 07 | 2.6 | 外 if 不成立，else 縮排在外層 | 只印 `end` | else 跟最近的 if 配對，不看縮排 | 是（-Wdangling-else） |
| 08 | 4.2 | `1 & 2`、`1 && 2` | `0 1` | `&` 逐位元（01 & 10 = 0）；`&&` 只看真假 | 否 |
| 09 | 4.5 | `sizeof(a)` 在 main 和在 `f(int a[10])` | `40` / `8` | 陣列當參數會退化成指標 | 是（-Wsizeof-array-argument） |
| 10 | 7.3/5.1 | `char c = 200; printf c; c == 200` | `-56` / `0` | x86 的 char 是 signed，200 存不下變成 -56 | 部分（-Wtype-limits） |
| 11 | 7.5 | `-7 / 2`、`-7 % 2` | `-3 -1` | C99 起除法向 0 截斷，餘數跟被除數同號 | 否 |
| 12 | 6.1 | `abs(a-b)`（沒括號的 macro）、`max(i++, j)` | `r1=-8`；`r2=2 i=3` | 展開成 `a-b>0 ? a-b : -a-b`；`i++` 被算兩次 | 否 |
| 13 | 6.2 | `#define IPTR int *` 然後 `IPTR a, b;` | `8 4` | 展開成 `int *a, b;`，b 是 int。要用 typedef | 否 |
| 14 | 4.8 | `strlen(s) > i`（s="hi", i=-1） | `shorter` | size_t 是 unsigned，-1 被轉成超大數 | 是（-Wsign-compare，在 -Wextra） |
| 15 | 4.5/4.7 | `malloc(strlen(s) + strlen(t))` 接字串 | 照常印 `abcd`，但寫出界 1 byte | 少了 `+ 1` 給 `'\0'`；ASan 報 heap-buffer-overflow | 否（要 ASan） |
| 16 | 7.8 | `for (p = head; p; p = p->next) free(p);` | crash：`free(): invalid pointer` | free 之後還讀 `p->next`＝use-after-free；要先抓 next | 是（-Wuse-after-free） |
