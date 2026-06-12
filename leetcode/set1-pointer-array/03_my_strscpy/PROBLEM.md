# Problem #03 — my_strscpy（kernel 安全字串複製）

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 20 | **Set 1**

## 🎯 題目

實作 kernel 用的 `strscpy`：

```c
ssize_t my_strscpy(char *dst, const char *src, size_t size);
```

行為：
- 把 `src` 複製到 `dst`，**最多** `size - 1` bytes（留 1 byte 給 `\0`）
- **永遠** `\0` 結尾
- 回傳：成功 = 寫入的 byte 數（不含 `\0`）；src 太長 = `-E2BIG`（用 `-7`）

## 🧪 Examples

```c
char buf[10];
my_strscpy(buf, "hello", 10) == 5    // buf="hello\0"
my_strscpy(buf, "hello", 3)  == -7   // 太長，buf="he\0"（仍 \0 結尾）
my_strscpy(buf, "", 10)      == 0    // 空字串
my_strscpy(buf, "exact", 6)  == 5    // 剛好
```

## ⚠️ Edge cases

- [ ] `size == 0` → 不寫任何東西，回 `-E2BIG`（對應 kernel 行為）
- [ ] 空 src → buf[0]='\0'，回 0
- [ ] src 剛好等於 size-1 → 成功
- [ ] src 比 size 長 → 截斷 + `\0` + 回負

## 🛠️ Hints

對照 [Linux kernel `lib/string.c`](https://elixir.bootlin.com/linux/latest/source/lib/string.c) 的 `strscpy()`：

```c
if (size == 0) return -E2BIG;   /* -7 */
size_t i = 0;
while (i < size - 1 && src[i]) {
    dst[i] = src[i];
    i++;
}
dst[i] = '\0';
return src[i] ? -E2BIG : (ssize_t)i;
```

## 🐧 Kernel 對應

`strscpy` 是 Linux kernel 推薦的字串複製函式（`strncpy` 跟 `strcpy` 都不安全）。
源碼：`lib/string.c` 的 `strscpy()`。**寫 driver 用這個**。

## 📚 相關
- C API cheatsheet 強烈標 ⭐
- 對照 `strncpy`（不保證 \0）跟 `strcpy`（不檢查長度）的差異
