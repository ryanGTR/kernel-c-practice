// SPDX-License-Identifier: GPL-2.0
/*
 * Day 21 — student.c（共用實作）
 *
 * 把宣告在 student.h 的所有函式都實作出來。
 * 看 wiki/daily/2026-04/day-21.md 的步驟 1-3 + 挑戰題。
 *
 * 三個品質檢查：
 *   [ ] malloc 都用 sizeof(*p) 慣用法
 *   [ ] 字串複製用 strncpy + 手動補 \0（Day 20 教過的 userspace 安全寫法）
 *   [ ] list_destroy 用「先存 next，再 free cur」防 UAF
 */

#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* === Step 1 === */

void student_fill(struct student *s, const char *name, int score)
{
	strncpy(s->name, name, sizeof(s->name) - 1);
	s->name[sizeof(s->name) - 1] = '\0'; // 手動補 \0（strncpy 不保證）
	s->score = score;
}

void student_print(const struct student *s)
{
	printf("%-20s  %d\n", s->name, s->score);
}

/* === Step 2 === */

struct student *student_create(const char *name, int score)
{
	/* TODO:
	 *   1. malloc(sizeof(*s))                ← Day 19 慣用法
	 *   2. NULL 檢查 → 回 NULL
	 *   3. student_fill(s, name, score)      ← 重用 Step 1 的函式
	 *   4. return s
	 *
	 * 契約：caller 拿到 pointer 後**必須**呼叫 student_destroy。
	 *       這個函式不擁有記憶體，所有權移轉給 caller。
	 */
	struct student *s = malloc(sizeof(*s));
	if (!s)
		return NULL;
	student_fill(s, name, score);
	return s;
}

void student_destroy(struct student *s)
{
	free(s);
}

void list_print(struct student_node *head)
{
	struct student_node *cur = head;
	while (cur) {
		student_print(&cur->data);
		cur = cur->next;
	}
}

void list_destroy(struct student_node *head)
{
	struct student_node *cur = head;
	while (cur) {
		struct student_node *next = cur->next;
		free(cur);
		cur = next;
	}
}

/* === 挑戰題（做完三步驟再寫） === */

size_t list_len(struct student_node *head)
{
	int i = 0;
	struct student_node *cur = head;
	while (cur) {
		i++;
		cur = cur->next;
	}
	return i;
}

struct student_node *list_find_max(struct student_node *head)
{
	struct student_node *max_node = head;
	struct student_node *cur = head ? head->next : NULL;
	while (cur) {
		if (cur->data.score > max_node->data.score) {
			max_node = cur;
		}
		cur = cur->next;
	}
	return max_node;
}

void list_prepend(struct student_node **head, struct student_node *n)
{
	/* TODO:
	 *   n->next = *head;
	 *   *head = n;
	 *
	 * 為什麼第一個參數是 `**`？
	 *   呼叫端有個 struct student_node *list = NULL;
	 *   你要改的是 list 這個變數的內容（讓它指向新的 head）
	 *   所以函式要拿 &list（型別是 struct student_node **）
	 *
	 * 比 Day 17 的 swap(int *a, int *b) 多一層——那邊改的是 int，
	 * 這邊改的是 pointer 變數本身。
	 */
	n->next = *head;
	*head = n;
}
