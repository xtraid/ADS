#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h> // i nee min :')

typedef struct ghash ghash;

/*
 * policy ownership of vertex:
 * graph duild the vertex instance in memory
 * graph forward the storage of the vertex to the hashmap
 * graph is responsible of destroying vertex instance
 *
 */






/* hashmap private struct  and constants */

#define MIN_BUCKET_NUMBER 8
#define MIN_BUCKET_SIZE 8
#define HMAP_CONST 2654435761u
#define MAX_LOAD_FACTOR 0.75
#define MIN_LOAD_FACTOR 0.15

typedef struct {
  uint32_t key;
  vertex *value;
}entry;


typedef struct {
  entry * bucket;
  int capacity;
  int size;
}gbucket;

struct ghash {
  gbucket *hashmap;
  int bucket_count;
  int total_entries;
};

/* hash map api */

static ghash * ghash_init (void);
static int ghash_insert (ghash *hmap, entry element);
static int bucket_resize(gbucket * b);
static int bucket_insert(gbucket *b, entry element);
static int drop_map (ghash *hmap);
static int drop_bucket_array(gbucket *map, int count);
static int hmap_resize (ghash * hmap);
static int hmap_search(gbucket *b, uint32_t key);
static vertex * hmap_get (ghash *hmap, uint32_t key);
static int entry_remove(ghash *hmap, uint32_t key);


/*
 * graph struct definition is opaque
 * it preserve the manipulation of ownership of vertex.
 *
 * definition of private api and costant for  edge index and graph
 */


#define MIN_EDGE_SIZE 8

typedef struct {
  edge *index;
  int cap;
  int size;
} eidx;

struct graph {
    ghash *vertices;
    eidx *edges;
};

/* api index */

static eidx * eindex_init(void);
static int eindex_drop(eidx *idx);
static int eindex_resize(eidx *idx);
static int eindex_bsearc (eidx *idx, edge item);
static int eindex_insert (eidx *idx, edge item);
static int eindex_delete(eidx *idx, edge item);
static int eindex_find_i (eidx *idx, vertex *node);

graph *graph_init(void){
  graph *g = malloc(sizeof(graph));
  if(!g){
    perror("malloc");
    return NULL;
  }
  g->vertices = ghash_init();
  if (!g->vertices){
      free(g);
      return NULL;
  }
  g->edges = eindex_init();
  if (!g->edges){
    free(g->vertices);
    free(g);
    return NULL;
  }
  return g;
}



int graph_add_vertex(graph *g, int id){ // prety wrapper over the hashmap love to see it <3
  if (!g)
    return -1;
  vertex *v = malloc(sizeof(*v));
  if (!v){
    perror("malloc");
    return -1;
  }
  *v = (vertex){ .id = id }; // magical syntax 
  int err = ghash_insert(g->vertices, (entry){.key = id, .value = v}); //more black magic
  if (err != 0) {
      free(v);
      return err;
  }
  return 0;
}


int graph_drop(graph *g){
  if(!g)
    return -1;
  int err = eindex_drop(g->edges);
  if (err != 0)
    return err;

  err = drop_map(g->vertices);
  if (err != 0)
    return err;

  free(g);
  return 0;
}

int graph_remove_vertex(graph *g, int id){
  if (!g)
    return -1;
  int err = entry_remove(g->vertices, (uint32_t)id);
  return err;
}


int graph_add_edge(graph *g, int from, int to){ // very big abstraction here i take 2 int make a struct get of the pointers of the corresponding iont in the hashmap, but graph doesnt know or care cool
  if (!g)
    return -1;
  vertex *vfrom = hmap_get(g->vertices, (uint32_t)from);
  if (!vfrom)
    return -2;
  vertex *vto = hmap_get(g->vertices, (uint32_t)to);
  if (!vto)
    return -2;
  edge item;
  item.from = vfrom;
  item.to = vto;
  int err = eindex_insert(g->edges, item);
  if (err != 0){
    return err;
  }
  return 0;
}


int graph_remove_edge(graph *g, int from, int to){
  if (!g)
    return -1;
  vertex *vfrom = hmap_get(g->vertices, (uint32_t)from);
  if (!vfrom)
    return -2;
  vertex *vto = hmap_get(g->vertices, (uint32_t)to);
  if (!vto)
    return -2;
  edge item;
  item.from = vfrom;
  item.to = vto;
  int err = eindex_delete(g->edges, item);
  return err;
}



/*edges index helperx */
static eidx *eindex_init(void){
  eidx *idx = malloc(sizeof(eidx));
  if(!idx){
    perror("malloc");
    return NULL;
  }
  idx->index = malloc(sizeof(edge) * MIN_EDGE_SIZE);
  if(!idx->index){
    perror("malloc");
    free(idx);
    return NULL;
  }
  idx->size = 0;
  idx->cap = MIN_EDGE_SIZE;
  return idx;
}

