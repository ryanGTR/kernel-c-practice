// struct 的四件事：值語意、指標、-> 、記憶體版面
// gcc -Wall -Wextra -Werror struct_demo.c -o struct_demo && ./struct_demo

#include <stdio.h>

struct point {
	int x;
	int y;
};

/* 傳值：拿到的是「複製品」，改了外面看不到 */
static void move_by_value(struct point p)
{
	p.x = 99;
	printf("  （函式內看到的 p.x=%d，但它是複製品）\n", p.x);
}

/* 傳指標：拿到的是「本體的地址」，改了外面看得到 */
static void move_by_pointer(struct point *p)
{
	p->x = 99;		/* (*p).x 的簡寫 */
}

/* 這一個示範 padding：sizeof 不等於各欄位相加 */
struct padded {
	char  c;		/* 1 byte，後面被塞 3 bytes 的洞 */
	int   n;		/* 4 bytes，要對齊到 4 的倍數 */
	char  tail;		/* 1 byte，結尾再補 3 bytes */
};

int main(void)
{
	struct point a = { .x = 1, .y = 2 };	/* 指定初始化，欄位多時比較好讀 */
	struct point b = a;			/* ⭐ 整包複製，不是取別名 */

	b.x = 7;
	printf("複製之後  a.x=%d  b.x=%d\n", a.x, b.x);

	move_by_value(a);
	printf("傳值之後  a.x=%d   ← 沒變\n", a.x);

	move_by_pointer(&a);
	printf("傳指標後  a.x=%d  ← 變了\n", a.x);

	printf("sizeof(struct point)  = %zu\n", sizeof(struct point));
	printf("sizeof(struct padded) = %zu（1+4+1=6，但實際不是 6）\n",
	       sizeof(struct padded));

	return 0;
}
