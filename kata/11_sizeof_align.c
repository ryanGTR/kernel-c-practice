// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 11  sizeof/對齊     struct 有多大？編譯器偷塞了什麼？
 * @fn: main
 *
 * 題目來源：huihut/interview「sizeof()」「編譯器擴展與標準對齊控制」「位域」「union」
 *          （那邊只有文字說明；這裡交給編譯器對答案）
 *
 * 這題沒有函式要寫。你要寫的是**預測**：
 *   把下面 PRED_* 每一個 -1 改成你猜的 byte 數，然後 make N=11。
 *   猜錯的會一行一行列出來。全對才綠。
 *
 * 規矩（比其他題更嚴）：
 *   ① 先全部填完再跑。**不准填一個跑一次**——那叫試誤，不叫預測。
 *   ② 第一次跑的結果（對幾題）寫進回報，那是你的真實分數。
 *   ③ 猜錯的每一題，寫一句「差在哪」。
 *
 * make style N=11 會報 1 個 WARNING「Prefer __packed」，指向 #define __packed 那一行。
 * 那是**誤報**：kernel 自己就是這樣定義 __packed 的，user-space 沒有現成的只好自己寫。
 *
 * 環境：x86_64 + gcc（char 1、short 2、int 4、double 8、指標 8）。
 *
 * 動手前先答：
 *   Q1. 🔥 struct c3 和 struct b3 放的是同樣三個欄位，只是順序不同。
 *       大小會一樣嗎？
 *       A:
 *   Q2. Java 的物件欄位順序跟你宣告的一樣嗎？C 的呢？
 *       （Java 類比：JVM 會自己重排欄位省空間，C 編譯器**絕對不會**。為什麼 C 不准它排？）
 *       A:
 *   Q3. union 的大小，是「最大成員」的大小嗎？一定嗎？
 *       A:
 *   Q4. 為什麼 kernel 開發者在意這個？（提示：一個 struct page 省 8 bytes，
 *       乘上系統裡幾百萬個 page。工具叫 pahole。）
 *       A:
 *
 * 記憶體圖：把 struct c3 一格一格畫出來（每格 1 byte，標出哪些是 padding）。
 *
 *
 */
#include <stddef.h>
#include <stdio.h>

/* kernel 的寫法（include/linux/compiler_attributes.h）；user-space 沒有，自己定義 */
#define __packed __attribute__((packed))

struct a2 { char c; int i; };
struct b3 { char a; char b; int i; };
struct c3 { char a; int i; char b; };
struct d2 { char c; double d; };
struct e2 { int i; char c; };
struct f1 { char c[5]; };
struct g2 { short s; char c; };
union  h2 { char c[5]; int i; };
struct __packed i2 { char c; int i; };
struct j3 { unsigned int a : 3; unsigned int b : 5; unsigned int c : 1; };

/* ===== 你的預測：把 -1 改掉 ===== */
#define PRED_A		-1	/* sizeof(struct a2) */
#define PRED_B		-1	/* sizeof(struct b3) */
#define PRED_C		-1	/* sizeof(struct c3) */
#define PRED_D		-1	/* sizeof(struct d2) */
#define PRED_E		-1	/* sizeof(struct e2)   🔥 尾巴 */
#define PRED_F		-1	/* sizeof(struct f1) */
#define PRED_G		-1	/* sizeof(struct g2) */
#define PRED_H		-1	/* sizeof(union h2)    🔥 */
#define PRED_I		-1	/* sizeof(struct i2)   packed */
#define PRED_J		-1	/* sizeof(struct j3)   位域 */
#define PRED_OFF_C_B	-1	/* offsetof(struct c3, b) */
/* ================================= */

static int wrong;

static void check(const char *what, long got, long pred)
{
	if (got == pred) {
		printf("  ok    %-28s = %ld\n", what, got);
		return;
	}
	printf("  WRONG %-28s 你猜 %ld\n", what, pred);
	wrong++;
}

int main(void)
{
	check("sizeof(struct a2)", sizeof(struct a2), PRED_A);
	check("sizeof(struct b3)", sizeof(struct b3), PRED_B);
	check("sizeof(struct c3)", sizeof(struct c3), PRED_C);
	check("sizeof(struct d2)", sizeof(struct d2), PRED_D);
	check("sizeof(struct e2)", sizeof(struct e2), PRED_E);
	check("sizeof(struct f1)", sizeof(struct f1), PRED_F);
	check("sizeof(struct g2)", sizeof(struct g2), PRED_G);
	check("sizeof(union h2)", sizeof(union h2), PRED_H);
	check("sizeof(struct i2)", sizeof(struct i2), PRED_I);
	check("sizeof(struct j3)", sizeof(struct j3), PRED_J);
	check("offsetof(struct c3, b)", offsetof(struct c3, b), PRED_OFF_C_B);

	if (wrong) {
		/* 故意不印正確答案：錯的那題要你自己推出為什麼 */
		printf("kata 11: %d 題猜錯（見上面 WRONG）\n", wrong);
		printf("  ^ 別改數字重跑。先把錯的那題畫成記憶體圖，想通了再改。\n");
		return 1;
	}
	printf("kata 11: PASS（全部預測正確）\n");
	return 0;
}
