#include <stdio.h>

int main(void) {

  int x = 42;
  int *p = &x;
  printf("x       = %d\n", x);          /* 變數的值 */
  printf("&x      = %p\n", (void *)&x); /* 變數的位址 */
  printf("p       = %p\n", (void *)p);  /* pointer 的值（就是 &x） */
  printf("*p      = %d\n", *p);         /* pointer 指到的值 */
  printf("&p      = %p\n", (void *)&p); /* pointer 自己住的位址 */
  return 0;
}
