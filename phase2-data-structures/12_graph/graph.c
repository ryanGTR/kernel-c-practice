// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include "graph.h"

struct graph *graph_create(size_t n)
{
	/* TODO: struct + calloc(n, sizeof(struct edge *)) -- why calloc? */
	(void)n;
	return NULL;
}

int graph_add_edge(struct graph *g, int u, int v)
{
	/* TODO: bounds check; push a new edge at the head of adj[u] (O(1)) */
	(void)g;
	(void)u;
	(void)v;
	return -1;
}

int graph_add_undirected(struct graph *g, int u, int v)
{
	/* TODO: two directed edges */
	(void)g;
	(void)u;
	(void)v;
	return -1;
}

int graph_has_edge(struct graph *g, int u, int v)
{
	/* TODO: walk adj[u] */
	(void)g;
	(void)u;
	(void)v;
	return 0;
}

size_t graph_degree(struct graph *g, int u)
{
	/* TODO */
	(void)g;
	(void)u;
	return 0;
}

void graph_destroy(struct graph *g)
{
	/* TODO: free every edge list, then adj, then struct */
	(void)g;
}

size_t graph_bfs(struct graph *g, int start, int *order)
{
	/*
	 * TODO: visited[] (calloc n), a queue (reuse the ring-buffer idea
	 * from 09, or a plain int array with head/tail since at most n
	 * vertices are ever enqueued -- think about why that bound holds).
	 * Mark visited when ENQUEUED, not when dequeued. Why?
	 */
	(void)g;
	(void)start;
	(void)order;
	return 0;
}

size_t graph_dfs(struct graph *g, int start, int *order)
{
	/* TODO: recursive helper dfs_visit(g, u, visited, order, &count) */
	(void)g;
	(void)start;
	(void)order;
	return 0;
}

size_t graph_dfs_iter(struct graph *g, int start, int *order)
{
	/*
	 * TODO: explicit stack. Note the visit order will generally differ
	 * from the recursive version -- both are valid DFS orders.
	 */
	(void)g;
	(void)start;
	(void)order;
	return 0;
}

void graph_bfs_dist(struct graph *g, int start, int *dist)
{
	/* TODO (bonus): BFS with dist[v] = dist[u] + 1 */
	(void)g;
	(void)start;
	(void)dist;
}

int graph_has_cycle(struct graph *g)
{
	/* TODO (bonus): three-colour DFS (white/grey/black); grey->grey = cycle */
	(void)g;
	return 0;
}

size_t graph_components(struct graph *g)
{
	/* TODO (bonus): count how many times you have to restart a traversal */
	(void)g;
	return 0;
}
