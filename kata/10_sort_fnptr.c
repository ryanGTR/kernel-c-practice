// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 10  函式指標排序    同一個 sort，換一個 cmp 就換一種順序
 * @fn: my_sort
 *
 * 規格：實作 my_sort，用 insertion sort 就好（不比效率，比你懂不懂 cmp）。
 *       簽名故意跟 qsort(3) 一樣 —— 你寫完就順手學會了 qsort。
 *
 *   void my_sort(char **items, size_t n, int (*cmp)(const void *, const void *));
 *
 * 動手前先答：
 *   Q1. cmp 的參數為什麼是 const void *？（提示：qsort 要能排任何型別的陣列）
 *       A:
 *   Q2. 🔥🔥 這題全部的重點：
 *       你在 my_sort 裡要比較 items[i] 和 items[j]，該怎麼呼叫 cmp？
 *
 *           cmp(items[i], items[j])      <- A
 *           cmp(&items[i], &items[j])    <- B
 *
 *       看下面 cmp_str 的實作：它拿到 a 之後做 `*(char * const *)a`——
 *       它**解了一次參考**。所以它期待收到的是什麼？char * 還是 char **？
 *       A:
 *   Q3. 承上。如果你傳了 A，程式會怎樣？（它會編譯過嗎？會 crash 嗎？）
 *       A:
 *
 * 💡 這就是 qsort 用起來最常見的 bug，而且 **-Wall -Wextra -Werror 抓不到**：
 *    void * 吃下任何指標，型別資訊在函式邊界上被抹掉了。編譯器不會說一個字，
 *    然後 cmp 把一個 char*（"abc" 的位址）當成 char** 解參考 → 讀到 'a','b','c'
 *    這幾個 byte 當成一個指標 → 飛出去 → segfault。
 *
 *    **這正是 day-31「指標當條件恆為真」的親戚**：型別合法、意圖錯誤。
 *    C 的兩大殺手都在這條線上：
 *      (1) 我要的是內容，卻寫成了地址      <- day-31
 *      (2) 我要的是地址，卻寫成了內容      <- 這題
 *    Java 沒有這兩個 bug，因為 Java 從來不讓你直接看見地址。
 *
 * 🎯 這題請務必 `make gdb N=10`，在 cmp_str 裡 `p a`、`p *(char * const *)a`，
 *    親眼看到那一層解參考在幹嘛。看懂了，qsort 這輩子不會再寫錯。
 *
 * ⚠️ `make style N=10` 會噴 3 個 warning：
 *      "char * array declaration might be better as static const"
 *    這是 **checkpatch 的誤報**，指向下面 main() 裡的測試陣列。
 *    它建議 static const —— 但這幾個陣列**就是要被排序的**（元素要交換），
 *    加了 const 就不能排了。這題本質上需要一個可變的指標陣列。
 *
 *    留著它，別去「修好」。這是你第四個裁判的盲區：
 *      gcc        抓型別錯                 （抓不到意圖錯 —— day-31 的血）
 *      assert     抓行為錯                 （抓不到記憶體越界 —— kata 07 的血）
 *      ASan       抓記憶體錯               （抓不到風格、抓不到邏輯）
 *      checkpatch 抓風格                   （不懂你的意圖，會誤報）
 *    四個裁判，四種盲區。**沒有一個能單獨證明你是對的。**
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*
 * Two comparators, already written for you. Read them carefully --
 * they tell you exactly what my_sort must hand them.
 */
static int cmp_str(const void *a, const void *b)
{
	char *const *pa = a;	/* a points AT a char* -- deref once to get it */
	char *const *pb = b;

	return strcmp(*pa, *pb);
}

static int cmp_len(const void *a, const void *b)
{
	char *const *pa = a;
	char *const *pb = b;
	size_t la = strlen(*pa);
	size_t lb = strlen(*pb);

	if (la < lb)
		return -1;
	if (la > lb)
		return 1;
	return 0;
}

static void my_sort(char **items, size_t n, int (*cmp)(const void *, const void *))
{
	(void)items;
	(void)n;
	(void)cmp;	/* TODO: delete these and write your insertion sort */
}

int main(void)
{
	char *by_name[] = { "delta", "alpha", "charlie", "bravo" };
	char *by_size[] = { "ccc", "a", "dddd", "bb" };
	char *one[] = { "only" };

	my_sort(by_name, 4, cmp_str);
	assert(strcmp(by_name[0], "alpha") == 0);
	assert(strcmp(by_name[1], "bravo") == 0);
	assert(strcmp(by_name[2], "charlie") == 0);
	assert(strcmp(by_name[3], "delta") == 0);

	/* Same sort function. Different comparator. Different order. */
	my_sort(by_size, 4, cmp_len);
	assert(strcmp(by_size[0], "a") == 0);
	assert(strcmp(by_size[1], "bb") == 0);
	assert(strcmp(by_size[2], "ccc") == 0);
	assert(strcmp(by_size[3], "dddd") == 0);

	/* n == 1 and n == 0 must not blow up */
	my_sort(one, 1, cmp_str);
	assert(strcmp(one[0], "only") == 0);
	my_sort(one, 0, cmp_str);

	/* Sorting an already-sorted array is a no-op, not a shuffle. */
	my_sort(by_name, 4, cmp_str);
	assert(strcmp(by_name[0], "alpha") == 0);
	assert(strcmp(by_name[3], "delta") == 0);

	printf("kata 10: PASS\n");
	printf("  ^ 這題最後跑 `make gdb N=10`，在 cmp_str 裡看那層解參考。\n");
	return 0;
}
