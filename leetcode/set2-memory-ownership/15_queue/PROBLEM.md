# Problem #15 — queue (FIFO)

> **Difficulty**: Easy | **Time**: 20 分鐘 | **Day**: 31 | **Set 2**

## 🎯 題目

```c
struct queue { int *data; size_t cap; size_t head; size_t tail; size_t count; };
int  queue_init(struct queue *q, size_t cap);
int  queue_enqueue(struct queue *q, int v);
int  queue_dequeue(struct queue *q, int *out);
void queue_free(struct queue *q);
```

FIFO（先進先出）。其實就是 #13 ring_buffer 換名字。

## 🧪 Examples

```c
enq 1, enq 2, enq 3
deq → 1, deq → 2, deq → 3
deq → fail
```

## 🛠️ Hints

跟 #13 ring_buffer 一模一樣，只是名字 push→enqueue、pop→dequeue。**這題的點是練習「同概念換包裝」**——在不同 API 下表達相同行為。

可選擇：
- 固定 cap 版：滿了 fail（同 #13）
- 動態 cap 版：滿了 doubling（自己 design 一個變體）

starter 給的是固定 cap 版，跟 ring_buffer 一致。
