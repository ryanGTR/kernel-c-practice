# Problem #02 — my_strcpy

> **Difficulty**: Easy | **Time**: 15 分鐘 | **Day**: 20 | **Set 1**

## 🎯 題目

```c
char *my_strcpy(char *dst, const char *src);
```

把 `src`（含 `\0`）複製到 `dst`，回傳 `dst`。

⚠️ **真實 strcpy 是危險函式**（不檢查長度）。這題練「pointer 同步走訪」基本功。下一題（#03）是安全版。

## 🧪 Examples

```c
char buf[10];
my_strcpy(buf, "hello");   // buf == "hello"
my_strcpy(buf, "");         // buf[0] == '\0'
```

## ⚠️ Edge cases

- [ ] 空字串：dst 第一個 byte 設 `\0`
- [ ] dst 太小：UB（你不檢查，因為簽名沒 size）
- [ ] dst 跟 src 重疊：UB（也不檢查）

## 🛠️ Hints

```c
char *p = dst;
while ((*p++ = *s++) != '\0') ;   /* C 經典寫法 */
return dst;
```

或更易讀的版本：
```c
size_t i = 0;
do { dst[i] = src[i]; } while (src[i++] != '\0');
```

## 📚 相關
- K&R §5.5
- C API cheatsheet 「字串」表格
