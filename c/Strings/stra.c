#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

size_t Str_getLength(const char pcSrc[])
{
    assert(pcSrc != NULL);
    size_t uLength = 0;
    while (pcSrc[uLength] != '\0')
        uLength++;
    return uLength;
}

char *Str_copy(char pcDest[], const char pcSrc[]) {
    assert(pcSrc != NULL);
    assert(pcDest != NULL);
    int dest = 0, src = 0;

    while(pcSrc[src] != '\0') {
        pcDest[dest] = pcSrc[src];
        dest++;
        src++;
    }
    pcDest[dest] = '\0';
    return pcDest;
}

char *Str_concat(char pcDest[], const char pcSrc[]) {
    assert(pcSrc != NULL);
    assert(pcDest != NULL);

    size_t last = Str_getLength(pcDest);
    size_t srcPtr = 0;
    while(pcSrc[srcPtr] != '\0') {
        pcDest[last] =  pcSrc[srcPtr];
        last++;
        srcPtr++;
    }

    pcDest[last] = '\0';
    return pcDest;
}

int Str_compare (const char str1[], const char str2[]) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ptr1 = 0, ptr2 = 0;
    while(str1[ptr1] != '\0' && str2[ptr2] != '\0') {
        int result = str1[ptr1] - str2[ptr2];
        if(result != 0){
            return result;
        }
        ptr1++;
        ptr2++;
    }

    if(str1[ptr1] == '\0' && str2[ptr2] == '\0') {
        return 0;
    }
    else if(str1[ptr1] == '\0') {
        return -1;
    }
    else {
        return 1;
    }
}

char *Str_search (const char *str1, const char *str2) {
    assert(str1 != NULL);
    assert(str2 != NULL);

    if(str2[0] == '\0') {
        return str1;
    }

    size_t length1 = Str_getLength(str1);
    size_t length2 = Str_getLength(str2);

    if(length1 < length2) { return NULL;}
    
    size_t target = length1 - length2;
    size_t start = 0;
    while(start <= target) {
        size_t curr = start;
        for(int i = 0; i <= length2; i++) {
            if(str2[i] == '\0') {
                return str1 + start;
            }
            else if(str1[curr] == str2[i]) {
                curr++;
            }
            else{
                break;
            }
        }
        start++;
    }

    return NULL;
}
