# Problem #18 — memory_pool（固定 size 物件池）

> **Difficulty**: Hard | **Time**: 60 分鐘 | **Day**: 32 | **Set 2**

## 🎯 題目

實作固定 size 物件池——startup 一次配大塊，之後 alloc/free 都從池內，避免反覆 malloc。

```c
struct mempool {
    void *block;          /* malloc 的大塊 */
    size_t obj_size;
    size_t capacity;
    size_t allocated;     /* 已配出去的 */
    void *free_list;      /* 鏈起來的 free 物件 */
};

int   mempool_init(struct mempool *p, size_t obj_size, size_t capacity);
void *mempool_alloc(struct mempool *p);
void  mempool_free(struct mempool *p, void *obj);
void  mempool_destroy(struct mempool *p);
```

## 🧪 行為

```c
struct mempool p;
mempool_init(&p, sizeof(struct foo), 100);   /* 配 100 個 struct foo */

struct foo *f = mempool_alloc(&p);            /* 從池拿一個 */
struct foo *g = mempool_alloc(&p);
mempool_free(&p, f);                           /* 還回池 */
struct foo *h = mempool_alloc(&p);            /* 大概拿到剛 free 的 f */

mempool_destroy(&p);                           /* 整池一次釋放 */
```

## 🛠️ Hints

技巧：用「**侵入式 free list**」——free 的物件本身存「下一個 free 物件的位址」：

```c
init: 把 block 切成 capacity 個格子，每格指向下一格，最後一個 NULL。
      free_list 指向第一格。

alloc: if (!free_list) return NULL;
       void *obj = free_list;
       free_list = *(void **)free_list;   /* 第一個 word 是 next 指標 */
       return obj;

free:  *(void **)obj = free_list;
       free_list = obj;
```

→ 不需要額外配置「free list 的 metadata」，重用 free 物件本身。**經典技巧**。

## ⚠️ Edge cases
- [ ] obj_size < sizeof(void*) → 你決定（強制最小 sizeof(void*) 或拒絕）
- [ ] alloc 池滿 → return NULL
- [ ] mempool_destroy 後不能再 alloc

## 🐧 對應
Linux `mempool_t` (`include/linux/mempool.h`)、`kmem_cache_*` 都是進階版這個概念。`drivers/md/dm.c`、network skb cache 處處可見。
