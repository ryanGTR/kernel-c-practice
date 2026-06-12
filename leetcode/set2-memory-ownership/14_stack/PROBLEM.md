# Problem #14 — stack (LIFO)

> **Difficulty**: Easy | **Time**: 20 分鐘 | **Day**: 31 | **Set 2**

## 🎯 題目

```c
struct stack { int *data; size_t len; size_t cap; };
int  stack_init(struct stack *s, size_t cap);
int  stack_push(struct stack *s, int v);
int  stack_pop(struct stack *s, int *out);
int  stack_peek(const struct stack *s, int *out);
void stack_free(struct stack *s);
```

LIFO（後進先出）。push 加在 top，pop 從 top 拿。內部就是動態陣列。

## 🧪 Examples

```c
push 1, push 2, push 3 → top=3
pop → 3, pop → 2, pop → 1
pop → fail (空)
peek 不消耗
```

## 🛠️ Hints

跟 #11 dynamic_int_array 幾乎一樣，**就是換 wrap**：
```c
stack_push = iarr_push
stack_pop  = iarr_pop（但回傳 top，即 data[len-1]）
stack_peek = data[len-1]，不改 len
```

可以直接 import #11 的解法重命名。

## 📚 對應
LeetCode #155 Min Stack（這題的進階版）。