static int eindex_drop(eidx *idx){
  if (!idx)
    return -1;
  free(idx->index);
  free(idx);
  return 0;
}

static int eindex_resize(eidx *idx){
  if(!idx)
    return -1;
  int new_cap;
  if(idx->cap == idx->size)
    new_cap = idx->cap * 2;
  else if (idx->cap >= 4 * idx->size && idx->cap > MIN_EDGE_SIZE)
    new_cap = idx->cap / 2;
  else
      return 0;
  edge *new_index = malloc (sizeof(edge) * new_cap);
  if (!new_index){
    perror("malloc");
    return -2;
  }
  memcpy(new_index, idx->index, (size_t)idx->size * sizeof(edge));
  free(idx->index);
  idx->index = new_index;
  idx->cap = new_cap;
  return 0;
}

static int eindex_bsearc (eidx *idx, edge item){
  if (!idx)
    return -1;
  int s = 0;
  int n = idx->size;
  int id = item.from->id;
  while (s < n){
    int m = s + ((n - s) / 2);
    int lookup = idx->index[m].from->id;
    if (lookup < id )
      s = m + 1;
    else if (lookup > id)
      n = m; // lower bound :)
    else if (idx->index[m].to->id < item.to->id) //found out that you can stack else if cool 
      s = m + 1;
    else
      n = m;
  }
  return s;
}

static int eindex_insert (eidx *idx, edge item){
  if (!idx)
    return -1;
  int pos = eindex_bsearc(idx, item);
  if (pos < idx->size && idx->index[pos].from->id == item.from->id &&
    idx->index[pos].to->id == item.to->id )
    return -3;
  if (idx->size == idx->cap){
    int err = eindex_resize(idx);
    if (err != 0)
      return err;
  }
  memmove(&idx->index[pos + 1], &idx->index[pos], (idx->size - pos) * sizeof(edge)); // not that i cant wriet a helper but i  need to befriend wih library optimize functions and here semed the right spot
  idx->index[pos] = item;
  idx->size ++;
  return 0;
}

static int eindex_delete(eidx *idx, edge item){
  if(!idx)
    return -1;
  int pos = eindex_bsearc(idx, item);
  if( pos >= idx->size || idx->index[pos].from->id != item.from->id ||
    idx->index[pos].to->id != item.to->id )
    return -3; // \not (a \land b) <=> \not a \lor \lnot b ty DeMorgan <3
  memmove(&idx->index[pos], &idx->index[pos +1], (idx->size - pos -1) * sizeof(edge));
  idx->size--;
  return 0;
}

static int eindex_find_i (eidx *idx, vertex *node){
  if(!idx || !node)
    return -1;
  vertex dummy;
  dummy.id = INT_MIN;
  edge item;
  item.from = node;
  item.to = &dummy;
  return eindex_bsearc(idx, item);
}


/* hash map private helper */

/*
 * define struct hash map for storing the graph vertexes
 */



static int drop_bucket_array(gbucket *map, int count){
    if (!map)
        return -1;

    for (int i = 0; i < count; i++) {
        free(map[i].bucket);
    }

    free(map);
  return 0;
}


static int drop_map (ghash *hmap){
  if (!hmap)
    return -1;
  for(int i = 0;i < hmap->bucket_count; i++){
    free(hmap->hashmap[i].bucket); // is 4 am and this thing is a trip i was confused if i had to free vertexes as well
  }
  free(hmap->hashmap);
  free(hmap);
  return 0;
}

static ghash * ghash_init (void){
  ghash * hmap = malloc(sizeof(ghash));
  if (!hmap){
    perror("malloc");
    return NULL;
  }
  hmap->bucket_count = MIN_BUCKET_NUMBER;
  hmap->total_entries = 0;
  hmap->hashmap =  calloc(MIN_BUCKET_NUMBER, sizeof(gbucket));
  if (!hmap->hashmap){
    perror("hash map calloc");
    free(hmap);
    return NULL;
  }
  return hmap;
}


static int bucket_insert(gbucket *b, entry element){
  if (!b)
    return -1;
  if (!b->bucket){
    b->bucket = malloc(sizeof(entry)* MIN_BUCKET_SIZE);
    if (!b->bucket){
      perror("malloc");
      return -2;
    }
    b->size = 0;
    b->capacity = MIN_BUCKET_SIZE;
  }
  if (b->size == b->capacity){
    int err = bucket_resize(b);
    if (err != 0)
      return err;
  }
  b->bucket[b->size] = element;
  b->size ++;
  return 0;
}

static int bucket_resize(gbucket * b){
  if (!b)
    return -1;
  int new_capacity;
  if (b->size == b->capacity)
    new_capacity = b->capacity * 2;
  else if ( b->capacity >= 4 * b->size && b->capacity > MIN_BUCKET_SIZE)
    new_capacity = b->capacity /2;
  else
    return 0;
  entry *new_bucket = malloc(sizeof(entry) * new_capacity);
  if (!new_bucket){
    perror("malloc");
    return -2;
  }
  memcpy(new_bucket, b->bucket, (size_t)b->size * sizeof(entry));
  free(b->bucket);
  b->bucket = new_bucket;
  b->capacity = new_capacity;
  return 0;
}



