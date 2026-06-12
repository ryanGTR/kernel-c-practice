# Problem #20 — bump_allocator（最簡 alloc）

> **Difficulty**: Easy | **Time**: 15 分鐘 | **Day**: 32 | **Set 2**

## 🎯 題目

最簡單的 allocator——靜態 buffer + offset 指標。**沒 free**、**沒 reset**、用完就完。

```c
extern char bump_buf[BUMP_SIZE];   /* 靜態 buffer */
void *bump_alloc(size_t n);
size_t bump_used(void);            /* 看用了多少 */
```

## 🧪 行為

```c
int *p = bump_alloc(sizeof(int));   /* 第一次：buf+0 */
char *s = bump_alloc(100);          /* buf+sizeof(int) */
/* 不能 free。 */
```

## 🛠️ Hints

```c
#define BUMP_SIZE 4096
char bump_buf[BUMP_SIZE];
static size_t bump_offset = 0;

void *bump_alloc(size_t n) {
    if (bump_offset + n > BUMP_SIZE) return NULL;
    void *p = bump_buf + bump_offset;
    bump_offset += n;
    return p;
}
```

→ 比 #19 arena 更簡單：buffer 不用 malloc（靜態配置），且**沒 reset**。

## 🐧 為什麼存在？

- **embedded firmware**：無 OS、無 heap，但要動態分配 → 用 bump
- **kernel boot 早期**：早期 allocator (`memblock`) 就是 bump 變體
- **freestanding C**：沒 libc 環境

簡單到一看就懂，但這就是「**最小可用 allocator**」的本質。
