#include <stdio.h>
#include "arena.h"

int
main() {
  arena_t *arena = arena_create();
  uint8_t *it = arena_malloc(arena, sizeof(uint8_t));
  *it = 7;
  printf("%i\n", *it);
  return 0;
}