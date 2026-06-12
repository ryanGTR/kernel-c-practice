# Problem #12 — strbuf（動態字串緩衝）

> **Difficulty**: Medium
> **Time limit**: 30-45 分鐘
> **Day mapping**: Day 19（Week C 主菜）
> **Tags**: malloc, realloc, ownership, **kernel-style init/exit pattern**

---

## 🎯 題目

實作一個會自動擴容的字串緩衝結構 `strbuf`，提供三個操作：

```c
struct strbuf {
    char *data;        /* heap-allocated string，永遠 \0 結尾 */
    size_t len;        /* 已寫入的字元數（不含 \0）*/
    size_t cap;        /* 已配置 buffer 大小（含 \0 空間）*/
};

int  strbuf_init(struct strbuf *sb, size_t initial_cap);
int  strbuf_append(struct strbuf *sb, const char *s);
void strbuf_free(struct strbuf *sb);
```

### Function 行為

#### `strbuf_init(sb, initial_cap)`
- 配 `initial_cap` bytes 給 `sb->data`
- 設 `sb->data[0] = '\0'`（空字串）
- 設 `len = 0, cap = initial_cap`
- **回傳**：成功 0、失敗 -1（malloc 失敗）

#### `strbuf_append(sb, s)`
- 把 `s` 接到 `sb->data` 結尾
- 自動擴容（cap 不夠時）
- **回傳**：成功 0、失敗 -1（realloc 失敗）

#### `strbuf_free(sb)`
- 釋放 `sb->data`
- 重置 `sb->data = NULL, len = 0, cap = 0`
- 可被重複呼叫不會 crash（idempotent）

---

## 🧪 Examples

### Example 1: 基本用法
```c
struct strbuf sb;
strbuf_init(&sb, 8);
strbuf_append(&sb, "Hello, ");
strbuf_append(&sb, "world");
strbuf_append(&sb, "! From day 19.");

printf("result: %s\n", sb.data);
printf("len=%zu cap=%zu\n", sb.len, sb.cap);

strbuf_free(&sb);
```

**Expected output**:
```
result: Hello, world! From day 19.
len=27 cap=??       (cap 看擴容策略，常見 32 或 64)
```

### Example 2: 多次擴容
```c
strbuf_init(&sb, 4);
for (int i = 0; i < 100; i++) strbuf_append(&sb, "ab");
// 應該成功，cap 多次擴容
// len = 200
```

### Example 3: 失敗清理
```c
strbuf_init(&sb, 4);
strbuf_append(&sb, "x");
strbuf_free(&sb);          // 正常釋放
strbuf_free(&sb);          // 應該安全，沒 crash
```

---

## ⚠️ Edge cases（必須處理）

1. **`initial_cap == 0`**: 你決定行為（可拒絕 / 可接受設成預設值）
2. **`s == NULL`**: 你決定行為（可 crash 報 BUG / 可視為 ""）
3. **`s == ""`** (空字串): 不該失敗、len 不變
4. **append 失敗時 sb 仍是合法狀態**: 失敗不該損壞既有資料
5. **realloc 失敗**: 不能覆蓋 `sb->data`（會 leak 舊資料）
6. **`strbuf_free` 後再 free**: 第二次必須無害

---

## 🛠️ C API hints

你會用到：

```c
malloc(size)             # 配 buffer
realloc(p, new_size)     # 擴容（4 條 path，看 wiki）
free(p)                  # 釋放
strlen(s)                # 算 s 長度
memcpy(dst, src, n)      # 複製 n bytes
```

**警告**：
- `realloc` 失敗回 NULL，但**舊指標還有效**——必須用 `tmp` 中介
- `memcpy(sb->data + sb->len, s, slen + 1)` 的 `+1` 是搬 `\0`
- `sb->data[0] = '\0'` 在 init 後必須執行（否則 strlen 看到垃圾）

---

## 📐 演算法重點

### 1. **realloc 失敗保護（必考）**

❌ **錯**:
```c
sb->data = realloc(sb->data, new_cap);
if (!sb->data) return -1;   // 舊資料已永久 lost！
```

✅ **對**:
```c
char *tmp = realloc(sb->data, new_cap);
if (!tmp) return -1;        // 舊 data 還有效，可以 caller 收尾
sb->data = tmp;
```

### 2. **擴容策略：doubling**

```c
size_t new_cap = (sb->cap + slen + 1) * 2;
```

確保 `new_cap >= sb->len + slen + 1`（含 `\0`），且翻倍避免後續頻繁 realloc。

### 3. **memcpy 起點 = 現有字串末端**

```c
memcpy(sb->data + sb->len, s, slen + 1);
sb->len += slen;             // 不是 += slen + 1！\0 不算長度
```

### 4. **strbuf_free 必須冪等**

```c
free(sb->data);              // free(NULL) 合法
sb->data = NULL;             // 第二次呼叫看到 NULL，free(NULL) no-op
sb->len = sb->cap = 0;
```

---

## ✅ 通關標準

跑：
```bash
make 12              # 編譯
./12_strbuf          # 跑（看 expected output）
make check 12        # valgrind + ASan 自動驗
```

通關需要：
- [ ] 輸出 `result: Hello, world! From day 19.`
- [ ] valgrind: `0 errors, 0 leaks`
- [ ] ASan: 無錯誤
- [ ] gcc `-Wall -Wextra -Werror` 編過（除了 step 3 的故意 demo）
- [ ] **白紙能寫出 `strbuf_append`**（戒律 #2）

---

## 🚫 禁忌（一律 fail）

- [ ] 用 `strcpy` / `strcat`（沒長度保護）
- [ ] `sb->data = realloc(sb->data, ...)` 直接覆蓋
- [ ] 擴容後 `len` 改錯（多算 1 或少算 1）
- [ ] init 後沒 `data[0] = '\0'`
- [ ] free 後不置 NULL

---

## 🐧 對 Kernel 的對應

這個 pattern 在 kernel 隨處可見：

```c
// Linux kernel 的 char_dev 範例
static int my_init(void) {
    struct my_data *d = kzalloc(sizeof(*d), GFP_KERNEL);
    if (!d) return -ENOMEM;
    /* ...其他初始化... */
    return 0;
err:
    kfree(d);
    return -EFAULT;
}

static void my_exit(struct my_data *d) {
    kfree(d->buf);
    kfree(d);
}
```

→ 你 strbuf 的 init/free 配對 = kernel 的 module_init/module_exit 配對的雛形。

---

## 📚 相關 wiki

- [`wiki/concepts/heap-allocator-internals.md`](../../../wiki/concepts/heap-allocator-internals.md) — realloc 4 條 path、doubling 策略、為什麼 realloc 失敗保護重要
- [`wiki/daily/2026-04/day-19.md`](../../../wiki/daily/2026-04/day-19.md) — Day 19 完整教材
- [`wiki/tools/c-api-cheatsheet.md`](../../../wiki/tools/c-api-cheatsheet.md) — malloc/realloc/memcpy 速查

---

## 🎯 解題步驟建議

1. **手機讀 PROBLEM.md**（10 分鐘）：理解規格 + edge cases
2. **電腦寫 starter.c**（25 分鐘）：填三個 function
3. **跑 `make check 12`**（5 分鐘）：valgrind + ASan
4. **失敗時讀 hints**（往上找對應「演算法重點」）
5. **通關後對 solution**：你怎麼寫的 vs 我怎麼寫的，差在哪

加油 🚀
