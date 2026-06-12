# Problem #08 — reverse_string

> **Difficulty**: Easy | **Time**: 15 分鐘 | **Day**: GDB Day 23 | **Set 1**

## 🎯 題目

```c
void reverse_string(char *s);
```

**in-place** 反轉字串（不能配新 buffer）。

## 🧪 Examples

```c
char a[] = "hello"; reverse_string(a); // "olleh"
char b[] = "a";     reverse_string(b); // "a"
char c[] = "";      reverse_string(c); // ""
char d[] = "abcd";  reverse_string(d); // "dcba"
```

## 🛠️ Hints

雙指標 swap：
```c
size_t len = strlen(s);
char *l = s, *r = s + len - 1;
while (l < r) {
    char t = *l; *l = *r; *r = t;
    l++; r--;
}
```

## ⚠️ Edge cases
- [ ] 空字串
- [ ] 單字元
- [ ] 長度為 0/1 不要做 swap（pointer 算術會出錯）

## 📚 LeetCode 對應
LeetCode #344 Reverse String（用 char[] 而非 string，正合本題）。
