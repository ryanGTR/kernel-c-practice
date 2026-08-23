// SPDX-License-Identifier: GPL-2.0
/* RED against the stubs on purpose -- see ../../TODO.md. */
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include "graph.h"

/*
 *   0 -- 1 -- 3
 *   |    |
 *   2 -- 4       5 (isolated)      6 -> 7 (directed only, separate graph)
 */
static struct graph *build_undirected(void)
{
	struct graph *g = graph_create(6);

	assert(g);
	assert(graph_add_undirected(g, 0, 1) == 0);
	assert(graph_add_undirected(g, 0, 2) == 0);
	assert(graph_add_undirected(g, 1, 3) == 0);
	assert(graph_add_undirected(g, 1, 4) == 0);
	assert(graph_add_undirected(g, 2, 4) == 0);
	return g;
}

static int contains(const int *a, size_t n, int x)
{
	for (size_t i = 0; i < n; i++)
		if (a[i] == x)
			return 1;
	return 0;
}

static int all_distinct(const int *a, size_t n)
{
	for (size_t i = 0; i < n; i++)
		for (size_t j = i + 1; j < n; j++)
			if (a[i] == a[j])
				return 0;
	return 1;
}

static void test_edges(void)
{
	struct graph *g = build_undirected();

	assert(graph_has_edge(g, 0, 1) && graph_has_edge(g, 1, 0));
	assert(!graph_has_edge(g, 0, 3));
	assert(graph_degree(g, 1) == 3);
	assert(graph_degree(g, 5) == 0);
	assert(graph_add_edge(g, 0, 99) == -1);		/* out of range */
	assert(graph_add_edge(g, -1, 0) == -1);
	graph_destroy(g);
	graph_destroy(NULL);
	printf("edges ok\n");
}

static void test_bfs(void)
{
	struct graph *g = build_undirected();
	int order[6];
	size_t n;

	n = graph_bfs(g, 0, order);
	assert(n == 5);				/* 5 is unreachable */
	assert(order[0] == 0);
	assert(all_distinct(order, n));
	assert(!contains(order, n, 5));
	/* BFS layers: {1,2} must both come before {3,4} */
	assert(contains(order + 1, 2, 1) && contains(order + 1, 2, 2));
	assert(contains(order + 3, 2, 3) && contains(order + 3, 2, 4));

	n = graph_bfs(g, 5, order);
	assert(n == 1 && order[0] == 5);
	graph_destroy(g);
	printf("bfs ok\n");
}

static void test_dfs(void)
{
	struct graph *g = build_undirected();
	int order[6];
	size_t n;

	n = graph_dfs(g, 0, order);
	assert(n == 5 && order[0] == 0 && all_distinct(order, n));
	assert(!contains(order, n, 5));

	n = graph_dfs_iter(g, 0, order);
	assert(n == 5 && order[0] == 0 && all_distinct(order, n));
	graph_destroy(g);
	printf("dfs ok\n");
}

static void test_directed(void)
{
	struct graph *g = graph_create(3);
	int order[3];

	/* 0 -> 1 -> 2, nothing back */
	assert(graph_add_edge(g, 0, 1) == 0);
	assert(graph_add_edge(g, 1, 2) == 0);
	assert(graph_has_edge(g, 0, 1) && !graph_has_edge(g, 1, 0));
	assert(graph_bfs(g, 0, order) == 3);
	assert(graph_bfs(g, 2, order) == 1);	/* dead end */
	graph_destroy(g);
	printf("directed ok\n");
}

int main(void)
{
	printf("=== Graph Tests ===\n");
	test_edges();
	test_bfs();
	test_dfs();
	test_directed();

	/* TODO (bonus): graph_bfs_dist from 0: {0,1,1,2,2,-1} */
	/* TODO (bonus): graph_has_cycle on 0->1->2->0 (1) and 0->1->2 (0) */
	/* TODO (bonus): graph_components on build_undirected() == 2 */

	printf("\nAll tests passed!\n");
	return 0;
}
