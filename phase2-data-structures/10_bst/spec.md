# 10 - 二元樹 → BST（插入、刪除三種情況、走訪）

## 目標

從零實作 binary search tree，儲存 `int` key。不平衡（AVL / 紅黑留給之後的 kernel `rbtree`）。
Java 對應：`TreeMap` 的底層是紅黑樹，但 API 形狀（`put / get / remove / firstKey / lastKey`）一樣。

## 需要實作的操作

| 函式 | 說明 |
|------|------|
| `bst_insert(root, key)` | 插入，回傳新 root；重複 key 忽略 |
| `bst_find(root, key)` | 找到回傳節點，否則 NULL（**用迴圈寫**） |
| `bst_delete(root, key)` | 刪除，回傳新 root；**三種情況** |
| `bst_min / bst_max` | 最左 / 最右 |
| `bst_height / bst_count` | 空樹 0 |
| `bst_inorder / preorder / postorder` | 走訪，callback 帶 `ctx` |
| `bst_free` | 釋放整棵樹 |

## 刪除三種情況（這站的核心）

1. **葉節點**：直接 free。
2. **一個子節點**：把子節點接給父節點，free 自己。
3. **兩個子節點**：找右子樹最小（in-order successor），把它的 key 抄進來，
   然後到右子樹刪掉那個 successor（它最多只有一個子節點 → 回到情況 1/2）。

「回傳新 root」的寫法讓你不用傳 `struct bst_node **`；寫完可以試試雙指標版本，
那是 kernel `rbtree` 的 `rb_link_node(node, parent, &parent->rb_left)` 的思路。

## 訓練技能

- 遞迴在 C 的真實代價：每層一個 stack frame。退化樹（依序插 1..100000）會不會 stack overflow？
- callback + `void *ctx`：C 沒有 closure，這是 kernel 到處在用的模式（`list_sort` 的 `cmp`、`kthread` 的 `data`）。
- 釋放順序：為什麼 `bst_free` 必須是 post-order？

## 陷阱

- `bst_is_valid` 的經典錯答：只檢查「左子 < 自己 < 右子」。
  `50 → left 30 → right 60` 局部都對但 60 跑到 50 的左邊了。要傳 `(lo, hi)` 範圍下去。
- 情況 3 刪 root 時 root 本身的 key 會變（測試檢查 `root->key == 60`）。

## 完成標準

- [ ] `make` 零警告
- [ ] `make test` 全過
- [ ] `make asan` 零報告（刪到空樹不能漏任何節點）
- [ ] `make style` 零 warning
- [ ] 口頭答：in-order 走訪為什麼會排序？刪除情況 3 為什麼選 successor 而不是隨便一個右子樹節點？
- [ ] **加分**：`bst_is_valid`、`bst_inorder_iter`（顯式 stack，不遞迴）

## 驗收 LeetCode（C，不看答案）

- LC 700 Search in a BST、LC 701 Insert into a BST、LC 450 Delete Node in a BST
- LC 98 Validate BST（就是上面那個陷阱）
- LC 94 Binary Tree Inorder Traversal（迭代版）
