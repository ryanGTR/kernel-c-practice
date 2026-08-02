# Kata — 一天一個零件，五分鐘一圈

> 型（kata）不是每天學新招，是同一招打到變成反射。
> 這裡的 10 個零件，**全部是 `04_my_grep` 和 `05_my_sort` 的拆解件**。
> 你不是在偏離主線，是在把主線的零件先拆下來練熟。

建立於 2026-07-14。緣由：「寫太少程式了感覺陌生」——陌生是**頻率**問題，不是難度問題。

---

## 為什麼是 TDD

C 在**這一層**（user-space 純函式）完全適用 TDD，因為三個前提都成立：

| TDD 的前提 | 這裡 | kernel 那層 |
|---|---|---|
| 回饋迴圈要快 | `gcc` 1 秒 ✅ | 重編 + QEMU 重開機，分鐘級 ❌ |
| 單元要能隔離 | 純函式，零依賴 ✅ | 依賴全域 context（中斷？持鎖？）❌ |
| assert 點要明確 | 有回傳值 ✅ | 副作用、race、leak，assert 不出來 ❌ |

所以：**測試已經幫你寫好了，實作是空的。** 你的迴圈就是紅 → 綠。

---

## 規矩

1. **一天一題，照順序，不准跳。** 10 天。跳題等於又在畫地圖。
2. 每題五步，**一步都不能少**：

   ```
   ① 答檔頭的 Q      不准查資料。答錯沒關係，空著才有關係。
   ② 畫記憶體圖      檔頭有位子。畫不出來 = 你還不知道要寫什麼。
   ③ make N=xx       先看它紅。沒紅過的測試 = assertTrue(true)。
   ④ 寫實作 → 再 make N=xx    紅 → 綠。
   ⑤ make asan / style / gdb  見下表——不同的 bug 要不同的裁判。
   ```

3. **卡超過 15 分鐘就停**，記下卡在哪，回報。不要硬撐（硬撐會變成「今天沒寫」）。

---

## 四個裁判，四種盲區

這是這 10 題真正要你帶走的東西：

| 裁判 | 指令 | 抓什麼 | **抓不到什麼** |
|---|---|---|---|
| gcc | `make N=xx` | 型別錯 | **意圖錯**（day-31：`if (ptr)` vs `if (*ptr)`） |
| assert | `make N=xx` | 行為錯 | **記憶體越界**（kata 07：讀 `line[-1]` 照樣全綠） |
| ASan/LSan | `make asan N=xx` | 記憶體錯、leak | 邏輯、風格 |
| checkpatch | `make style N=xx` | kernel 風格 | 你的意圖（**會誤報** —— kata 10 見） |

**沒有任何一個裁判能單獨證明你是對的。** 這句話比這 10 題本身值錢。

有幾題是故意設計成「`make` 綠、`make asan` 紅」的（07、08、09）。
綠燈不是證據——這是 day-31 用血換來的，這裡讓你再撞三次。

---

## 指令

```bash
make N=05          # 編譯 + 跑（你的紅/綠迴圈）
make asan N=05     # ASan/LSan/UBSan（本機 valgrind 是壞的）
make style N=05    # checkpatch
make gdb N=05      # 直接斷在被測函式上
make list          # 看題目
make clean
```

---

## 題目

| # | 零件 | 練什麼 | 給誰用 |
|---|---|---|---|
| 01 | `my_strlen` | 指標走訪 | 基礎 |
| 02 | `my_strcpy` | 寫進別人的 buffer（**所有權**） | 基礎 |
| 03 | `my_strrev` | 雙指標、原地修改 | 基礎 |
| 04 | `str_tolower` | `(unsigned char)` cast 陷阱 | `my_grep -i` |
| 05 | `my_strstr` | 子字串搜尋、退回重試 | **`my_grep` 的心臟** |
| 06 | `ci_strstr` | 零複製 + 回傳原始指標 | **`my_grep -i` 的真零件** |
| 07 | `chomp` | 去行尾 `\n`；**空字串越界陷阱** | `my_grep` / `my_cat` |
| 08 | `my_strdup` | 第一次 `malloc`：**所有權轉移** | `my_sort` |
| 09 | 動態陣列 | `malloc`/`realloc`/`free` 兩層 heap | **`my_sort` 的心臟** |
| 10 | 函式指標排序 | `cmp(&a[i], ...)` 那個 `&` | **`my_sort` 的魔王** |

做完 01–10，`my_grep` 和 `my_sort` 你已經有全部零件了，剩下只是組裝。

---

## 回報格式（每題）

```
kata NN 完成
Q 答對幾題 / 共幾題:
記憶體圖 vs gdb 實際: 幾處不一樣   <- 這個數字最重要，那是你的心智模型破洞數
make 紅 → 綠 花了幾分鐘:
asan 有沒有抓到東西:
卡在哪:
```

---

## 這些檔案我驗過了

10 題的參考解全部：`gcc -Wall -Wextra -Werror` 零 warning、測試全過、ASan 乾淨、checkpatch 零 warning（kata 10 的 3 個誤報除外，已在檔頭說明）。
空白版全部：編得過、跑起來紅。
07/08/09 宣稱「make 綠但 asan 紅」的部分，已用故意寫壞的實作實測確認：
`stack-buffer-underflow` / `heap-buffer-overflow` / `145 bytes leaked`。

day-31 的教訓：**沒跑過的骨架不是骨架，是猜測。** 不會再犯第二次。
