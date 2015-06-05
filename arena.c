#include "arena.h"
#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4095

static arena_t *
_arena_create(size_t size) {
  arena_t *arena = (arena_t *) calloc(1, sizeof(arena_t));
  if(!arena) return NULL;
  arena->region = (uint8_t *) calloc(size, sizeof(uint8_t));
  arena->size   = size;
  if(!arena->region) { free(arena); return NULL; }
  return arena;
}

arena_t *
arena_create() {
  return _arena_create(PAGE_SIZE);
}

void *
arena_malloc(arena_t *arena, size_t size) {
  arena_t *last = arena;

  do {
    if((arena->size - arena->current) > size){
      arena->current += size;
      return arena->region + (arena->current - size);
    }
    last = arena;
  } while((arena = arena->next) != NULL);

  size_t asize   = size > PAGE_SIZE ? size : PAGE_SIZE;
  arena_t *next  = _arena_create(asize);
  last->next     = next;
  next->current += size;
  return next->region;
}

void
arena_destroy(arena_t *arena) {
  arena_t *next, *last = arena;
  do {
    next = last->next;
    free(last->region);
    free(last);
    last = next;
  } while(next != NULL);
}
