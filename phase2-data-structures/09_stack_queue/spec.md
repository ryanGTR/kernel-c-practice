# 09 - Stack / Queue（陣列版 vs 鏈結串列版）

## 目標

同一個介面各寫兩種儲存方式，共四個結構，然後**自己填對照表**。
這站的重點不是寫出來（都很短），是寫完後能說出「為什麼某種情況該選哪個」。

| 結構 | 陣列版 | 鏈結串列版 | Java 對應 |
|------|--------|-----------|-----------|
| Stack | `astack_*`（動態陣列，尾端 push/pop） | `lstack_*`（頭部 push/pop） | `ArrayDeque` / `LinkedList` |
| Queue | `aqueue_*`（**ring buffer**） | `lqueue_*`（head + tail 指標） | `ArrayDeque` / `LinkedList` |

## 需要實作的操作

每個結構：`create / push|enqueue / pop|dequeue / peek / size / destroy`。
回傳值慣例：成功 0、失敗 -1（空結構 pop、配置失敗）；值經 `int *out` 帶出。
`destroy(NULL)` 必須安全（kernel 的 `kfree(NULL)` 慣例）。

## 訓練技能

- **ring buffer**：`head`、`size`、`% capacity`。dequeue 不搬資料。
- ring buffer **擴容時不能直接 realloc**——元素可能繞過尾端，要按順序抄進新 buffer。
- 鏈結 queue 的 tail 指標：dequeue 到空時 tail 要歸 NULL（測試會抓）。
- 單一介面多種實作 → 對照 Java 的 `interface Deque<E>`。

## 陷阱（測試專門抓這些）

- `aqueue`：先塞 4、取 2、再塞到 50 → head 不在 0 時擴容，順序要對。
- `lqueue`：enqueue 1 → dequeue → 再 enqueue 2：tail 指到已 free 的節點 = UAF，ASan 會報。
- `lstack_destroy`：free 前先存 `next`，否則讀到已 free 的記憶體。

## 完成後填這張表（寫進本檔）

| 比較項 | 陣列版 | 鏈結版 |
|--------|--------|--------|
| push/enqueue 時間 | | |
| 最差單次（擴容那次） | | |
| 每個元素額外記憶體 | | |
| cache 友善度（為什麼） | | |
| 什麼情況選它 | | |

## 完成標準

- [ ] `make` 零警告
- [ ] `make test` 全過（一開始對 stub 是紅的，這是故意的）
- [ ] `make asan` 零報告
- [ ] `make style` 零 warning
- [ ] 上表填完，並口頭回答：Linux kernel 的 `kfifo` 是哪一種？它為什麼要求 capacity 是 2 的冪？
- [ ] **加分**：`aqueue` 的 `% capacity` 改成 `& (capacity - 1)`，說出前提條件

## 與 lab0-c 的關係

lab0-c 的 `q_insert_head/q_remove_head` 就是 stack、`q_insert_tail/q_remove_head` 就是 queue，
底層是雙向環狀串列（本站鏈結版的最終型態）。這站寫完，lab0-c Day 1–2 應該是順手的事。
