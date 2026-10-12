# Kernel 的 linked list 怎麼寫（原始碼導讀）

> 來源：`~/Documents/linux_kernel`（v7.0），每段都附路徑:行號，可以自己打開對照。
> 2026-09-29 建，接在 06_linked_list 通關之後、lab0-c 之前讀。

## 先猜（讀之前先寫下答案）

1. 你的 `struct node` 有 `data` 和 `next`。kernel 的節點裡**沒有 data 欄位**，那資料放哪？
2. 你的 `list_delete` 要處理「刪 head」特例。kernel 的 `list_del` 只收**一個參數**（要刪的節點），它怎麼知道前一個是誰？
3. 你的 `list_delete` 刪完就 `free`。kernel 的 `list_del` 刪完之後把指標設成什麼？為什麼不設成 NULL？

---

## 1. 節點本體：只有兩根指標

`include/linux/types.h:204 (v7.0)`

```c
struct list_head {
	struct list_head *next, *prev;
};
```

跟你的 `struct node` 對照：

| | 你的 06 | kernel |
|---|---|---|
| 資料在哪 | 節點裡的 `int data` | **節點塞在資料裡**（侵入式） |
| 方向 | 單向 `next` | 雙向 `next` + `prev` |
| 結尾 | `NULL` | 繞回 head（環狀），沒有 NULL |

用法長這樣（`include/linux/sched.h:954 (v7.0)`，每個 process 的 `task_struct` 裡）：

```c
struct task_struct {
	...
	struct list_head		tasks;
	...
};
```

Java 類比：不是 `List<Task>` 裝著 Task，而是每個 Task 身上自己長了一個「掛勾」，串列只串掛勾。

## 2. 插入：四個指標，沒有 if

`include/linux/list.h:154 (v7.0)`

```c
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	if (!__list_add_valid(new, prev, next))
		return;

	next->prev = new;
	new->next = next;
	new->prev = prev;
	WRITE_ONCE(prev->next, new);
}
```

`include/linux/list.h:175 (v7.0)`

```c
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}
```

- `__list_add_valid` 是除錯檢查（開 `CONFIG_LIST_HARDENED` 時才真的檢查），邏輯上可以先忽略。
- `WRITE_ONCE` 是給並行（多核）看的，單執行緒下就等於 `prev->next = new`。
- **重點**：你的 `list_append` 有 `if (!head) return new;`（空串列特例）。這裡沒有。
  為什麼？→ 因為 head 本身就是一個節點，空串列時 `head->next == head`，四個指標照樣接得起來。
  這就是 phase2 README 站 2b′ 那題「哪些 if 在哨兵版消失了」的答案。

## 3. 刪除：兩行，沒有 prev 追蹤、沒有 head 特例

`include/linux/list.h:201 (v7.0)`

```c
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	WRITE_ONCE(prev->next, next);
}
```

`include/linux/list.h:235 (v7.0)`

```c
static inline void list_del(struct list_head *entry)
{
	__list_del_entry(entry);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}
```

✍️ **留給你自己讀的一段**：打開 `include/linux/list.h`，找 `__list_del_entry`（就在 201 行附近），
回答：它怎麼把 `prev` 和 `next` 交給 `__list_del`？對照你今天 `list_delete` 裡的 prev/cur 兩根手指，差在哪？

## 4. 從掛勾找回整個 struct：container_of

`include/linux/container_of.h:19 (v7.0)`

```c
#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	static_assert(__same_type(*(ptr), ((type *)0)->member) ||	\
		      __same_type(*(ptr), void),			\
		      "pointer type mismatch in container_of()");	\
	((type *)(__mptr - offsetof(type, member))); })
```

`include/linux/list.h:608 (v7.0)`

```c
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)
```

- 核心只有最後一行：**掛勾的位址 − 掛勾在 struct 裡的偏移量 = struct 的起點**。
- `static_assert` 只是型別檢查，防止你傳錯東西。
- 你之前問「怎麼做 address 的比對」——這裡就是拿位址直接做減法。

## 5. 走訪：一個 for 迴圈巨集

`include/linux/list.h:781 (v7.0)`

```c
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
	     !list_entry_is_head(pos, head, member);			\
	     pos = list_next_entry(pos, member))
```

跟你今天的 `list_print` 對照：你的停止條件是 `cur != NULL`，這裡是「繞回 head 就停」。
而且 `pos = next` 寫在 for 的第三格，每圈必做，**不可能忘記往前走**。你今天的無窮迴圈 bug，在這個寫法下不會發生。

---

## 對帳（讀完寫一句）

> 我原本以為 ______，實際上 kernel ______，差在 ______。

## See Also

- `06_linked_list/diagrams/` — 單向版逐步圖
- `06b_dlist/` — 教科書版雙向（有 NULL、有 head/tail 特例），拿來跟本頁對照
- `~/Documents/lab0-c/list.h` — 從 kernel 複製出來的 userspace 版
