# Problem #28 — BUILD_BUG_ON（編譯期斷言）

> **Difficulty**: Hard | **Time**: 30 分鐘 | **Day**: 36 | **Set 3**

## 🎯 題目

實作 `BUILD_BUG_ON(cond)` macro：**`cond` 為真時編譯失敗**。

```c
BUILD_BUG_ON(sizeof(struct foo) > 64);   /* 編譯期確認 struct 不超過 cache line */
BUILD_BUG_ON(N != 16);                    /* 編譯期參數驗證 */
```

## ⚠️ 為什麼不用 `assert()`?

`assert()` 是 **runtime** 檢查——程式跑到那行才會炸。
`BUILD_BUG_ON` 是 **compile time** 檢查——條件不成立直接無法編譯，省了 runtime 成本，且**所有 binary 都保證**滿足條件。

## 🛠️ 經典做法（C89/C99）

技巧：用「**負寬度 bit field**」觸發編譯錯：

```c
#define BUILD_BUG_ON(cond) \
    ((void)sizeof(char[1 - 2*!!(cond)]))

/* cond 假 → !!(cond) = 0 → char[1] OK */
/* cond 真 → !!(cond) = 1 → char[-1]，編譯錯 */
```

## C11 替代方案

C11 引入 `_Static_assert`：

```c
_Static_assert(sizeof(int) == 4, "int is not 4 bytes");
```

更乾淨。但 kernel 因相容老編譯器，仍用 BUILD_BUG_ON 風格。

## 🧪 行為

```c
BUILD_BUG_ON(0);                      /* 過 */
BUILD_BUG_ON(sizeof(int) == 4 && 0);  /* 過 */

/* 取消下面註解編譯應該失敗 */
/* BUILD_BUG_ON(1); */
/* BUILD_BUG_ON(sizeof(struct foo) > 1024); */
```

## 🐧 對應

Linux `include/linux/build_bug.h`：

```c
#define BUILD_BUG_ON_ZERO(e) ((int)(sizeof(struct { int:(-!!(e)); })))
#define BUILD_BUG_ON(condition) \
    ((void)BUILD_BUG_ON_ZERO(condition))
```

整個 kernel 用它做編譯期 invariant check。
