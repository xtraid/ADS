#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct ghash ghash;







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




struct graph {
    ghash *vertices;
};


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
  return g;
}





/*hash map private helper */

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
