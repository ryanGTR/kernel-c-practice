/* 7.8 free then use: freeing a list */
#include <stdio.h>
#include <stdlib.h>
struct node { int v; struct node *next; };
int main(void)
{
	struct node *head = NULL, *p;

	for (int i = 0; i < 3; i++) {
		p = malloc(sizeof(*p));
		p->v = i;
		p->next = head;
		head = p;
	}
	for (p = head; p; p = p->next)
		free(p);
	return 0;
}
