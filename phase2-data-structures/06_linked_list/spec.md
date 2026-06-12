# 06 - 單向鏈結串列 (Singly Linked List)

## 目標

從零實作一個單向鏈結串列（不使用 kernel 的 `list.h`，那是進階練習）。
儲存 `int` 資料。

## 需要實作的操作

| 函式 | 說明 |
|------|------|
| `list_create()` | 建立空串列（回傳 head pointer） |
| `list_append(head, value)` | 在尾端新增節點 |
| `list_prepend(head, value)` | 在頭部新增節點 |
| `list_find(head, value)` | 尋找值，回傳該節點指標（或 NULL） |
| `list_delete(head, value)` | 刪除第一個符合的節點 |
| `list_print(head)` | 印出所有節點 |
| `list_free_all(head)` | 釋放整個串列 |

## 訓練技能

- `malloc` / `free` 記憶體管理
- 指標操作（pointer manipulation）
- NULL 檢查
- 走訪（traversal）

## 工具

- `gcc` + `make`
- `valgrind --leak-check=full` — 檢查記憶體洩漏
- `gdb` — 在 insert 設中斷點，逐步觀察指標變化
- Address Sanitizer (`-fsanitize=address`)

## 提示

- **先在紙上畫指標圖**。每個操作前後，node 之間的箭頭怎麼變？
- `delete` 需要追蹤「前一個節點」（previous pointer），否則無法重新接上鏈結。
- 注意刪除 head 節點的特殊情況。

## 完成標準

- [x] `make` 零警告編譯
- [x] 所有操作正確運作（append、prepend、find、delete、print、free）
- [x] `valgrind` 無記憶體洩漏
- [ ] **加分**：實作 `list_reverse()` 反轉串列
