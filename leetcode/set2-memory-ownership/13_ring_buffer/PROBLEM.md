# Problem #13 — ring_buffer

> **Difficulty**: Medium | **Time**: 45 分鐘 | **Day**: 31 | **Set 2**

## 🎯 題目

實作固定大小的 ring buffer（FIFO）：

```c
struct ring {
    int *data;
    size_t cap;
    size_t head;    /* 下個 push 寫的位置 */
    size_t tail;    /* 下個 pop 讀的位置 */
    size_t count;   /* 目前內容數 */
};

int  ring_init(struct ring *r, size_t cap);
int  ring_push(struct ring *r, int v);   /* 滿了 fail */
int  ring_pop(struct ring *r, int *out); /* 空了 fail */
void ring_free(struct ring *r);
```

## 🧪 行為

```c
ring_init(r, 4);
ring_push(r, 1); ring_push(r, 2); ring_push(r, 3); ring_push(r, 4);
ring_push(r, 5);   /* 滿，fail */
int v;
ring_pop(r, &v);   /* v=1 */
ring_pop(r, &v);   /* v=2 */
ring_push(r, 5);   /* 現在有空位，OK */
```

## 🛠️ Hints

關鍵：head/tail 用 modulo 環繞：
```c
push: r->data[r->head] = v; r->head = (r->head + 1) % r->cap; r->count++;
pop:  *out = r->data[r->tail]; r->tail = (r->tail + 1) % r->cap; r->count--;
```

判斷空/滿用 count（最簡單），不要用 head==tail（無法區分滿跟空）。

## ⚠️ Edge cases
- [ ] cap == 0 → 你決定（強制改最小 1 或拒絕）
- [ ] 滿 push fail
- [ ] 空 pop fail
- [ ] 多輪 push/pop 後 head/tail 環繞

## 🐧 對應
kernel `kfifo` 是 lock-free 進階版（見 #29）。
