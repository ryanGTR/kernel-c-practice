# Problem #06 — my_strdup

> **Difficulty**: Easy | **Time**: 15 分鐘 | **Day**: 29 | **Set 1**

## 🎯 題目

```c
char *my_strdup(const char *s);
```

回傳 `s` 的 heap 副本。失敗回 NULL。**caller 負責 free**。

## 🧪 Examples

```c
char *p = my_strdup("hello");
assert(strcmp(p, "hello") == 0);
free(p);

char *empty = my_strdup("");
assert(empty[0] == '\0');
free(empty);
```

## 🛠️ Hints

```c
size_t len = strlen(s);
char *p = malloc(len + 1);   /* +1 for \0 */
if (!p) return NULL;
memcpy(p, s, len + 1);
return p;
```

## ⚠️ Edge cases

- [ ] 空字串 → malloc(1) + `\0`
- [ ] malloc 失敗 → return NULL
- [ ] s == NULL → UB（不檢查）或回 NULL（你決定）

## 🐧 對應

POSIX 內建。kernel 對應是 `kstrdup(s, GFP_KERNEL)`。
