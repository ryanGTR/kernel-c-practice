// SPDX-License-Identifier: GPL-2.0
/*
 * Day 21 Step 3 ★ 主菜 — student linked list（Week C 結業作品）
 *
 * 目標：把 Day 17-20 的技能全部串起來——struct + pointer + malloc/free + 字串安全。
 *
 * TODO：
 *   1. 寫一個 helper node_create(name, score) 配 + 填好一個 node、next = NULL
 *   2. main 裡建三個 node：Alice 92、Bob 85、Carol 78
 *   3. 手動串 a->next = b、b->next = c、c->next = NULL
 *   4. list_print(a) 印整條
 *   5. list_destroy(a) 釋放整條
 *
 * 編譯：make list_demo
 * 預期輸出：
 *   Alice                 92
 *   Bob                   85
 *   Carol                 78
 *
 * valgrind：必須 0 leaks 0 errors
 *   make valgrind  或 valgrind --leak-check=full ./list_demo
 *
 * 觀念里程碑：
 *   - 一週前你寫 list 會漏 Alice
 *   - 今天你**自己**配三個 node、自己串、自己 destroy 掉、valgrind 全綠
 *   - 這就是 Week C 的證書
 *
 * 挑戰題（做完上面再做，加在 main 結尾）：
 *   - printf("len = %zu\n", list_len(a));
 *   - struct student_node *top = list_find_max(a);
 *     if (top) student_print(&top->data);
 *   - list_prepend 練習用一個空 list 開始：
 *       struct student_node *l = NULL;
 *       list_prepend(&l, node_create("Dave", 95));
 *       list_print(l);
 *       list_destroy(l);
 */

#include <stdio.h>
#include <stdlib.h>
#include "student.h"

/* TODO: 寫 node_create 函式 — malloc + student_fill + next = NULL */
static struct student_node *node_create(const char *name, int score)
{
	struct student_node *s = malloc(sizeof(*s));
	if (!s)
		return NULL;
	student_fill(&s->data, name, score);
	s->next = NULL;
	return s;
}

int main(void)
{
	/* TODO: 建三個 node、串起來、印、釋放 */
	struct student_node *a = node_create("Alice", 92);
	struct student_node *b = node_create("Bob", 85);
	struct student_node *c = node_create("Carol", 78);

	a->next = b;
	b->next = c;
	c->next = NULL;

	list_print(a);
	printf("len : %zu\n", list_len(a));

	/* 挑戰題 #2 示範：找分數最高的學生 */
	struct student_node *top = list_find_max(a);
	if (top) {
		printf("max : ");
		student_print(&top->data); /* student_print 已自帶 \n */
	}

	list_destroy(a);

	/* 挑戰題 #3 示範：list_prepend — 從空 list 開始，後 prepend 的排在前面 */
	struct student_node *l = NULL;
	list_prepend(&l, node_create("Dave", 95));
	list_prepend(&l, node_create("Eve", 88));
	printf("\nprepend 結果（預期 Eve -> Dave）:\n");
	list_print(l);
	printf("len : %zu\n", list_len(l));
	list_destroy(l);

	return 0;
}
