# merge-sort-list：單向串列的 merge sort

## 題目

把單向鏈結串列依 `key` 由小到大排序，要**穩定**、要 **O(n log n)**。拆成 3 個函式，**照順序做**：

| 順序 | 函式 | 做什麼 | 對應 |
|---|---|---|---|
| 1 | `merge_two` | 兩條已排好的串列合成一條 | LeetCode 21 |
| 2 | `split_half` | 從中間切成兩半 | 快慢指標（跟 LC 19、`q_delete_mid` 同招） |
| 3 | `merge_sort` | 用 1、2 遞迴排好整條 | LeetCode 148 |

```sh
make test   # 一開始 RED 4/16（空串列那幾題 stub 剛好會過），全綠＝過關
```

只改 `merge_sort.c`；規格在 `merge_sort.h`。不准 `malloc`／`free`，只能改 `next` 重新串。

測試輸出的 `5#2` 意思是 `key=5`、原本排第 `2` 個（`id`），用來看穩定性。

## 為什麼插隊

2026-10-28 做 lab0-c `q_sort` 前，Ryan 說「感覺到這裡應該要做一個獨立 merge sort」「我忘記太多了」。lab0-c 的版本同時要顧環狀雙向串列、`head`、`descend`，先在單向串列把演算法本身練熟，做完回 lab0-c `q_sort`。

## Java 你會怎麼寫

```java
Collections.sort(list);   // 內部是 TimSort：merge sort 家族，穩定
```

`Arrays.sort(Object[])` 用 TimSort、`Arrays.sort(int[])` 用 quick sort——物件要穩定，`int` 不用。

## 動手前先猜（先回答再寫）

| # | 問題 |
|---|------|
| 1 | `merge_two` 比大小用 `<=` 還是 `<`？哪一個才穩定？（測試 `stable_equal_keys`） |
| 2 | `merge_two` 迴圈跑完時，一定有一邊還剩節點。剩下的要怎麼接？需要再跑一個迴圈嗎？ |
| 3 | `split_half`：`1→2→3→4→5`，慢指標要停在哪個節點，才能切成 `1→2→3` 和 `4→5`？快指標從 `head` 還是 `head->next` 出發，差在哪？ |
| 4 | `split_half` 切完，第一半的尾巴要做什麼？忘了做會發生什麼事？ |
| 5 | `merge_sort` 的遞迴什麼時候停？0 個節點和 1 個節點都要停嗎？ |
| 6 | 為什麼是 O(n log n)？切幾層？每層合併總共碰幾個節點？ |

## 你以前卡過的點（先自己檢查）

| 弱點 | 這題會出現在哪 |
|------|------|
| 回傳值＝新的頭 | 三個函式都回傳頭，呼叫端要接住 |
| 快慢指標 off-by-one | `split_half` 奇數長度時第一半要多一個 |
| 改指標前先抓住 `next` | `merge_two` 接上一個節點後，原本那條的下一個在哪 |
| 該移動的指標寫成頭 | 移動的是你的游標，不是 `head` |
