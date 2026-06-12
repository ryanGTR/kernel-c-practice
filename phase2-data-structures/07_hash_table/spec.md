# 07 - 雜湊表 (Hash Table with Chaining)

## 目標

實作一個使用鏈結法（chaining）處理碰撞的雜湊表。
每個 bucket 是一個 linked list。Key 為字串，Value 為整數。

參考資料：K&R 6.6 節。

## 需要實作的操作

| 函式 | 說明 |
|------|------|
| `ht_create(size)` | 建立指定大小的雜湊表 |
| `ht_put(ht, key, value)` | 插入鍵值對（key 已存在則更新 value） |
| `ht_get(ht, key, out_value)` | 查詢 key，成功回傳 0 並將值寫入 `out_value` |
| `ht_delete(ht, key)` | 刪除指定 key |
| `ht_destroy(ht)` | 釋放整個雜湊表（含所有節點和字串） |

## 訓練技能

- 雜湊函式設計
- 陣列 + 鏈結串列組合結構
- 字串複製（`strdup`）與記憶體所有權
- 理解「誰 malloc 的，誰就要 free」

## 工具

- `gcc` + `make`
- `valgrind` — 這題特別棘手：每次 `strdup` 都需要對應的 `free`
- `gdb` — 觀察碰撞時 bucket 的鏈結變化

## 提示

- 先從最簡單的雜湊函式開始：`所有字元加總 % size`
- K&R 6.6 有更好的版本（類似 djb2）
- `strdup` 會 `malloc` 一塊新記憶體 — 你有責任 `free` 它
- `ht_put` 遇到重複 key 時，要更新 value，不要插入第二份

## 完成標準

- [x] `make` 零警告編譯
- [x] 正確處理碰撞（多個 key hash 到同一個 bucket）
- [x] `valgrind` 無記憶體洩漏
- [ ] **加分**：當 load factor > 0.75 時自動 resize（rehash 所有元素）
