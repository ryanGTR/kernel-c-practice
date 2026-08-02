// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 09  動態陣列        malloc / realloc / free（my_sort 的心臟）
 * @fn: sarray_push
 *
 * 規格：實作一個會自己長大的字串陣列。
 *
 *   sarray_push(a, s)  把 s 的**副本**存進 a。成功回 0，記憶體不足回 -1。
 *                      容量滿了要 realloc 擴大。
 *   sarray_free(a)     釋放所有東西（含每一個字串），並把 a 歸零。
 *
 * 動手前先答：
 *   Q1. 🔥 push 進去的是「指標」還是「字串的副本」？
 *       如果只存指標，呼叫者把原字串改掉/free 掉，陣列裡的東西會怎樣？
 *       （這就是「誰擁有這塊記憶體」——規格上面已經幫你選了一種，看清楚）
 *       A:
 *   Q2. 🔥 realloc 失敗時，**原本那塊記憶體還有效嗎**？
 *       所以這行為什麼是 bug：`a->items = realloc(a->items, n);`
 *       A:
 *   Q3. cap 從 0 開始。第一次 push 時要配多大？之後怎麼長？（每次 +1 有什麼問題？）
 *       A:
 *   Q4. sarray_free 要 free 幾次？（提示：items 這塊 + 裡面每一根字串）
 *       A:
 *
 * 記憶體圖（這題一定要畫，不畫寫不出來）：
 *
 *   struct sarray (在 main 的 stack 上)
 *     items ─────┐  len=3  cap=4
 *                │
 *                ▼  (heap: malloc 出來的指標陣列)
 *              ┌────┬────┬────┬────┐
 *              │ *  │ *  │ *  │ ?  │   <- cap 個格子，len 個有效
 *              └─┬──┴─┬──┴─┬──┴────┘
 *                │    │    │
 *                ▼    ▼    ▼   (heap: 每一根都是 my_strdup 出來的)
 *              "ab" "cd" "ef"
 *
 *   ⇒ 兩層 heap。free 的順序不能反。
 *
 * ⚠️ `make asan N=09` 是這題真正的裁判：漏 free 任何一層，LSan 都會叫。
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sarray {
	char **items;
	size_t len;
	size_t cap;
};

static int sarray_push(struct sarray *a, const char *s)
{
	(void)a;
	(void)s;	/* TODO: delete these and write your code */
	return -1;
}

static void sarray_free(struct sarray *a)
{
	(void)a;	/* TODO: delete this and write your code */
}

int main(void)
{
	struct sarray a = { NULL, 0, 0 };
	char mutable_src[] = "original";
	size_t i;
	char buf[16];

	/* empty array frees cleanly */
	sarray_free(&a);
	assert(a.items == NULL && a.len == 0 && a.cap == 0);

	assert(sarray_push(&a, "first") == 0);
	assert(a.len == 1);
	assert(a.cap >= 1);
	assert(strcmp(a.items[0], "first") == 0);

	/* 🔥 push stores a COPY: mutating the source must not change the array */
	assert(sarray_push(&a, mutable_src) == 0);
	mutable_src[0] = 'X';
	assert(strcmp(a.items[1], "original") == 0);
	assert(a.items[1] != mutable_src);

	/* force at least one realloc: 20 pushes */
	for (i = 0; i < 20; i++) {
		snprintf(buf, sizeof(buf), "item%zu", i);
		assert(sarray_push(&a, buf) == 0);
	}
	assert(a.len == 22);
	assert(a.cap >= 22);

	/* everything survived the realloc(s) */
	assert(strcmp(a.items[0], "first") == 0);
	assert(strcmp(a.items[1], "original") == 0);
	for (i = 0; i < 20; i++) {
		snprintf(buf, sizeof(buf), "item%zu", i);
		assert(strcmp(a.items[2 + i], buf) == 0);
	}

	/* growth must be geometric, not +1 each time */
	assert(a.cap < 100);

	sarray_free(&a);
	assert(a.items == NULL && a.len == 0 && a.cap == 0);

	printf("kata 09: PASS\n");
	printf("  ^ 現在跑 `make asan N=09`。漏掉任何一層 free，LSan 才會告訴你。\n");
	return 0;
}
