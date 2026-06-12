# Problem #21 — container_of

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 36 | **Set 3**

## 🎯 題目

實作 kernel 經典 macro：

```c
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
```

給你某 struct 的 member 指標，反推整個 struct 的起點。

## 🧪 行為

```c
struct foo {
    int x;
    char buf[16];
    int y;
};

struct foo f = { .x = 1, .y = 99 };
int *py = &f.y;

struct foo *p = container_of(py, struct foo, y);
assert(p == &f);
assert(p->x == 1);
```

## 🛠️ Hints

`offsetof(struct foo, y)` 算出 `y` 在 struct 內的 byte offset。
`(char *)py - offsetof` 倒推到 struct 起點。
最後 cast 回 `(struct foo *)`。

`(char *)` 是因為 pointer 算術以**型別大小為單位**，char 是 1 byte，才能精確 byte-wise 倒推。

## ⚠️ 限制

- ptr 必須**真的**指向 struct 內的 member（否則 UB）
- 同一個 struct 不同 member 用 container_of 都能反推

## 🐧 對應

`include/linux/container_of.h`：

```c
#define container_of(ptr, type, member) ({         \
    void *__mptr = (void *)(ptr);                  \
    static_assert(__same_type(*(ptr), ((type *)0)->member), ...); \
    ((type *)(__mptr - offsetof(type, member))); })
```

Linux 版加了型別檢查（`__same_type`），但核心算式一樣。

**這個 macro 是 kernel OOP 的基石**：
- `list_entry`、`hlist_entry` 都是 `container_of` 包裝
- driver 接 `struct device *dev` 後反推出整個 driver-specific struct
- VFS 從 `struct inode *` 反推 fs-specific data

→ 你 Day 36 必懂。
