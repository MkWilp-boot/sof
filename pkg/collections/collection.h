#ifndef SOF_ARRAY_H
#define SOF_ARRAY_H

#define MINIMAL_CAP 64

#include <stddef.h>

typedef struct {
    unsigned char *ptr;
    size_t elements_size;
    size_t len;
    size_t cap;
} array_t;

#endif // SOF_ARRAY_H