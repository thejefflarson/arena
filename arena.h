#ifndef ARENA_H
#define ARENA_H
/*
 * arena.h — bump-pointer arena allocator
 *
 * Safety contract (finding [low, line 1]):
 *   - arena_malloc() returns NULL on allocation failure; always check the result.
 *   - Passing a NULL arena to arena_malloc() returns NULL immediately.
 *   - Allocations of size 0 return NULL.
 *   - ALL pointers vended by arena_malloc() become dangling (undefined behaviour)
 *     after arena_destroy() is called — do not access them afterwards.
 *   - This allocator is NOT thread-safe; external synchronisation is required
 *     for concurrent access to the same arena.
 */
/* Finding [medium, line 4]: add <stddef.h> to guarantee size_t is declared;
   replace non-portable <memory.h> with the standard <string.h>. */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct arena {
  uint8_t *region;
  size_t size;
  size_t current;
  struct arena *next;
} arena_t;

/* Finding [low, line 17]: use (void) instead of empty () to match the C23
   requirement and prevent callers from passing undeclared arguments. */
arena_t *
arena_create(void);

void *
arena_malloc(arena_t *arena, size_t size);

void
arena_destroy(arena_t *arena);

#ifdef __cplusplus
}
#endif

#endif
