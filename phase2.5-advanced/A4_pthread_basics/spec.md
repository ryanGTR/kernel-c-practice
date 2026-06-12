# A4 — pthread 基礎 + thread-safe hash_table

## 目標

第一次碰真正的並行：學 `pthread_mutex` / `pthread_cond`，然後把 Phase 2 的 hash_table 改成 thread-safe。

kernel 裡 race condition 是日常，早練早好。

## 階段

### 階段 1 — producer-consumer（bounded buffer）

經典題：N 個 producer、M 個 consumer、固定大小的 ring buffer。

```c
struct bounded_buf {
    int   data[CAP];
    size_t head, tail, count;
    pthread_mutex_t lock;
    pthread_cond_t  not_full;
    pthread_cond_t  not_empty;
};

void bbuf_put(struct bounded_buf *b, int v);
int  bbuf_get(struct bounded_buf *b);
```

要求：
- `put` 在滿的時候要 block，`get` 在空的時候要 block
- 用 condvar 正確喚醒（記得 `while(cond)` 不是 `if(cond)`）
- 測試：2 producer + 3 consumer 各產 1000 筆，全部數字都被取到且只取一次

### 階段 2 — thread-safe hash_table

把 Phase 2 `07_hash_table` 加鎖。

**難點**：怎麼加鎖粒度？

- 粗粒度：整個 table 一把 mutex（簡單，但 contention 高）
- 細粒度：每個 bucket 一把 mutex（複雜，throughput 高）

先做粗粒度版，跑 micro-benchmark（8 thread 各做 100k 次 insert/lookup），量 throughput。

再做細粒度版，比較。

### 階段 3（延伸）— reader-writer lock

用 `pthread_rwlock` 改寫 hash_table：多 reader 可以並發，writer 互斥。

量 read-heavy workload（95% lookup / 5% insert）的 throughput 差異。

## 硬規則

- **沒跑過 ThreadSanitizer 不算過**
  ```
  CFLAGS += -fsanitize=thread -g -O1
  ```
  TSan 會抓 data race，比 eyeball review 可靠 100 倍
- 每把鎖一定要配好 lock/unlock（包含 error path）
- Condvar wait 永遠寫在 while loop 裡，不要用 if

## 完成清單

- [ ] 階段 1 producer-consumer 正確（sum 對、無 deadlock、TSan 乾淨）
- [ ] 階段 2 粗粒度 hash_table 正確
- [ ] 階段 2 細粒度 hash_table 正確且 throughput 高於粗粒度
- [ ] TSan 全程零 warning
- [ ] `valgrind --tool=helgrind` 乾淨（另一個 race detector）

## 工具

- `pthread_create` / `pthread_join` / `pthread_mutex_*` / `pthread_cond_*`
- `-pthread` 編譯 flag（不是 `-lpthread`）
- `man 7 pthreads` — 整體介紹
- `man pthread_cond_wait` — 搞懂為什麼要配 mutex

## 學習重點

- 為什麼 kernel 有那麼多鎖（spinlock、mutex、rwlock、rcu、seqlock）
- Condvar 的 spurious wakeup
- False sharing（cache line 層級的效能陷阱）
- Deadlock 的三種成因

## 陷阱預告

做了你就會踩到：

1. 忘記在 cond_wait 前持有 mutex → 未定義行為
2. cond_signal 呼叫時沒持有 mutex → 競態
3. while vs if 搞錯 → 偶發 bug
4. 解鎖順序跟上鎖順序不一致 → deadlock
5. TSan 抓到「看起來沒問題」的 race → 原來 memory model 比你想得複雜

## 進階延伸（做完再挑）

- 讀 `kernel/locking/mutex.c` — kernel 自己的 mutex 實作
- 讀 `include/linux/rcupdate.h` — RCU 為什麼在 kernel 裡這麼重要
- 用 atomic + memory barrier 寫 lock-free SPSC ring buffer
