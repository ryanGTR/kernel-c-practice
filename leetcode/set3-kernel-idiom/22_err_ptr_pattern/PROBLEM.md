# Problem #22 — ERR_PTR / IS_ERR / PTR_ERR

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 38 | **Set 3**

## 🎯 題目

Kernel 用「**pointer 編碼 errno**」回傳「pointer or error」：

```c
#define MAX_ERRNO 4095   /* errno 不會超過這個 */

static inline void *ERR_PTR(long error)        /* 把負 errno 當 pointer 回傳 */
static inline long  PTR_ERR(const void *ptr)   /* 從假 pointer 取出 errno */
static inline int   IS_ERR(const void *ptr)    /* 判斷 ptr 是不是錯誤碼 */
```

## 🛠️ 原理

最高位 4096 個位址永遠不會是合法 pointer（OS 不會配那邊）。
所以可以**把小負數塞進 pointer 空間**：

```c
ERR_PTR(-ENOMEM)  →  0xFFFFFFFFFFFFFFF4   (-12 cast 成 unsigned)
IS_ERR(0xFFFFFFFFFFFFFFF4) → true，因為 >= -MAX_ERRNO（很大的 unsigned）
PTR_ERR(0xFFFFFFFFFFFFFFF4) → -12
```

## 🧪 行為

```c
struct foo *open_foo(int n) {
    if (n < 0) return ERR_PTR(-EINVAL);
    if (out_of_memory) return ERR_PTR(-ENOMEM);
    struct foo *f = malloc(sizeof(*f));
    if (!f) return ERR_PTR(-ENOMEM);
    return f;
}

/* Caller */
struct foo *f = open_foo(-1);
if (IS_ERR(f)) {
    printf("error: %ld\n", PTR_ERR(f));
    return -1;
}
/* 用 f */
```

## 🛠️ Hints

```c
#define MAX_ERRNO 4095

static inline void *ERR_PTR(long error) {
    return (void *)error;
}

static inline long PTR_ERR(const void *ptr) {
    return (long)ptr;
}

static inline int IS_ERR(const void *ptr) {
    return (unsigned long)ptr >= (unsigned long)-MAX_ERRNO;
}
```

→ 關鍵：`(unsigned long)-MAX_ERRNO` 是個很大的數字（約 `0xFFFFFFFFFFFFF001`）。所有負 errno cast 成 unsigned 都比它大。

## 🐧 對應

`include/linux/err.h` 完整實作。`fs/`、`drivers/` 隨處可見。**取代 NULL + errno 全域變數的方式**——回傳值自帶錯誤資訊。

⚠️ 不要把這 pattern 用在沒研究過的 userspace code，會讓 reader 困惑。**它是 kernel 慣用法**。
