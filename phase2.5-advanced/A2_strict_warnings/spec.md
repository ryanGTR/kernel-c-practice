# A2 — linked_list 嚴格 flag 重新檢討

## 目標

拿你 Phase 2 `06_linked_list` 寫好的 code，用**極致嚴格**的警告 flag 重 compile，把所有問題修乾淨。

這題不是重寫，是**審視**。

## 做法

### 第一步 — 複製現有 code

```bash
cp -r ../phase2-data-structures/06_linked_list ./my_linked_list
cd my_linked_list
```

### 第二步 — 改 Makefile 的 CFLAGS

```
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic \
         -Wconversion -Wshadow -Wcast-align \
         -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
         -Wdouble-promotion -Wnull-dereference \
         -Wformat=2 -Wwrite-strings -Wjump-misses-init \
         -Wlogical-op -Wduplicated-cond -Wduplicated-branches \
         -fstack-protector-strong -D_FORTIFY_SOURCE=2
```

### 第三步 — 看輸出，逐條修

常見會跳的警告（預測）：
- `-Wconversion` — `int` 傳給 `size_t` 的隱式轉換
- `-Wshadow` — 內層變數遮蔽外層
- `-Wstrict-prototypes` — `f()` 沒寫 `f(void)`
- `-Wcast-align` — pointer cast 到更嚴格對齊

### 第四步 — 加 sanitizer build

新增 Makefile target：

```makefile
debug: CFLAGS += -fsanitize=address,undefined -g -O1
debug: CFLAGS += -fno-omit-frame-pointer
debug: $(TARGET)
```

跑：
```bash
make clean && make debug
./test_linked_list
```

ASan 會抓 use-after-free / heap-overflow / memory-leak。
UBSan 會抓 signed integer overflow / alignment issues / null deref。

## 完成清單

- [ ] 全 CFLAGS 零 warning
- [ ] ASan build 跑完零 error
- [ ] UBSan build 跑完零 error
- [ ] `valgrind --leak-check=full --show-leak-kinds=all` 乾淨
- [ ] 寫一份 `findings.md` 記錄：哪些警告跳出來 + 為什麼 + 怎麼修

## 學習重點

這題不是學新語法，是學：
- **你的 C code 有多少「能過但不對」的地方**
- 嚴格警告是免費的靜態分析
- Sanitizer 能抓到 valgrind 抓不到的（signed overflow、OOB on stack）
- 為什麼 kernel code 對 type 這麼小心

## 延伸

做完這題再回頭看 Phase 1 你寫的 `my_wc`，多半也會有好幾個警告。全部修一遍。
