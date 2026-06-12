# Problem #30 — atomic counter (C11 _Atomic)

> **Difficulty**: Medium | **Time**: 30 分鐘 | **Day**: 49 | **Set 3**

## 🎯 題目

用 C11 `<stdatomic.h>` 實作執行緒安全的 counter：

```c
struct counter {
    _Atomic int value;
};

void counter_init(struct counter *c, int v);
void counter_inc(struct counter *c);
void counter_dec(struct counter *c);
int  counter_get(const struct counter *c);
int  counter_inc_and_get(struct counter *c);   /* atomic 加 1 並回新值 */
```

## 🧪 行為

```c
struct counter c;
counter_init(&c, 0);

/* 多 thread 各自 inc 1000 次 */
#pragma omp parallel for
for (int i = 0; i < N_THREADS * 1000; i++)
    counter_inc(&c);

/* 結果應該等於 N_THREADS * 1000，無 race */
assert(counter_get(&c) == N_THREADS * 1000);
```

## 🛠️ Hints

C11 `<stdatomic.h>` 提供：

```c
#include <stdatomic.h>

atomic_init(&c->value, v);              /* 初始化（非 atomic）*/
atomic_fetch_add(&c->value, 1);         /* atomic value++ 並回舊值 */
atomic_fetch_sub(&c->value, 1);
atomic_load(&c->value);                 /* 安全讀 */
```

`atomic_fetch_add` 回**舊值**，加 1 後新值 = `fetch_add(...) + 1`。

## 🐧 對應

Linux `<linux/atomic.h>`：
```c
atomic_t counter = ATOMIC_INIT(0);
atomic_inc(&counter);
atomic_dec(&counter);
int v = atomic_read(&counter);
int new_v = atomic_inc_return(&counter);
```

API 命名不同但概念一致。
**真實 kernel atomic 還有 memory ordering**（`smp_mb`, `acquire/release`），本題簡化版用 default seq_cst。

## ⚠️ 為什麼不能用普通 `int++`？

`int++` 在多 thread 環境**不是 atomic**：
- read value
- add 1
- write value

兩 thread 同時執行 → 最後可能只 +1（race）。
`atomic_fetch_add` 由 CPU 硬體保證單一 atomic 操作（用 `lock` prefix on x86）。

## 📚 進階閱讀
- 你 2026-04-26 凌晨我們聊過的 UB → privesc 鏈：很多 kernel UAF 是 refcount race，**這題就是防止那種 race 的基礎工具**
