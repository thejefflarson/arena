#ifndef ARENA_H
#define ARENA_H
#include <stdint.h>
#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct arena {
  uint8_t *region;
  size_t size;
  size_t current;
  struct arena *next;
} arena_t;

arena_t *
arena_create();

void *
arena_malloc(arena_t *arena, size_t size);

void
arena_destroy(arena_t *arena);

#ifdef __cplusplus
}
#endif

#endif
