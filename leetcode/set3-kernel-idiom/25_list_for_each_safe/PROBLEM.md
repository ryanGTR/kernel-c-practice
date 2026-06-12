# Problem #25 — list_for_each_entry_safe（走訪時刪除）

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 39 | **Set 3**

## 🎯 題目

實作 list 走訪時可安全刪除節點的 macro（簡化版）：

```c
struct list_node {
    struct list_node *next;
    int value;
};

#define list_for_each_safe(cur, tmp, head) \
    for ((cur) = (head), (tmp) = (cur) ? (cur)->next : NULL; \
         (cur) != NULL; \
         (cur) = (tmp), (tmp) = (cur) ? (cur)->next : NULL)
```

關鍵：**先存下一個節點到 `tmp`**，這樣即使 free 掉 `cur`，下次迭代也不會碰到 freed 的記憶體。

## ⚠️ 為什麼需要

❌ 普通 `list_for_each` 走訪時 free → UAF：
```c
for (cur = head; cur; cur = cur->next) {
    free(cur);   /* 💥 下一輪 cur->next 是 dangling */
}
```

✅ safe 版：
```c
list_for_each_safe(cur, tmp, head) {
    free(cur);   /* OK，cur->next 已存在 tmp */
}
```

## 🧪 行為

```c
struct list_node *head = build_list(1, 2, 3, 4, 5);

struct list_node *cur, *tmp;
int sum = 0;
list_for_each_safe(cur, tmp, head) {
    sum += cur->value;
    free(cur);
}
assert(sum == 15);
/* 走訪 + 釋放完成，valgrind 應該乾淨 */
```

## 🐧 對應

`include/linux/list.h` 的 `list_for_each_entry_safe`（更複雜，含 container_of）。
這題是入門簡化版。Day 39 你會看真實 kernel 版。
