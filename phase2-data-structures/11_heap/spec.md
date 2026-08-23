# 11 - Heap（陣列存，不用指標）

## 目標

實作 binary min-heap。**整棵樹住在一個 `int` 陣列裡**，父子關係用 index 算出來，
一個指標都沒有。前三站都在接指標，這站要你換腦袋：結構可以是「隱含的」。
Java 對應：`PriorityQueue`（底層就是這個）。

## 需要實作的操作

| 函式 | 說明 |
|------|------|
| `heap_create()` | 初始容量 4 |
| `heap_push(h, v)` | 放尾端 → `sift_up` |
| `heap_pop(h, &out)` | 取 `data[0]`，最後一個補到 0 → `sift_down` |
| `heap_peek / heap_size / heap_destroy` | |
| `sift_up / sift_down` | `static`，這兩個是全部的核心 |

## 索引公式（寫在紙上驗三個節點再動手）

```
parent(i) = (i - 1) / 2        left(i) = 2i + 1        right(i) = 2i + 2
```

思考：為什麼 0-based 是 `2i+1`，1-based 是 `2i`？哪個在 C 比較自然？kernel 的 `lib/min_heap.h` 用哪種？

## 訓練技能

- 把「樹」想成「陣列的一種讀法」：同一塊記憶體，換公式就是不同結構。
- `sift_down` 的邊界：右子不一定存在；只剩左子時別讀 `data[2i+2]`（ASan 會抓 heap-buffer-overflow）。
- 陣列 heap 為什麼比指標樹 cache 友善？（下一站 cachelab 會真量）

## 陷阱

- `sift_down` 選「較小的子」再比自己——不是先比左再比右。
- pop 後 `size--` 要在 `sift_down` 之前，否則會把已移出的元素當成合法節點。
- 重複值：heap 允許（測試有 1000 個元素塞 101 種值）。

## 完成標準

- [ ] `make` 零警告
- [ ] `make test` 全過
- [ ] `make asan` 零報告
- [ ] `make style` 零 warning
- [ ] 口頭答：push 和 pop 各 O(?)；為什麼 heap 只能拿 min，不能 O(log n) 找任意值？
- [ ] **加分**：`heap_build` O(n)（並說出為什麼不是 O(n log n)）、`heap_sort`、`heap_is_valid`

## 驗收 LeetCode（C，不看答案；C 沒有內建 PriorityQueue，正好用自己的）

- LC 215 Kth Largest Element、LC 347 Top K Frequent（配 12 的 hash）、LC 23 Merge k Sorted Lists（配 lab0-c 的 `q_merge`）
