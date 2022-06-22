#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

size_t Str_getLength(const char *pcSrc)
{
   const char *pcEnd;
   assert(pcSrc != NULL);
   pcEnd = pcSrc;
   while (*pcEnd != '\0')
      pcEnd++;
   return (size_t)(pcEnd - pcSrc);
}

char *Str_copy(char *pcDest, const char *pcSrc) {
    assert(pcSrc != NULL);
    assert(pcDest != NULL);

    char *desPtr = pcDest;

    while(*pcSrc != '\0') {
        *desPtr = *pcSrc;
        desPtr++;
        pcSrc++;
    }

    *desPtr = '\0';
    return pcDest;
}

char *Str_concat(char *pcDest, const char *pcSrc) {
    assert(pcSrc != NULL);
    assert(pcDest != NULL);

    size_t last = Str_getLength(pcDest);

    char *desPtr = pcDest + last;
    while(*pcSrc != '\0') {
        *desPtr =  *pcSrc;
        desPtr++;
        pcSrc++;
    }

    *desPtr = '\0';
    return pcDest;
}

int Str_compare (const char *str1, const char *str2) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    while(*str1 != '\0' && *str2 != '\0') {
        int result = *str1 - *str2;
        if(result != 0){
            return result;
        }
        str1++;
        str2++;
    }

    if(*str1 == '\0' && *str2 == '\0') {
        return 0;
    }
    else if(*str1 == '\0') {
        return -1;
    }
    else {
        return 1;
    }
}

char *Str_search (const char *str1, const char *str2) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    if(*str2 == '\0') {
        return str1;
    }

    size_t length1 = Str_getLength(str1);
    size_t length2 = Str_getLength(str2);

    if(length1 < length2) { return NULL;}
    
    size_t target = length1 - length2;
    size_t start = 0;
    while(start <= target) {
        const char *curr1 = str1;
        const char *curr2 = str2;
        while(*curr2 != '\0'){
          if(*curr1 != *curr2) {
            break;
          }
          else {
            curr1++;
            curr2++;
          }
        }
        if(*curr2 == '\0'){
          return str1;
        }

        str1++;
        start++;
    }

    return NULL;
}
