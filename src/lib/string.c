#include <stddef.h>

int str_cmp(char *str1, char *str2) {
    size_t len2 = 0;
    while (str2[len2]) {
        len2++;
    }

    size_t len1 = 0;
    while (str1[len1]) {
        len1++;
    }

    if(len1 != len2) return 0;

    for(size_t i = 0; i < len1; i++) {
        if(str1[i] != str2[i]) return 0;
    }
    return 1;
}

char *strcpy(char *dest, const char *src)
{
    char *orig = dest;

    while ((*dest++ = *src++) != '\0')
        ;
    return orig;
}