#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

int
main() {
  arena_t *arena = arena_create();
  uint8_t *it = arena_malloc(arena, 1);
  *it = 7;
  for(int i = 0; i < 10; i++) {
    uint8_t *i = arena_malloc(arena, 1000);
    i[99] = 7;
  }
  it = arena_malloc(arena, 5000);
  it = arena_malloc(arena, 40);
  it = arena_malloc(arena, 40);
  it = arena_malloc(arena, 100);
  arena_destroy(arena);
  return 0;
}
