# Problem #09 — count_words

> **Difficulty**: Easy | **Time**: 20 分鐘 | **Day**: 23 (my_wc) | **Set 1**

## 🎯 題目

```c
size_t count_words(const char *s);
```

數字串裡有多少個 word。**word = 連續非空白字元**，由一個或多個空白分隔。

## 🧪 Examples

```c
count_words("hello world") == 2
count_words("  multiple   spaces  ok  ") == 3
count_words("") == 0
count_words("   ") == 0
count_words("oneword") == 1
count_words("a b c") == 3
```

## 🛠️ Hints

state machine：
```c
int in_word = 0;
size_t count = 0;
for (; *s; s++) {
    if (isspace((unsigned char)*s)) {
        in_word = 0;
    } else if (!in_word) {
        in_word = 1;
        count++;
    }
}
return count;
```

→ 「進入 word 邊界時 count++」

## ⚠️ Edge cases
- [ ] 全空白 → 0
- [ ] 開頭/結尾有空白 → 不影響
- [ ] 只有一個 word → 1

## 🐧 對應
這是 `wc -w` 的核心邏輯，你 Day 23 寫 my_wc 會直接用。
