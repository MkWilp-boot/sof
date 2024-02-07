#ifndef SOF_STRINGS_H
#define SOF_STRINGS_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char *trim_whitespace(char *str) {
    //Trim leading space
    size_t chop_size = 0;
    
    while(isspace((unsigned char)*str)) {
        str++;
        chop_size++;
    }
    size_t size = sizeof(char)*strlen(str);
    char *copy_str = malloc(size);
    
    strcpy(copy_str, str);
    free(str-chop_size);

    return copy_str;
}
#endif