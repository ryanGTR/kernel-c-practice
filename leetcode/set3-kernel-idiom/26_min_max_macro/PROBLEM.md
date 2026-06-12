# Problem #26 — type-safe min/max macro

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 36 | **Set 3**

## 🎯 題目

寫 type-safe 的 `min` / `max` macro。**避免常見 macro 坑**：
1. side-effect 重複求值（`min(i++, j++)` 算兩次）
2. 不同型別比較

```c
#define min(a, b) ...   /* a < b ? a : b，但安全 */
#define max(a, b) ...
```

## ⚠️ 經典壞 macro

```c
#define min(a, b) ((a) < (b) ? (a) : (b))   // ❌

min(i++, j++)
→ ((i++) < (j++) ? (i++) : (j++))   /* 加兩次！*/
```

## 🛠️ Hints（GCC extension）

用 GCC `__typeof__` + 區塊 expression `({ ... })`：

```c
#define min(a, b) ({                    \
    __typeof__(a) _a = (a);             \
    __typeof__(b) _b = (b);             \
    (void)(&_a == &_b);   /* 型別檢查：不同型別會 warning */ \
    _a < _b ? _a : _b;                  \
})
```

關鍵：
1. `_a = (a)` 只 evaluate 一次
2. `__typeof__` 抓型別
3. `(void)(&_a == &_b)` 編譯期檢查兩者是同型別（不同型別比較指標 → 編譯器警告）
4. `({ ... })` 是 GCC statement expression，可在 expression context 內
5. 變數加 `_` prefix 避免命名衝突（user 變數可能叫 `a`、`b`）

## 🧪 行為

```c
assert(min(3, 5) == 3);
assert(max(3, 5) == 5);

int i = 1, j = 2;
assert(min(i++, j++) == 1);   /* i++、j++ 都只應該執行一次 */
assert(i == 2 && j == 3);
```

## 🐧 對應

`include/linux/minmax.h`：
```c
#define min(x, y) ({                     \
    typeof(x) _min1 = (x);                \
    typeof(y) _min2 = (y);                \
    (void) (&_min1 == &_min2);            \
    _min1 < _min2 ? _min1 : _min2; })
```

→ 整個 kernel 都用這套。Day 36 必懂。
