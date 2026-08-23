/* SPDX-License-Identifier: GPL-2.0 */
#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

/*
 * Adjacency list: vertex u's neighbours are a singly linked list of
 * struct edge hanging off adj[u]. Vertices are 0..n-1.
 *
 *   adj[0] -> (1) -> (2) -> NULL
 *   adj[1] -> (3) -> NULL
 *   ...
 */
struct edge {
	int to;
	struct edge *next;
};

struct graph {
	size_t n;
	struct edge **adj;	/* array of n list heads */
};

struct graph *graph_create(size_t n);
/* directed edge u -> v; for undirected call twice (or use the helper) */
int graph_add_edge(struct graph *g, int u, int v);	/* 0 ok, -1 bad/alloc */
int graph_add_undirected(struct graph *g, int u, int v);
int graph_has_edge(struct graph *g, int u, int v);
size_t graph_degree(struct graph *g, int u);		/* out-degree */
void graph_destroy(struct graph *g);

/*
 * Traversals write the visit order into `order` (caller provides n ints)
 * and return how many vertices were reached from `start`.
 */
size_t graph_bfs(struct graph *g, int start, int *order);
size_t graph_dfs(struct graph *g, int start, int *order);	/* recursive */
size_t graph_dfs_iter(struct graph *g, int start, int *order);	/* own stack */

/* Bonus */
/* shortest hop count from start to every vertex; -1 if unreachable */
void graph_bfs_dist(struct graph *g, int start, int *dist);
int graph_has_cycle(struct graph *g);		/* directed cycle detection */
size_t graph_components(struct graph *g);	/* treat as undirected */

#endif /* GRAPH_H */
