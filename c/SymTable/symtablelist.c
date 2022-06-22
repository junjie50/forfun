/*--------------------------------------------------------------------*/
/* symtable.c                                                         */
/* Author: JunJie                                                     */
/*--------------------------------------------------------------------*/

#include "symtable.h"
#include <string.h>
#include <assert.h>

struct TableNode {
    char *key;
    void *data;
    struct TableNode *next;
};


struct SymTable {
    struct TableNode *root;
    int length;
};

SymTable_T SymTable_new(void) {
    SymTable_T newTable = malloc(sizeof(struct SymTable));
    if(!newTable) {return NULL;}
    newTable->length = 0;
    newTable->root = NULL;
    return newTable;
}

void freeHelper(struct TableNode *curr) {
    if(curr == NULL) {
        return;
    }

    freeHelper(curr->next);
    free(curr->key);
    free(curr);
}

void SymTable_free(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    freeHelper(oSymTable->root);
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->length;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    assert(oSymTable != NULL && pcKey != NULL);

    if(SymTable_contains(oSymTable, pcKey)) {return 0;}
    
    size_t length = strlen(pcKey);
    char *strKey = malloc(length + 1);
    struct TableNode *insert = malloc(sizeof(struct TableNode));
    if(insert == NULL) {return 0;}
    if(strKey == NULL) {
        free(insert);
        return 0;
    }
    strcpy(strKey, pcKey);

    struct TableNode **curr = &(oSymTable->root);
    while((*curr) != NULL) {
        curr = &((*curr)->next);
    }

    insert->key = strKey;
    insert->data = pvValue;
    insert->next = NULL;
    oSymTable->length++;
    (*curr) = insert;
    return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue){
    assert(oSymTable != NULL && pcKey != NULL);
    struct TableNode *curr = oSymTable->root;
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
    struct TableNode *curr = oSymTable->root;
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
    struct TableNode *curr = oSymTable->root;
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

    struct TableNode **curr = &(oSymTable->root);
    while((*curr) != NULL) {
        if(strcmp((*curr)->key, pcKey) == 0) {
            free((*curr)->key);
            struct TableNode *deleting = (*curr);
            void *deleteval = deleting->data;
            (*curr) = (*curr)->next;
            free(deleting);
            oSymTable->length--;
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
    struct TableNode *curr = oSymTable->root;
    while(curr != NULL) {
        (*pfApply)(curr->key, curr->data, pvExtra);
        curr = curr->next;
    }
}