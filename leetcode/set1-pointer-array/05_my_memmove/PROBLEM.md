# Problem #05 — my_memmove（處理重疊）

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: GDB Day 24 | **Set 1**

## 🎯 題目

```c
void *my_memmove(void *dst, const void *src, size_t n);
```

跟 `memcpy` 一樣，但**正確處理重疊區域**。

## 🧪 重疊範例（這就是難點）

```c
char buf[] = "abcdef";
my_memmove(buf + 1, buf, 5);     // dst 在 src 後面，重疊
// buf 應該變 "aabcde"

char buf2[] = "abcdef";
my_memmove(buf2, buf2 + 1, 5);   // dst 在 src 前面，重疊
// buf2 應該變 "bcdef" + 'f'（位置 5）= "bcdeff"
```

→ 想像「把字串往右搬一格」跟「往左搬一格」結果完全不同。

## 🧪 Examples

```c
char buf[] = "abcdef";
my_memmove(buf + 1, buf, 5);   // "aabcde"

char buf2[] = "abcdef";
my_memmove(buf2, buf2 + 1, 5); // "bcdeff"

char dst[10] = "xxx";
my_memmove(dst, "hi", 3);      // dst="hi" + 多個 x（沒重疊，跟 memcpy 一樣）
```

## 🛠️ Hints

判斷方向：
```c
if (dst < src) {
    /* 從前面往後搬：用 memcpy 邏輯就對 */
    while (n--) *d++ = *s++;
} else if (dst > src) {
    /* dst 在 src 後：要從後面往前搬，避免覆蓋還沒搬的 byte */
    d += n; s += n;
    while (n--) *--d = *--s;
} 
/* dst == src: no-op */
```

→ **方向錯了會把資料撞壞**。

## ⚠️ Edge cases
- [ ] `n == 0`
- [ ] dst == src
- [ ] dst < src
- [ ] dst > src（重疊或不重疊）

## 🐧 為什麼 kernel 在意？

DMA buffer、ring buffer 操作常需要 in-place 移動。`memcpy` 在這場景會悄悄毀資料。**規矩：不確定是否重疊就用 memmove**。
