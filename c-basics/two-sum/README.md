# two-sum：成對數字加總（two pointer）

## 題目

`nums` 已由小到大排序（LeetCode 167）。找出 `i < j`，使 `nums[i] + nums[j] == target`。

```sh
make test   # 一開始 RED 5/8，全綠＝過關（含 overflow 題，UBSan 會中止）
```

只改 `two_sum.c`；測試在 `test_two_sum.c`，編譯會開 ASan＋UBSan。

## 這題在考什麼

一句話：**利用「已排序」這個性質，每比較一次就整批淘汰候選答案。**

- 暴力法要看全部 n² 組配對。可以想成一張 n×n 的表格，每格是一組 `(i, j)`。
- `nums[l] + nums[r] < target`：`nums[l]` 配上最大的數都還不夠，它所在的**整列**都可以刪掉，所以 `l++`。
- `nums[l] + nums[r] > target`：`nums[r]` 配上最小的數都已經太大，它所在的**整行**都可以刪掉，所以 `r--`。
- 每一步刪掉一整列或一整行，最多走 n 步，時間 O(n)、額外空間 O(1)。

O(n²) 降到 O(n) 是結果。真正要練的是：**證明被淘汰的東西不可能是答案。**
只要寫得出這個證明，題目換了也知道 two pointer 能不能用。

## 重要的點

| 重點 | 說明 |
|------|------|
| 前提是單調性（monotonicity，單調性） | 沒排序就不能淘汰。排序要多花 O(n log n)，而且原本的 index 會遺失 |
| 迴圈條件 `l < r` | 不能是 `<=`，同一個元素不能用兩次 |
| 溢位 | `nums[l] + nums[r]` 在 int 邊界可能 overflow，C 的 signed overflow 是 UB；UBSan 會抓到 |
| 要回傳原 index 就改用 hash | LeetCode 1：O(n) 時間、O(n) 空間，Java 類比是 `HashMap<Integer, Integer>` 存「值 → index」 |

## 變形題

### A. 對撞指標（兩端往中間收）

| 題目 | 跟本題的差別 |
|------|--------------|
| LC 1 Two Sum | 沒排序、要原 index，改用 hash |
| LC 15 3Sum | 排序後固定一個數，剩下的跑本題，O(n²)；難點是去重 |
| LC 16 3Sum Closest | 同上，改成追蹤最接近的總和 |
| LC 18 4Sum | 兩層固定，再跑本題 |
| LC 11 Container With Most Water | 淘汰的理由要自己證：移短的那根才可能變大 |
| LC 977 Squares of a Sorted Array | 負數平方後變大，從兩端比大小 |
| LC 125 Valid Palindrome | 兩端比對字元 |

### B. 同向指標／快慢指標

| 題目 | 備註 |
|------|------|
| LC 26 Remove Duplicates from Sorted Array | 讀指標、寫指標 |
| LC 283 Move Zeroes | 同上 |
| LC 876 / 2095 Middle / Delete Middle Node | **= lab0-c `q_delete_mid`**（`queue.c:137`） |
| LC 141 Linked List Cycle | Floyd 快慢指標 |

### C. 滑動視窗（同向指標加上視窗內的狀態）

| 題目 | 備註 |
|------|------|
| LC 209 Minimum Size Subarray Sum | 前提是全為正數（又是單調性） |
| LC 3 Longest Substring Without Repeating Characters | 視窗內放 set |

### 建議順序

167（本題）→ 15 → 11 → `q_delete_mid` → 209

## Kernel 裡的雙指標

| 位置 | 型態 | 重點 |
|------|------|------|
| `lib/list_sort.c:13 (v7.0)` `merge()` | 兩條已排序串列同向前進 | 每步挑小的輸出；`tail` 是 `**`，指向「下一個要寫的 next 欄位」，所以 head 不用特判 |
| `include/linux/kfifo.h:50 (v7.0)` `in` / `out` | 讀寫索引同向追趕 | `lib/kfifo.c:20` 用 `in - out` 算已用量，靠的是 **unsigned** 繞回（有定義），對照本題 **signed** 溢位是 UB |

本題和 merge 的差別：Two Sum 在一條陣列上兩端對撞，每步淘汰一個；merge 在兩條串列上同向前進，每步挑一個。兩者都靠「已排序」，才能只看當前兩個數就做決定。

## See Also

- `~/Documents/lab0-c/queue.c`：`q_delete_mid`、`q_merge` 都用得到雙指標
