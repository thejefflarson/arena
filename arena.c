#include "arena.h"
#include <stdlib.h>

#define PAGE_SIZE 4096

arena_t *
arena_create() {
  arena_t *arena = (arena_t *)calloc(1, sizeof(arena));
  if(!arena) return NULL;
  arena->region = (uint8_t *)calloc(PAGE_SIZE, sizeof(uint8_t));
  arena->size   = PAGE_SIZE;
  if(!arena->region) {free(arena); return NULL; }
  return arena;
}

void *
arena_malloc(arena_t *arena, size_t size) {
  arena_t *last = arena;
  do {
    if((arena->size - arena->current) > size){
      arena->current += size;
      return arena->region + (arena->size - arena->current);
    }
    last = arena;
  } while((arena = arena->next));
}

void
arena_destroy(arena_t *arena) {

}