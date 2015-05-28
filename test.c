#include <stdio.h>
#include "arena.h"

int
main() {
  arena_t *arena = arena_create();
  uint8_t *it = arena_malloc(arena, 1);
  *it = 7;
  printf("%i\n", *it);
  for(int i; i < 10; i++) {
  	uint8_t *i = arena_malloc(arena, 100);
  	i[99] = 7;
  }
  it = arena_malloc(arena, 5000);
  arena_destroy(arena);
  return 0;
}
