# Problem #01 — my_strlen

> **Difficulty**: Easy | **Time**: 15 分鐘 | **Day**: 20 | **Set 1**

## 🎯 題目

實作 `strlen` 不靠 libc：

```c
size_t my_strlen(const char *s);
```

回傳 `s` 的長度（不含 `\0`）。

## 🧪 Examples

```c
my_strlen("hello") == 5
my_strlen("") == 0
my_strlen("a") == 1
```

## ⚠️ Edge cases

- [ ] 空字串 → 0
- [ ] 只有 `\0` → 0
- [ ] `s == NULL` → 你決定（一般 UB 不檢查；安全版可回 0 或 abort）

## 🛠️ Hints

- pointer 走訪：`while (*s) s++; return s - start;`
- 或 index：`size_t i = 0; while (s[i]) i++; return i;`
- 進階：libc 真正版本用 word-wise SIMD，先做 byte-wise 版

## 📚 相關
- K&R §5.5 字串
- `wiki/tools/c-api-cheatsheet.md` strxxx
