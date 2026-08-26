# Phase 2 — 資料結構刻一遍（2026-08-23 路線圖）

Ryan 2026-08-23 定的順序，對應到本目錄與 lab0-c。**不開新 repo**；一天一格 25 分鐘，照號碼走。

## 七站對照表

| 站 | Ryan 的順序 | 位置 | Java 類比 | 開檔前先答（先猜再答） |
|---|---|---|---|---|
| 1 | 動態陣列（自己寫 resize，理解 amortized） | `08_dynamic_array/` | `ArrayList` | 容量 4 起加倍，push 100 次共搬幾個元素？+4 版呢？（答：124 vs 1200） |
| 2a | 單向鏈結串列 | `06_linked_list/` | 自刻 `Node` | 刪除節點為什麼要追蹤 prev？刪 head 有什麼特別？ |
| 2b | 雙向 → 環狀（哨兵節點） | **lab0-c** `queue.c`（Day 1–2） | `LinkedList` 內部 | `list_add` 對只有 head 的空環各寫哪四個指標？ |
| 3 | Stack / Queue（陣列版＋鏈結版各一次） | `09_stack_queue/`；lab0-c 的 `q_insert_head/tail + q_remove_head` 就是 | `ArrayDeque` vs `LinkedList` | ring buffer 的 dequeue 為什麼不搬資料？擴容時為什麼不能直接 realloc？ |
| 4 | 二元樹 → BST（插入、刪除三情況、走訪） | `10_bst/` | `TreeMap`（少了平衡） | 刪有兩個子節點的節點，為什麼拿右子樹最小值來補？ |
| 5 | Heap（陣列存，不用指標） | `11_heap/` | `PriorityQueue` | index i 的父、左子、右子是哪三個式子？pop 後為什麼要把最後一個搬到 0？ |
| 6 | Hash Table（chaining） | `07_hash_table/` | `HashMap`（Java 8 前的純 chaining 版） | load factor 是什麼？什麼時候該 resize？resize 為什麼要全部重新 hash？ |
| 7 | Graph（鄰接串列）+ BFS/DFS | `12_graph/` | `List<List<Integer>>` | BFS 要在入隊時還是出隊時標 visited？差在哪？ |

站 2b 與站 3 的鏈結版**就是 lab0-c 的格子**，不是另一條線。lab0-c 8/19 派的 Day 1 在站 1 做完後接著做。

## 每格的四個裁判（每站 `Makefile` 都一樣）

```
make         # gcc -Wall -Wextra -Werror：跑得對
make test    # 對 stub 一開始是紅的——故意的。綠燈不是證據，「曾經紅過」才是
make asan    # ASan/LSan/UBSan 零報告（本機 valgrind 跑不起來，見 ../TODO.md）
make style   # checkpatch 零 warning
```

09–12 的 `main.c` 已寫好會紅的測試；06–08 的 `main.c` 是 TODO，測試要自己寫（寫測試也是練習）。
四個裁判全過 → 回答 `spec.md` 底下的口頭題 → commit + push → 勾 `../TODO.md`。

## 每站寫完要留下的東西

- `spec.md` 底下的對照表 / 口頭題答案（寫進檔案，不是口頭帶過）。
- 一行「預測 vs 實際」：開寫前先猜會卡在哪，寫完對帳。

## 參考書：`data-structure-in-c`（2026-08-25 掛上）

本機路徑 `~/Documents/data-structure-in-c`（fork 自 **ksw2000/Data-Structure-in-C**，MIT 授權，內容 2022 年）。
**不併進本 repo、不複製程式碼**——它是別人寫好的完整實作，等於**答案本**。

### ⚠️ 使用規則（只有一條，但很重要）

> **四個裁判全過之後才准打開。** 卡住時可以看，但看之前先在 `spec.md` 寫下「我卡在哪、我猜是什麼問題」。
>
> 理由：本 phase 的設計是「`make test` 一開始是紅的，**綠燈不是證據，曾經紅過才是**」。
> 開著答案寫，紅燈就不會出現，整套設計失效——那時累積的是「看過」不是「會」。

### 七站對照

| 站 | 本目錄 | 參考檔案 | 覆蓋 |
|---|---|---|---|
| 1 動態陣列 | `08_dynamic_array/` | `Queue/05-動態陣列實作佇列.c`（resize 在這裡）；`Array/` 只有二維宣告，**不是** amortized 那題 | ⚠️ 部分 |
| 2a 單向鏈結 | `06_linked_list/` | `Linked-List/03-insert.c`、`04-delete.c`、`05-invert.c`、`06-concatenate.c` | ✅ |
| 2b 雙向→環狀 | **lab0-c** `queue.c` | `Linked-List/07-insertFront-circular.c`、`08-length-circular.c`（環狀有，**雙向沒有**——lab0-c 的 `list.h` 才是雙向環狀） | ⚠️ 部分 |
| 3 Stack / Queue | `09_stack_queue/` | `Stack/01,02`（陣列）、`Stack/03`（鏈結）；`Queue/01,02`（陣列）、`Queue/03`（環狀）、`Queue/04`（鏈結） | ✅ 陣列版＋鏈結版各一，正好對上「各一次」 |
| 4 二元樹→BST | `10_bst/` | `Tree/Binary-Search-Tree.c`；延伸 `AVL-Tree.c`、`Red-Black-Tree.c`（**站 4 不做平衡，留到讀 kernel rbtree 時再看**） | ✅ |
| 5 Heap | `11_heap/` | `Heap/01-simple-max-heap.c` | ✅ |
| 6 Hash Table | `07_hash_table/` | **無** | ❌ |
| 7 Graph + BFS/DFS | `12_graph/` | **無**（`Stack/04-堆疊解迷宮問題.c` 有 DFS 的味道，但不是圖的表示法） | ❌ |

**站 6、站 7 沒有參考**——這兩站只能自己寫，或另找材料。先知道比寫到一半才發現好。

### 不在七站、但可併到別條線的

| 資料夾 | 併到哪 | 備註 |
|---|---|---|
| `Pointer/` | `../c-basics/` | 取址取值、指標指向大塊記憶體、陣列名即位址 |
| `Struct/`、`Union/`、`Enum/` | `../c-basics/` | 自訂型別；`04-結構體指標.c` 對後面每一站都有用 |
| `String/` | `../c-basics/` | 自刻 `strlen`／`strcpy`／`strcmp`／`strcat`——**這批很值得自己先寫再對照** |
| `Sorting/` | **lab0-c** Day 5–6 | 五種排序；`merge-sort.c` 正對 lab0-c 的 `q_sort`（穩定、對鏈結串列友善） |
| `C語言概念澄清.md` | 隨時 | 概念釐清 |

### 授權與出處

上游 `ksw2000/Data-Structure-in-C`，MIT。**只讀不併**；若日後真要引用片段，保留 MIT 授權聲明與出處。

---

## 之後（不展開）

這 7 站是 CSAPP 線 lab0-c → datalab → … 的前置，也是 kernel 容器（`list.h` / `rbtree` / `min_heap.h` / `hashtable.h`）的對照組：
自己刻過一次，再讀 kernel 版才看得出它為什麼那樣設計（侵入式、`container_of`）。
