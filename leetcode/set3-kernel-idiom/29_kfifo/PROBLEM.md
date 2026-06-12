# Problem #29 — kfifo（kernel ring buffer 簡化版）

> **Difficulty**: Hard | **Time**: 45-60 分鐘 | **Day**: 49 | **Set 3**

## 🎯 題目

實作 `kfifo` 風格的 ring buffer，特色：
1. **size 必須是 2 的次方**（用 `&` 替代 `%` 加速）
2. head/tail **不取 modulo**——直接讓它無限增長，讀寫時用 `& mask`
3. **byte-wise**（kfifo 操作的是 bytes 不是 elements）

```c
struct kfifo {
    unsigned char *buf;
    unsigned int size;       /* 必須 power of 2 */
    unsigned int mask;       /* size - 1 */
    unsigned int in;         /* 寫入位置（不取模）*/
    unsigned int out;        /* 讀取位置（不取模）*/
};

int kfifo_init(struct kfifo *f, unsigned int size);
unsigned int kfifo_in(struct kfifo *f, const void *buf, unsigned int n);   /* 寫，回實際寫入 */
unsigned int kfifo_out(struct kfifo *f, void *buf, unsigned int n);         /* 讀，回實際讀出 */
unsigned int kfifo_len(struct kfifo *f);
void kfifo_free(struct kfifo *f);
```

## 🛠️ 為什麼 size 是 2 的次方？

```c
/* 普通 ring buffer */
in = (in + 1) % size;    /* 除法慢 */

/* kfifo */
in = (in + 1) & mask;    /* AND 快 */
/* mask = size - 1，當 size = 2^N 時，& mask 等價於 mod size */
```

且因為 `in` 跟 `out` 不取模，**它們的差就是 fifo 內元素數**：
```c
len = in - out;
free_space = size - (in - out);
```

無正負溢出處理需要（unsigned wrap-around 自動正確，前提：size <= INT_MAX）。

## 🧪 行為

```c
struct kfifo f;
kfifo_init(&f, 16);    /* 16 是 2^4 */

unsigned char data[] = "hello";
kfifo_in(&f, data, 5);

unsigned char buf[10];
unsigned int n = kfifo_out(&f, buf, 5);
assert(n == 5);
assert(memcmp(buf, "hello", 5) == 0);

kfifo_free(&f);
```

## 🐧 對應

`include/linux/kfifo.h` 是這個的「巨集化 + 型別安全」進階版（用 `__kfifo_int_must_check_helper` 等）。
本題簡化非 byte-element 變化。

⚠️ 真實 kfifo 還有 `lockless` 版（單 producer / 單 consumer 不需 lock），這題不處理並發。
