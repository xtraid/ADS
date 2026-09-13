#ifndef GRAPH_H
#define GRAPH_H



/*
 * we define graphs as G : (V, E)
 * we'll make 3 separate abstractions: graph vertex and edges
 */
typedef struct edge edge;

typedef struct{
  int id;
  edge *adj;
}vertex;

struct edge{
  vertex *to;
  struct edge *next;
};

typedef struct graph graph;

/*graph api*/

graph *graph_init(void);

int graph_add_vertex(graph *g, int id);
int graph_remove_vertex(graph *g, int id);

int graph_add_edge(graph *g, int from, int to);
int graph_remove_edge(graph *g, int from, int to);

void graph_drop(graph *g);


#endif

