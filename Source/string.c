#include <string.h>

void string_copy(char *dest, const char* src) {
    while (*src) {      // while source string isnt null, copy, then increase pointer.
        *dest = *src;
        dest++;
        src++;
    }
}

int string_compare(const char* a, const char* b) {
    while (*a && *a == *b) {
         ++a; ++b; 
    }
    
    return (int)(unsigned char)(*a) - (int)(unsigned char)(*b);
}
