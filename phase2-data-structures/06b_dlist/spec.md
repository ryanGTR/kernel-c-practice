# 06b - 雙向鏈結串列（教科書版）— 拿來跟 lab0-c 對照

## 為什麼有這一站

lab0-c 的 `queue.c` 是 Linux kernel 風格的鏈結串列：**侵入式＋環狀＋哨兵**。
三件事疊在一起，很難分清楚哪個特性造成哪個寫法（例如 `q_free` 裡 `safe` 為什麼最後會變成假元素）。

這一站寫**最普通的版本**，同樣的操作、同樣的測試情境，寫完兩份擺在一起比。

## 兩種版本對照（寫完再回來填右欄的「你的觀察」）

| 面向 | 教科書版（本站） | lab0-c / kernel `list.h` | 你的觀察 |
|---|---|---|---|
| 誰包誰 | `dnode` **包住**資料（`char *value`） | 資料 `element_t` **嵌入** `list_head` | |
| 兩端 | `head->prev == NULL`、`tail->next == NULL` | 環狀，繞回哨兵，**沒有 NULL** | |
| 空串列 | `head == tail == NULL` | `head->next == head` | |
| 串列本身 | `struct dlist { head; tail; }` | 一個不裝資料的哨兵 `list_head` | |
| 插入／刪除 | 要判斷「是不是第一個／最後一個」 | 永遠同一套 4 個指標 | |
| 走訪停止條件 | `p != NULL` | `p != head` | |
| 從節點拿資料 | `p->value` | `list_entry(p, element_t, list)->value` | |
| Java 類比 | `java.util.LinkedList`（Node 包物件） | 你的物件裡有一個 node 欄位 | |

## 需要實作的操作（`dlist.c`）

| 函式 | 說明 | lab0-c 對應 |
|------|------|---|
| `dl_new()` | 建立空串列 | `q_new` |
| `dl_free(l)` | 釋放全部（含每個 value）；`NULL` 要安全 | `q_free` |
| `dl_insert_head(l, s)` / `dl_insert_tail(l, s)` | 插入 `s` 的**副本** | `q_insert_head/tail` |
| `dl_remove_head(l)` / `dl_remove_tail(l)` | 拆下並回傳 value，**呼叫者負責 free** | `q_remove_head/tail` |
| `dl_size(l)` | 數節點 | `q_size` |
| `dl_delete_mid(l)` | 刪第 ⌊n/2⌋ 個（0 起算） | `q_delete_mid` |

## 開檔前先答（先猜再答）

1. `dl_insert_head` 在**空串列**和**非空串列**時，各要改哪幾個指標？lab0-c 的 `list_add` 需要分這兩種情況嗎？
2. `dl_remove_tail` 拆掉**最後一個**節點時，除了 `tail`，還有什麼一定要跟著變 `NULL`？
3. 🔥 刪中間節點時，教科書版可能遇到哪些「特殊位置」？哨兵版為什麼沒有？

## 裁判

| 指令 | 抓什麼 |
|---|---|
| `make test` | 行為：**每個檢查都正走（next）一次、反走（prev）一次**——忘記接 `prev` 會在反走時被抓 |
| `make asan` | leak、越界、use-after-free（valgrind 在這台壞掉） |
| `make style` | kernel coding style |

> 這組測試已驗過：空白版紅；參考實作綠且 ASan 乾淨；
> 故意「插頭時不接舊 head 的 prev」→ 反走 assert 失敗；故意「free 時漏 free value」→ LSan 報 23 bytes leaked。

## 完成標準

- [ ] `make test` 全綠
- [ ] `make asan` 零報告
- [ ] `make style` 零 warning
- [ ] 上面對照表右欄填完：兩個版本**最大的差別**用一句話講出來
