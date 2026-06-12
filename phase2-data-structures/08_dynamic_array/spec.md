# 08 - 動態陣列 (Dynamic Array)

## 目標

實作一個動態陣列（類似 C++ 的 `vector` 或 Python 的 `list`）。
儲存 `int` 資料。初始容量為 4，滿了就加倍。

## 需要實作的操作

| 函式 | 說明 |
|------|------|
| `dynarray_create()` | 建立動態陣列（初始容量 4） |
| `dynarray_push_back(da, value)` | 在尾端加入元素 |
| `dynarray_get(da, index)` | 取得指定 index 的值 |
| `dynarray_set(da, index, value)` | 設定指定 index 的值 |
| `dynarray_pop_back(da)` | 移除並回傳最後一個元素 |
| `dynarray_size(da)` | 回傳目前元素數量 |
| `dynarray_destroy(da)` | 釋放動態陣列 |

## 訓練技能

- `realloc` 策略（倍增法）
- `capacity` vs `size` 的區別
- 邊界檢查（bounds checking）

## 工具

- `gcc` + `make`
- `valgrind` — 確認 realloc 不會造成洩漏
- `assert()` — 用於邊界檢查

## 提示

- 分清楚 `size`（目前有多少元素）和 `capacity`（分配了多少空間）
- `realloc` 可能失敗 — 要檢查回傳值！
- 如果 `realloc` 失敗，**不要覆蓋原本的指標**，否則會遺失資料且造成記憶體洩漏
- `pop_back` 在空陣列時要處理（可以 assert 或回傳錯誤）

## 完成標準

- [x] `make` 零警告編譯
- [x] 容量增長正確運作（push 超過 4 個元素後自動擴充）
- [x] `valgrind` 無記憶體洩漏
- [ ] **加分**：實作 `dynarray_insert_at(da, index, value)` 和 `dynarray_remove_at(da, index)`
