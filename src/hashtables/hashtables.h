//
// Created by Mael GAZIO on 05/10/2024.
//

#ifndef LANGAJE_HASHTABLES_H
#define LANGAJE_HASHTABLES_H

typedef struct hm hm;

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

#endif //LANGAJE_HASHTABLES_H
