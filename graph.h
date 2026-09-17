#ifndef GRAPH_H
#define GRAPH_H



/*
 * we define graphs as G : (V, E)
 * we'll make 3 separate abstractions: graph vertex and edges
 */


/*
 * description of edges: theyll be stored in a
 * index owned by graph 
 */

typedef struct edge edge;

typedef struct{
  int id;
}vertex;

struct edge{
  vertex *to;
  vertex *from;
};

typedef struct graph graph;

/*graph api*/

graph *graph_init(void);

int graph_add_vertex(graph *g, int id);
int graph_remove_vertex(graph *g, int id);
int graph_drop(graph *g);

int graph_add_edge(graph *g, int from, int to);
int graph_remove_edge(graph *g, int from, int to);




#endif

