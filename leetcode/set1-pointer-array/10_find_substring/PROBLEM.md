# Problem #10 — find_substring

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 25 (my_grep) | **Set 1**

## 🎯 題目

```c
const char *find_substring(const char *haystack, const char *needle);
```

在 `haystack` 找 `needle` 第一次出現的位置。
- 找到 → 回傳那個位置（haystack 中的指標）
- 沒找到 → 回傳 NULL
- needle 是空字串 → 回傳 haystack（任何位置）

跟 libc `strstr` 行為一致。

## 🧪 Examples

```c
find_substring("hello world", "world") -> "world"
find_substring("hello", "x") -> NULL
find_substring("aaa", "aa") -> "aaa" (第一個位置)
find_substring("abc", "") -> "abc"
find_substring("", "x") -> NULL
find_substring("abc", "abcd") -> NULL  (needle 比 haystack 長)
```

## 🛠️ Hints

簡單版 O(n*m)：
```c
if (!*needle) return (char *)haystack;
for (; *haystack; haystack++) {
    const char *h = haystack, *n = needle;
    while (*h && *n && *h == *n) { h++; n++; }
    if (!*n) return (char *)haystack;   /* needle 走完 = 匹配 */
}
return NULL;
```

進階：KMP / Boyer-Moore O(n+m)，先做簡單版。

## ⚠️ Edge cases
- [ ] needle == ""
- [ ] needle 比 haystack 長
- [ ] needle 在 haystack 開頭/結尾
- [ ] needle 跟 haystack 完全相等

## 🐧 對應
你 Day 25 寫 my_grep 會用到這個。kernel 對應 `strstr` (lib/string.c)。
