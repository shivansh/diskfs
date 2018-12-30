#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

typedef uint32_t word_t;

void set_bit(word_t*, int);
void clear_bit(word_t*, int);
int get_bit(word_t*, int);

#endif
