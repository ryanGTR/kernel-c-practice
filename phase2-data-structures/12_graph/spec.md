# 12 - Graph（鄰接串列）+ BFS / DFS

## 目標

用鄰接串列（adjacency list）存圖，寫 BFS 與 DFS（遞迴版 + 顯式 stack 版）。
這站是前面所有站的組合題：鏈結串列（adj）+ queue（BFS）+ stack（DFS）+ 陣列（visited）。
Java 對應：`Map<Integer, List<Integer>>` 或 `List<List<Integer>>`。

## 需要實作的操作

| 函式 | 說明 |
|------|------|
| `graph_create(n)` | n 個頂點，`adj` 用 `calloc` |
| `graph_add_edge(g, u, v)` | 有向邊 u→v，插在 `adj[u]` 頭部 O(1) |
| `graph_add_undirected(g, u, v)` | 兩條有向邊 |
| `graph_has_edge / graph_degree` | 走 `adj[u]` |
| `graph_bfs(g, start, order)` | 回傳到達頂點數，順序寫進 `order` |
| `graph_dfs(g, start, order)` | 遞迴 |
| `graph_dfs_iter(g, start, order)` | 顯式 stack |
| `graph_destroy` | 每條邊、`adj`、struct |

## 訓練技能

- `struct edge **adj`：指標的指標——一個「串列頭的陣列」。畫出來。
- visited 陣列：`calloc(n, sizeof(int))` 為什麼比 `malloc` 好？
- BFS 的 queue 可以只是 `int q[n]` + `head/tail`，不用 ring buffer——**為什麼**？（每個頂點最多入隊一次）
- 遞迴 DFS 在 10 萬個頂點的鏈狀圖會怎樣？這就是為什麼要會寫 `dfs_iter`。

## 陷阱

- BFS：**入隊時**就標 visited，不是出隊時。否則同一個頂點可能被入隊多次（測試的 4 有兩條路進來）。
- 無向圖：`has_edge(0,1)` 和 `has_edge(1,0)` 都要 true，`degree` 要算對。
- 測試不規定 DFS 的確切順序（只驗「不重複、起點在前、可達集合正確」），因為鄰接串列的插入順序會影響走訪順序——這是合法的自由度，不是 bug。
- `destroy`：三層釋放（edge → adj → graph），漏一層 LSan 就報。

## 完成標準

- [ ] `make` 零警告
- [ ] `make test` 全過
- [ ] `make asan` 零報告
- [ ] `make style` 零 warning
- [ ] 口頭答：BFS 為什麼能給最短路徑（以邊數計）而 DFS 不行？鄰接串列 vs 鄰接矩陣，什麼時候該換？
- [ ] **加分**：`graph_bfs_dist`、`graph_has_cycle`（三色 DFS）、`graph_components`

## 驗收 LeetCode（C，不看答案）

- LC 200 Number of Islands（grid 當圖，BFS 或 DFS）
- LC 133 Clone Graph、LC 207 Course Schedule（有向圖找環 = 加分題）
- LC 127 Word Ladder（BFS 最短路徑）
