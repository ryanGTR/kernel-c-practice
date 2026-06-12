# Problem #16 — doubly_linked_list

> **Difficulty**: Medium | **Time**: 45 分鐘 | **Day**: 21, Day 29 | **Set 2**

## 🎯 題目

實作 **doubly-linked list**（含 head node 風格）：

```c
struct dlist_node {
    int value;
    struct dlist_node *prev;
    struct dlist_node *next;
};

struct dlist {
    struct dlist_node *head;
    struct dlist_node *tail;
    size_t len;
};

void dlist_init(struct dlist *l);
int  dlist_push_front(struct dlist *l, int v);
int  dlist_push_back(struct dlist *l, int v);
int  dlist_pop_front(struct dlist *l, int *out);
int  dlist_pop_back(struct dlist *l, int *out);
void dlist_free(struct dlist *l);
```

## 🛠️ Hints

雙向 list 的 push_front：
```c
node = malloc(...);
node->value = v;
node->prev = NULL;
node->next = l->head;
if (l->head) l->head->prev = node;
else l->tail = node;        /* 空 list 時 tail 也要設 */
l->head = node;
l->len++;
```

關鍵：**邊界條件多**——空 list、單一 node、多 node。每個 push/pop 都要正確維護 4 條邊：
1. 新 node 的 prev / next
2. 鄰居的 prev / next
3. l->head 跟 l->tail
4. l->len

## ⚠️ Edge cases
- [ ] 空 list pop → fail
- [ ] 只剩 1 node 時 pop → head/tail 都要置 NULL
- [ ] 連續 push_front × N → tail 永遠是第一個 push 進去的

## 🐧 對應
Linux `list_head` (`include/linux/list.h`) 是 circular doubly-linked，比這題進階。Day 36 你會碰到。
