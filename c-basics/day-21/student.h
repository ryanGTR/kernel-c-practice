/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Day 21 — student.h（共用 header）
 *
 * Step 1：struct student 本體 + value 操作
 * Step 2：heap 上的 student（create/destroy）
 * Step 3：linked list（student_node + list_print/destroy）
 *
 * TODO（依 wiki/daily/2026-04/day-21.md）：
 *   1. 填 struct student（name 32 bytes + score）
 *   2. 填 struct student_node（包 student + next pointer）
 *   3. 不要寫 typedef—— kernel 不用 typedef，這裡跟著 kernel 風格
 */

#ifndef STUDENT_H
#define STUDENT_H

#include <stddef.h>

/* === Step 1: 基本 student === */

struct student {
	char name[32];
	int score;
};

/* 填一個已存在的 struct student（caller 提供記憶體）*/
void student_fill(struct student *s, const char *name, int score);

/* 印出來（const 表示只讀）*/
void student_print(const struct student *s);

/* === Step 2: heap 上的 student === */

/* malloc + fill 一個新 student；失敗回 NULL；caller 必須呼叫 student_destroy */
struct student *student_create(const char *name, int score);

/* free 掉 create 出來的 student */
void student_destroy(struct student *s);

/* === Step 3: linked list === */

struct student_node {
	struct student data;
	struct student_node *next;
};

/* 走訪整條 list，每個節點呼叫 student_print */
void list_print(struct student_node *head);

/* 釋放整條 list（先存 next，再 free cur，防 UAF）*/
void list_destroy(struct student_node *head);

/* === 挑戰題（做完三步驟後再寫） === */

/* 回傳節點數 */
size_t list_len(struct student_node *head);

/* 回傳分數最高的節點 pointer；空 list 回 NULL */
struct student_node *list_find_max(struct student_node *head);

/* 把 n 加到 list 頭。**head 是因為 head 本身可能要被改寫 */
void list_prepend(struct student_node **head, struct student_node *n);

#endif /* STUDENT_H */
