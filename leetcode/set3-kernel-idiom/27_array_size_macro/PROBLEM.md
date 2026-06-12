# Problem #27 — ARRAY_SIZE macro（含 __must_be_array）

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 36 | **Set 3**

## 🎯 題目

寫 `ARRAY_SIZE` macro，編譯期算陣列長度，**且傳 pointer 進去要編譯失敗**。

```c
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
```

## ⚠️ 為什麼要 `__must_be_array`？

普通版 `sizeof(arr) / sizeof((arr)[0])` 對 pointer 會悄悄錯：

```c
int arr[10];
int *p = arr;

ARRAY_SIZE(arr) == 10   ✅
ARRAY_SIZE(p)   == 2    ❌ (8 / 4 = 2，silently wrong)
```

→ kernel 加 `__must_be_array` 防呆：傳 pointer **編譯失敗**。

## 🛠️ Hints

用 GCC extension `__builtin_types_compatible_p`：

```c
#define __must_be_array(a) \
    BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(typeof(a), typeof(&(a)[0])))

/* 對陣列：typeof(a) = int[N]，typeof(&a[0]) = int*，不同 → 0 */
/* 對 pointer：typeof(p) = int*，typeof(&p[0]) = int*，相同 → 1 → BUILD_BUG_ON_ZERO 失敗 */

#define BUILD_BUG_ON_ZERO(e) ((int)(sizeof(struct { int:(-!!(e)); })))
/* e 真 → 負寬度 bit field → 編譯錯 */
/* e 假 → 寬度 0 → 沒事 */
```

`__must_be_array` 對陣列回 0（給 +0 給 ARRAY_SIZE 加進去無害）；對 pointer **編譯失敗**。

## 🧪 行為

```c
int arr[10];
size_t n = ARRAY_SIZE(arr);   /* 10，OK */

int *p = arr;
size_t m = ARRAY_SIZE(p);     /* ❌ compile error */
```

## 🐧 對應

`include/linux/kernel.h` / `include/linux/compiler.h`：
```c
#define ARRAY_SIZE(arr) \
    (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
```

→ 你 Day 36 後寫 kernel code 一律用 ARRAY_SIZE，不要寫 `n / sizeof(int)` 手算。
