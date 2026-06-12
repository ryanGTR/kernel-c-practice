# Problem #04 — my_memcpy

> **Difficulty**: Easy | **Time**: 15 分鐘 | **Day**: GDB Day 24 | **Set 1**

## 🎯 題目

```c
void *my_memcpy(void *dst, const void *src, size_t n);
```

複製 `n` bytes 從 `src` 到 `dst`，回傳 `dst`。

⚠️ **不處理重疊**（重疊看 #05 memmove）。

## 🧪 Examples

```c
char dst[10];
my_memcpy(dst, "hello", 6);   // 含 \0 = 6 bytes

int a[3] = {1,2,3}, b[3];
my_memcpy(b, a, sizeof a);    // b = {1,2,3}
```

## 🛠️ Hints

```c
unsigned char *d = dst;
const unsigned char *s = src;
while (n--) *d++ = *s++;
return dst;
```

→ **必須 cast `unsigned char *`**。`void *` 不能 deref，且 byte-wise 處理才對。

## ⚠️ Edge cases
- [ ] `n == 0` → 不做事，回 dst
- [ ] dst == src → 等價於 no-op
- [ ] 重疊 → UB（你不處理）

## 📚 相關
- C API cheatsheet「記憶體」
- 對照 #05 memmove 學習重疊處理
