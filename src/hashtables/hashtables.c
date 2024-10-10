#include "hashtables.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
* Function that allocate an empty hashmap, with the size of #BASE_CAPACITY 
* Returns the hashmap pointer
*/
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

/**
* Function that frees all entries in the hashtable (ONLY THE ENTRIES, IF THE ENTRY ITSELF IS AN ALLOCATED OBJECT, FREE IT BEFORE)
* Then free the hashtable
*/

void hm_free(hm* hashtable){
    for(int i = 0; i < hashtable->capacity; i++){
        free((void*)hashtable->entries[i].key);
    }

    free(hashtable->entries);
    free(hashtable);
}


/**
* Function that hases the value and returns it on a 64bit unsigned integer
* See source to see how it works 
* Source : https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
*/
static uint64_t hash_key(const char* key){
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}

/**
* Function that hashes the key passed in argument, and takes the modulo of the index by the capacity 
* (Binary module because our size is pow(2), it's faster)
* We're using linear probing so the index goes ++ until we reach an empty entry meaning the ket doesn't exist
*/
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

/**
* Function that call the expand function if the length reach half the capacity
* Then, when the table is expanded, set the new entry value with the key by calling hm_set_entry
*
*/
const char* hm_set(hm* hashtable, const char* key, void* value){
    if(value == NULL) return NULL;

    if(hashtable->length >= hashtable->capacity / 2) {
        if(hm_expand(hashtable) == 0){
            return NULL;
        }
    }

    return hm_set_entry(hashtable->entries, hashtable->capacity, key, value, &hashtable->length);
}


/**
* Function that does linear probing to insert the value, until a free space is found at hash % capacity (++)
* If the plength is null : example : to expand the hashmap, we dont INCREASE the number of elements each time we copy, so we dont increment the pointer.
* else, we increment it because we are adding a value 
* if key already exist, we only replace the value.
* returns the key
*/
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


/**
* Function that double the size of the hashmap when called. 
* Copies all the values into the new hashmap and free the old one
* Returns 1 on success, 0 on failure.
*/
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

/**
* Returns length of the hashtable
*/
int hm_len(hm* hashtable){
    return hashtable->length;
}

/**
* Initialize an iterator for a given hashtable.
*/
hmi hm_iterator(hm* hashtable) {
    hmi it;
    it._hashtable = hashtable;
    it._index = 0;
    return it;
}

/**
* Function that make the iterator point to the next element of the hashtable (NOT NULL ELEMENTS ONLY)
* Returns 1 if element is found, 0 if there's nothing left.
*/
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