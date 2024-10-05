#include "hashtables.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

hm* hm_create(){
    hm* hashtable = malloc(sizeof(hm)); //Allocate hashtable
    if(hashtable == NULL) return NULL; //If allocation failed, return null



    hashtable->length = 0;
    hashtable->capacity = BASE_CAPACITY;

    hashtable->entries = calloc(hashtable->capacity, sizeof(hm_entry));
    if(hashtable->entries == NULL){
        free(hashtable);
        return NULL;
    }

    return hashtable;
}

void hm_free(hm* hashtable){
    for(int i = 0; i < hashtable->capacity; i++){
        free((void*)hashtable->entries[i].key);
    }

    free(hashtable->entries);
    free(hashtable);
}

// Source : https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char* key){
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}

void* hm_get(hm* hashtable, const char* key){
    uint64_t hash = hash_key(key);
    int index = (int)(hash & (uint64_t)(hashtable->capacity - 1));

    while(hashtable->entries[index].key != NULL){
        if(!strcmp(key,hashtable->entries[index].key)){
            return hashtable->entries[index].value;
        }

        index++;
        if(index >= hashtable->capacity) index = 0;
    }

    return NULL;
}

const char* hm_set(hm* hashtable, const char* key, void* value){
    if(value == NULL) return NULL;

    if(hashtable->length >= hashtable->capacity / 2) {
        if(hm_expand(hashtable) == 0){
            return NULL;
        }
    }

    return hm_set_entry(hashtable->entries, hashtable->capacity, key, value, &hashtable->length);
}

static const char* hm_set_entry(hm_entry* entries, int capacity, const char* key, void* value, int* plength){
    uint64_t hash = hash_key(key);
    int index = (int)(hash & (uint64_t)(capacity - 1));

    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            entries[index].value = value;
            return entries[index].key;
        }
        index++;
        if (index >= capacity) {
            index = 0;
        }
    }

    if (plength != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return NULL;
        }
        (*plength)++;
    }
    entries[index].key = (char*)key;
    entries[index].value = value;
    return key;
}


//0 = false
//1 = true
static int hm_expand(hm* hashtable){
    int newCap = hashtable->capacity * 2;

    if(newCap < hashtable->capacity) return 0;   

    hm_entry* newEntries = calloc(newCap,sizeof(hm_entry));
    if(newEntries == NULL) return 0;

    for(int i = 0; i < hashtable->capacity; i++){
        hm_entry entry = hashtable->entries[i];
        if(entry.key != NULL){
            hm_set_entry(newEntries,newCap,entry.key,entry.value,NULL);
        }
    }

    free(hashtable->entries);
    hashtable->entries = newEntries;
    hashtable->capacity = newCap;
    return 1;
}

int hm_len(hm* hashtable){
    return hashtable->length;
}

hmi hm_iterator(hm* hashtable) {
    hmi it;
    it._hashtable = hashtable;
    it._index = 0;
    return it;
}

int hm_next(hmi* it) {
    hm* hashtable = it->_hashtable;
    while (it->_index < hashtable->capacity) {
        int i = it->_index;
        it->_index++;
        if (hashtable->entries[i].key != NULL) {
            hm_entry entry = hashtable->entries[i];
            it->key = entry.key;
            it->value = entry.value;
            return 1;
        }
    }
    return 0;
}