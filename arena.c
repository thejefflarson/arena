#include "arena.h"
#include <stdio.h>
#include <stdlib.h>

/* Finding [medium, line 5]: renamed from PAGE_SIZE (collides with POSIX macro)
   and corrected from 4095 to 4096. */
#define ARENA_PAGE_SIZE 4096

/* Finding [high, line 34] / [medium, line 11]: hard upper bound prevents
   oversized allocations, overcommit DoS, and SIZE_MAX arithmetic overflows. */
#define ARENA_MAX_ALLOC (SIZE_MAX / 2)

/* Finding [high, line 28]: round sz up to the platform's strictest alignment
   requirement so every vended pointer is valid for any scalar type. */
static size_t
_align_size(size_t sz) {
  size_t align = _Alignof(max_align_t);
  size_t rem   = sz % align;
  return rem == 0 ? sz : sz + (align - rem);
}

static arena_t *
_arena_create(size_t size) {
  /* Finding [medium, line 11]: reject zero and oversized requests before
     passing to calloc — prevents overcommit DoS on over-committing kernels. */
  if (size == 0 || size > ARENA_MAX_ALLOC) return NULL;
  arena_t *arena = (arena_t *) calloc(1, sizeof(arena_t));
  if (!arena) return NULL;
  arena->region = (uint8_t *) calloc(size, sizeof(uint8_t));
  arena->size   = size;
  if (!arena->region) { free(arena); return NULL; }
  return arena;
}

arena_t *
arena_create(void) {
  return _arena_create(ARENA_PAGE_SIZE);
}

void *
arena_malloc(arena_t *arena, size_t size) {
  /* Finding [medium, line 23]: guard against NULL arena argument. */
  if (!arena) return NULL;

  /* Finding [high, line 34]: reject pathological sizes before any arithmetic. */
  if (size == 0 || size > ARENA_MAX_ALLOC) return NULL;

  /* Finding [high, line 28]: work with alignment-padded size throughout so
     every returned pointer satisfies the strictest platform alignment. */
  size_t asize = _align_size(size);
  if (asize < size || asize > ARENA_MAX_ALLOC) return NULL; /* overflow */

  arena_t *last = arena;

  do {
    /* Finding [high, line 27]: use addition-based guard to avoid size_t
       underflow when arena->current > arena->size. */
    if (arena->current + asize <= arena->size) {
      void *ptr = arena->region + arena->current;
      arena->current += asize;
      return ptr;
    }
    last = arena;
  } while ((arena = arena->next) != NULL);

  size_t chunk  = asize > ARENA_PAGE_SIZE ? asize : ARENA_PAGE_SIZE;
  arena_t *next = _arena_create(chunk);

  /* Finding [high, line 37]: check _arena_create() return value before
     dereferencing; propagate NULL on allocation failure. */
  if (!next) return NULL;

  last->next    = next;
  void *ptr     = next->region;
  next->current = asize;
  return ptr;
}

void
arena_destroy(arena_t *arena) {
  arena_t *next, *last = arena;
  do {
    next = last->next;
    free(last->region);
    /* Finding [high, line 42]: zero every field before freeing the arena_t
       node so any surviving pointer to it is visibly invalid (crash-fast)
       rather than silently stale (use-after-free). */
    last->region  = NULL;
    last->size    = 0;
    last->current = 0;
    last->next    = NULL;
    free(last);
    last = next;
  } while (next != NULL);
}
