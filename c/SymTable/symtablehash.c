/*--------------------------------------------------------------------*/
/* symtable.c                                                         */
/* Author: JunJie                                                     */
/*--------------------------------------------------------------------*/


#include "symtable.h"

#define INIT_SIZE 509
#define MAX_SIZE 65521
#define SIZE_ARRAY {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521}

struct TableData {
    char *key;
    void *data;
    struct TableData *next;
};

struct SymTable {
    size_t capacity;
    struct TableData **table;
    size_t size;
};

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
    const size_t HASH_MULTIPLIER = 65599;
    size_t u;
    size_t uHash = 0;

    assert(pcKey != NULL);

    for (u = 0; pcKey[u] != '\0'; u++)
    uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

    return uHash % uBucketCount;
}
      


SymTable_T SymTable_new(void) {
    SymTable_T newTable = malloc(sizeof(struct SymTable));
    if(!newTable) {return 0;}
    newTable->size = 0;
    newTable->capacity = INIT_SIZE;
    newTable->table = malloc(INIT_SIZE * sizeof(struct TableData*));
    if(!newTable->table) {free(newTable);return 0;}

    int i;
    for(i = 0; i < INIT_SIZE; i++) {
        newTable->table[i] = NULL;
    }

    return newTable;
}

void freeHelper(struct TableData *curr) {
    if(curr == NULL) {
        return;
    }

    freeHelper(curr->next);
    free(curr->key);
    free(curr);

}

void SymTable_free(SymTable_T oSymTable) {
    assert(oSymTable != NULL);

    int i;
    for(i = 0; i < oSymTable->capacity; i++) {
        struct TableData *curr = oSymTable->table[i];
        if(curr != NULL) {
            freeHelper(curr);
        }
    }

    free(oSymTable->table);
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->size;
}

size_t nextSize (size_t currSize) {
    const int data[] = SIZE_ARRAY;
    int curr = 0;
    while(data[curr] <= currSize) {
        curr++;
    }
    return data[curr];
}

void reSizeHelper (SymTable_T oSymTable) {
    size_t nextCap = nextSize(oSymTable->capacity);

    struct TableData **newTable = malloc(nextCap * sizeof(struct TableData*));
    if(!newTable) {return;}

    int i;
    for(i = 0; i < nextCap; i++) {
        newTable[i] = NULL;
    }

    struct TableData **oldTable = oSymTable->table;
    oSymTable->table = newTable;
    oSymTable->capacity = nextCap;
    size_t oldSize = oSymTable->size;
    oSymTable->size = 0;

    for(i = 0; i < oldSize; i++) {
        struct TableData *curr = oldTable[i];
        while(curr != NULL) {
            SymTable_put(oSymTable, curr->key, curr->data);
            curr = curr->next;
        }

        freeHelper(oldTable[i]);
    }

    free(oldTable);
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    assert(oSymTable != NULL && pcKey != NULL);
    if(SymTable_contains(oSymTable, pcKey)) {
        return 0;
    }

    size_t hashcode = SymTable_hash(pcKey, oSymTable->capacity);

    struct TableData *insert = malloc(sizeof(struct TableData));
    if(insert == NULL) {return 0;}
    
    // Defensive copy of key.
    size_t lengthKey = strlen(pcKey);
    char *keyStr = malloc(lengthKey + 1);
    if(keyStr == NULL) {
        free(insert);
        return 0;
    }
    strcpy(keyStr, pcKey);


    insert->data = pvValue;
    insert->key = keyStr;
    insert->next = oSymTable->table[hashcode];
    oSymTable->table[hashcode] = insert;
    oSymTable->size++;
    if(oSymTable->capacity != MAX_SIZE && oSymTable->size > oSymTable->capacity) {
        reSizeHelper(oSymTable);
    }
    return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue) {
    assert(oSymTable != NULL && pcKey != NULL);

    size_t hashcode = SymTable_hash(pcKey, oSymTable->capacity);

    struct TableData *curr = oSymTable->table[hashcode];
    while(curr != NULL) {
        if(strcmp(curr->key, pcKey) == 0) {
            void *oldval = curr->data;
            curr->data = pvValue;
            return oldval;
        }
        curr = curr->next;
    }
    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL && pcKey != NULL);

    size_t hashcode = SymTable_hash(pcKey, oSymTable->capacity);

    struct TableData *curr = oSymTable->table[hashcode];
    while(curr != NULL) {
        if(strcmp(curr->key, pcKey) == 0) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL && pcKey != NULL);

    size_t hashcode = SymTable_hash(pcKey, oSymTable->capacity);

    struct TableData *curr = oSymTable->table[hashcode];
    while(curr != NULL) {
        if(strcmp(curr->key, pcKey) == 0) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;

}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    assert(oSymTable != NULL && pcKey != NULL);

    size_t hashcode = SymTable_hash(pcKey, oSymTable->capacity);

    struct TableData **curr = &(oSymTable->table[hashcode]);
    
    while((*curr) != NULL) {
        if(strcmp((*curr)->key, pcKey) == 0) {
            void *deleteval = (*curr)->data;
            struct TableData *deleteitem = (*curr);
            free(deleteitem->key);
            (*curr) = (*curr)->next;
            free(deleteitem);
            oSymTable->size--;
            return deleteval;
        }
        curr = &((*curr)->next);
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra) {
    assert(oSymTable != NULL && pfApply != NULL);

    int i;
    for(i = 0; i < oSymTable->capacity; i++) {
        struct TableData *curr = oSymTable->table[i];
        while(curr != NULL) {
            (*pfApply)(curr->key, curr->data, pvExtra);
            curr = curr->next;
        }
    }
}