/*update i started writing this function at 2 am and i ened up writing 3 support functions and is 5 am
 * whata trip reallocating hash maps
 */
static int hmap_resize (ghash * hmap){
  if (!hmap)
    return -1;
  int new_total;
  gbucket *new_map;
  double alpha = (double)(hmap->total_entries + 1) / hmap->bucket_count;
  if (alpha >= MAX_LOAD_FACTOR){
    new_total = hmap->bucket_count * 2;
   new_map = calloc(new_total, sizeof(gbucket));// were risking ub when free if errors occur
    if (!new_map){
      perror("calloc");
      return -2;
    }
    for (int i = 0; i < hmap->bucket_count;i ++){
      for (int j = 0; j < hmap->hashmap[i].size; j++){
        entry element = hmap->hashmap[i].bucket[j];
        int new_hash = (element.key * HMAP_CONST) % new_total;
        int err = bucket_insert(&new_map[new_hash], element);
        if (err!= 0){
          int droperr = drop_bucket_array(new_map, new_total); // writing drop map
          if(droperr != 0)
          return 10*err +droperr;
        return err;
        }
      }
    }
  }
  else if ((double)(hmap->total_entries) / hmap->bucket_count<= MIN_LOAD_FACTOR && hmap->bucket_count > MIN_BUCKET_NUMBER){
    new_total = hmap->bucket_count / 2;
    if (new_total < MIN_BUCKET_NUMBER)
      new_total = MIN_BUCKET_NUMBER;
    new_map = calloc(new_total, sizeof(gbucket));// were risking ub when free if errors occur
    if (!new_map){
      perror("calloc");
      return -2;
    }
    for (int i = 0; i < hmap->bucket_count;i ++){
      for (int j = 0; j < hmap->hashmap[i].size; j++){
        entry element = hmap->hashmap[i].bucket[j];
        int new_hash = (element.key * HMAP_CONST) % new_total;
        int err = bucket_insert(&new_map[new_hash], element);
        if (err!= 0){
          int droperr = drop_bucket_array(new_map, new_total); // writing drop map
          if(droperr != 0)
          return 10*err +droperr;
        return err;
        }
      }
    }
  }
  else
    return 0;
  gbucket *old_map = hmap->hashmap;
  hmap->hashmap = new_map;
  int old_count = hmap->bucket_count;
  hmap->bucket_count = new_total;
  int err = drop_bucket_array(old_map, old_count);
  if (err != 0){
    fprintf(stderr, "Warning, resize of hashmap went through but the drop of the previus hashmap failled");
  }
  return 0;
}







static int ghash_insert (ghash *hmap, entry element){
  if (!hmap)
    return -1;
  if (!element.value){
    fprintf(stderr, "Error: no vertex found");
    return -1;
  }
  int hash_index = (element.key * HMAP_CONST) % hmap->bucket_count;
  if(hmap_search(&hmap->hashmap[hash_index],element.key) != -3){
    fprintf(stderr, "Error: graph hash table insert failed because the element has a key alreasy saved ");
    return -3;
  
  }
  if((double)(hmap->total_entries + 1) / hmap->bucket_count >= MAX_LOAD_FACTOR){
    int err =hmap_resize (hmap);
    if (err != 0)
      return err;
    hash_index = (element.key * HMAP_CONST) % hmap->bucket_count; // need to be updated cos resize
  }
  int err = bucket_insert(&hmap->hashmap[hash_index], element);
  if (err != 0)
    return err;
  hmap->total_entries++;
  return 0;
}


static int hmap_search(gbucket *b, uint32_t key){
  if (!b)
    return -1;
  for(int i = 0; i < b->size; i++){
    if(b->bucket[i].key == key)
      return i;
  }
  return -3;
}

static vertex * hmap_get (ghash *hmap, uint32_t key){
  if(!hmap)
    return NULL;
  int hash = (key * HMAP_CONST) % hmap->bucket_count;
  int id = hmap_search(&hmap->hashmap[hash], key);
  if (id < 0)
    return NULL;
  else
    return hmap->hashmap[hash].bucket[id].value;
}


static int entry_remove(ghash *hmap, uint32_t key){
  if(!hmap)
    return -1;
  int hash = (key * HMAP_CONST) % hmap->bucket_count;
  int id = hmap_search(&hmap->hashmap[hash], key);
  if (id < 0)
    return id;
  hmap->hashmap[hash].bucket[id] = hmap->hashmap[hash].bucket[hmap->hashmap[hash].size - 1];
  hmap->hashmap[hash].size --;
  hmap->total_entries --;
  return 0;
}
