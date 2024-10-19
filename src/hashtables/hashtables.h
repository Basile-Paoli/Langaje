//
// Created by Mael GAZIO on 05/10/2024.
//

#ifndef LANGAJE_HASHTABLES_H
#define LANGAJE_HASHTABLES_H
#define BASE_CAPACITY 32 //Base capacity for our hashmaps MUST BE POWER OF TWO !!
#define FNV_OFFSET 14695981039346656037UL // For hashing 
#define FNV_PRIME 1099511628211UL // For hashing

typedef struct{
    const char* key;
    void* value;
}hm_entry;

typedef struct{
    hm_entry* entries;
    int capacity;
    int length;
}hm;

typedef struct{
    hm** stack;
    int capacity;
    int length;
}hmStack;


hm* hm_create();

void hm_free(hm* hashtable);

void* hm_get(hm* hashtable, const char* key);

const char* hm_set(hm* hashtable, const char* key, void* value);

int hm_len(hm* hashtable);

typedef struct{
    const char* key;
    void* value;

    hm* _hashtable;
    int _index;
} hmi;


hmi hm_iterator(hm* hashtable);

int hm_next(hmi* iterator);

static const char* hm_set_entry(hm_entry* entries, int capacity, const char* key, void* value, int* plength);

static int hm_expand(hm* hashtable);



hmStack* hmStackCreate(int baseSize);
int hmStackPush(hmStack* stack, hm* map);
int hmStackPop(hmStack* stack);
int hmStackExpand(hmStack* stack);
int hmStackDestroy(hmStack* stack);
int isInStack(hmStack* stack, char* key);


#endif //LANGAJE_HASHTABLES_H
