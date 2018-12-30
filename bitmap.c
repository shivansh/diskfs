#include "bitmap.h"

#include <stdio.h>

#define BITS_PER_WORD sizeof(word_t)

#define WORD_OFFSET(b) ((b) / BITS_PER_WORD)
#define BIT_OFFSET(b) ((b) % BITS_PER_WORD)

void set_bit(word_t *words, int n) {
    words[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n));
}

void clear_bit(word_t *words, int n) {
    words[WORD_OFFSET(n)] &= ~(1 << BIT_OFFSET(n));
}

int get_bit(word_t *words, int n) {
    return words[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n));
}
