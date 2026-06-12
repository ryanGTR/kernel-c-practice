# Problem #07 — my_atoi

> **Difficulty**: Easy | **Time**: 20 分鐘 | **Day**: 23 (my_wc) | **Set 1**

## 🎯 題目

```c
int my_atoi(const char *s);
```

把 `s` 轉成 int。簡化版：
- 跳過前導空白
- 處理可選正負號 `+` / `-`
- 收 0-9 數字直到非數字
- 不檢查溢出（簡化版；真正 atoi 也不檢查）

## 🧪 Examples

```c
my_atoi("42") == 42
my_atoi("-42") == -42
my_atoi("  +123") == 123
my_atoi("abc") == 0
my_atoi("12abc") == 12
my_atoi("") == 0
```

## ⚠️ Edge cases

- [ ] 空字串 → 0
- [ ] 全空白 → 0
- [ ] 只有負號 → 0
- [ ] 數字後接非數字 → 取前面數字
- [ ] 多個正負號（"+-12"）→ 0（不合法）

## 🛠️ Hints

```c
const char *p = s;
while (isspace(*p)) p++;
int sign = 1;
if (*p == '+' || *p == '-') sign = (*p++ == '-') ? -1 : 1;
int n = 0;
while (isdigit(*p)) n = n * 10 + (*p++ - '0');
return sign * n;
```

⚠️ `isspace`/`isdigit` 收 `int`，傳 `char` 要 cast `(unsigned char)`，否則 negative 字元 UB。

## 📚 相關
- 進階版用 `strtol`（有錯誤檢查）
