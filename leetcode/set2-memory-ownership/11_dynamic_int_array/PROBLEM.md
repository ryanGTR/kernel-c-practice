# Problem #11 — dynamic_int_array

> **Difficulty**: Medium | **Time**: 30-45 分鐘 | **Day**: 26 (my_sort), Day 31 | **Set 2**

## 🎯 題目

實作會自動擴容的 int 陣列：

```c
struct iarr {
    int *data;
    size_t len;
    size_t cap;
};

int  iarr_init(struct iarr *a, size_t cap);
int  iarr_push(struct iarr *a, int v);
int  iarr_pop(struct iarr *a, int *out);
void iarr_free(struct iarr *a);
```

跟 strbuf 結構幾乎一樣，但放 int 而非 char。**這是 darray 的 int 簡化版**。

## 🛠️ Hints

`iarr_push`：cap 不夠就 doubling realloc（同 strbuf）。
`iarr_pop`：len > 0 才能 pop，把 data[len-1] 寫到 *out，len--。

```c
if (a->len == a->cap) {
    size_t new_cap = a->cap == 0 ? 8 : a->cap * 2;
    int *tmp = realloc(a->data, new_cap * sizeof(*a->data));
    if (!tmp) return -1;
    a->data = tmp;
    a->cap = new_cap;
}
a->data[a->len++] = v;
```

## ⚠️ Edge cases
- [ ] iarr_pop 空陣列 → 失敗
- [ ] init cap=0 → 你決定（強制改 8 或允許）
- [ ] realloc 失敗 → 用 tmp 中介

## 📚 對應
- 你 `kernel-c-practice/phase2-data-structures/08_dynamic_array/` 的核心
- C++ `std::vector<int>` 簡化版
