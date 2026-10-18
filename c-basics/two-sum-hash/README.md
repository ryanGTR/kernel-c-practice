# two-sum-hash：沒排序的 Two Sum（自己做 hash table）

## 題目

LeetCode 1。`nums` **沒有排序**，找出 `i < j`，使 `nums[i] + nums[j] == target`，回傳**原本的 index**。

```sh
make test   # 一開始 RED 9/12，全綠＝過關（含 overflow 題，UBSan 會中止）
```

只改 `two_sum_hash.c`；測試在 `test_two_sum_hash.c`，編譯會開 ASan＋UBSan。

規則細節見 `two_sum_hash.h`：有多組答案時取 **j 最小**，同一個 j 再取 **i 最小**。

## 這題在考什麼

一句話：**用空間換時間——邊走邊問「我要的另一半，之前出現過嗎？」**

Java 你會這樣寫：

```java
Map<Integer, Integer> seen = new HashMap<>();   // 值 → index
for (int j = 0; j < n; j++) {
    Integer i = seen.get(target - nums[j]);
    if (i != null) return new int[] { i, j };
    seen.putIfAbsent(nums[j], j);
}
```

C 沒有 `HashMap`，所以這題真正的功課是**自己做一個**：

| Java 幫你做的事 | 你在 C 要自己寫 |
|---|---|
| `new HashMap<>()` 自動擴容 | `ht_init`：一次配好容量（≥ 2n 的 2 的次方），不擴容 |
| `hashCode()` ＋ 找桶子 | `hash(key) & mask`（已給 `hash()`） |
| 碰撞用 linked list／紅黑樹 | **open addressing＋linear probing**：位子被占就往下一格找 |
| `get()` 回傳 `null` | `ht_find` 回 `false`；值用 `int *val` 帶出去 |
| GC 回收 | `ht_free`；所有 return 路徑都要 free（ASan 會抓 leak） |

## 動手前先猜（先回答再寫）

| # | 問題 |
|---|------|
| 1 | `{1, 2, 3}`、target 4：如果先 `insert(nums[j])` 再 `find(target - nums[j])`，`j = 1` 會發生什麼？（測試 `self_twice`） |
| 2 | `{1, 1, 1, 5}`、target 6：重複的 key 要覆蓋還是保留第一個？各會回傳哪個 i？（測試 `first_i`） |
| 3 | `ht_find` 的 probing 迴圈，**停下來的條件**有哪兩種？為什麼容量 ≥ 2n 保證它一定會停？ |
| 4 | `target - nums[j]` 在 `{INT_MIN, 3, -2}`、target 1 時會怎樣？怎麼算才不是 UB？ |
| 5 | `mask` 為什麼是「容量 − 1」？容量不是 2 的次方會怎樣？ |

## 重要的點

| 重點 | 說明 |
|------|------|
| 先查再插 | 順序反了，同一個元素會和自己配對 |
| `used[]` 陣列 | key 可以是 0 或負數，不能拿某個值當「空位」記號 |
| 溢位 | 補數用 `long long` 算，超出 `int` 範圍就不可能在表裡 |
| `big` 測試 | n = 200000；暴力 O(n²) 約 2×10¹⁰ 次比較，跑不完＝逼你用 hash |
| 複雜度 | 平均 O(n) 時間、O(n) 空間；對照 `../two-sum` 是排序後 O(n)、O(1) 空間但 index 會遺失 |

## Kernel 裡的 hash table

| 位置 | 重點 |
|------|------|
| `include/linux/hashtable.h:16 (v7.0)` `DEFINE_HASHTABLE` | 固定大小、碰撞用 `hlist`（串列），和本題的 open addressing 相反 |
| `include/linux/hash.h:65 (v7.0)` `hash_32()` | 乘黃金比例常數 `GOLDEN_RATIO_32`（:41）再 `>> (32 - bits)` 取**高位**（:68）。本題 `hash()` 同樣是乘法，但改成把高位 XOR 到低位，讓你可以直接 `& mask` |

## See Also

- `../two-sum`：已排序版（LC 167，two pointer）
- `../three-sum`：固定一個數＋two-sum
