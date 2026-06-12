# Problem #19 — arena_allocator

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 32 | **Set 2**

## 🎯 題目

實作 arena allocator——大塊一次配，內部 bump 切分，**不支援個別 free**，整個 arena reset 才釋放。

```c
struct arena {
    char *block;
    size_t cap;
    size_t offset;
};

int   arena_init(struct arena *a, size_t cap);
void *arena_alloc(struct arena *a, size_t n);    /* 不夠就 NULL */
void  arena_reset(struct arena *a);              /* offset 歸 0，重用 block */
void  arena_destroy(struct arena *a);            /* 釋放 block */
```

跟 #18 memory_pool 差別：
- mempool：固定 size，可個別 free
- arena：任意 size，**不能**個別 free，但很快很簡單

## 🧪 行為

```c
arena_init(a, 1024);
int *p = arena_alloc(a, sizeof(int));
char *s = arena_alloc(a, 100);
struct foo *f = arena_alloc(a, sizeof(*f));
/* ... 用 ... */
arena_reset(a);   /* 全部失效，但 block 還在，可重用 */
arena_destroy(a); /* 釋放 block */
```

## 🛠️ Hints

```c
arena_alloc:
    if (offset + n > cap) return NULL;
    void *p = block + offset;
    offset += n;
    return p;

arena_reset:
    offset = 0;
```

注意 alignment：嚴格版要 `offset = (offset + 7) & ~7;` 對齊。簡化版可不處理。

## 🐧 對應

PostgreSQL 的 **MemoryContext** 是這個概念的進階版（含 hierarchical 樹）。
GCC 自己內部也用 obstack。
Linux kernel 的 `seq_buf` 類似想法。
