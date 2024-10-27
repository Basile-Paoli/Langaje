//
// Created by Mael on 02/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "types.h"

/**
 * @brief Assigns a string to the `value._string` field of a `var` structure.
 * @warning Free the value you passed after calling the function !
 * @param v Pointer to a `var` structure with `type` set to `_string`.
 * @param value String to assign.
 * @return 0 on success, 1 on failure.
 */
int assignString(var *v, char *value) {
    int len = strlen(value);
    if (len < 0) {
        return 1;
    }
    if (v->value._string == NULL || len != strlen(v->value._string)) {
        v->value._string = (char *) realloc(v->value._string, sizeof(char) * (len + 1));
    }
    if (v->value._string == NULL) {
        return 1;
    }

    if (strcpy(v->value._string, value) == NULL) {
        return 1;
    }
    return 0;
}

/**
 * @brief Assigns a value to the `value._type` field of a `var` structure.
 * @warning Always pass a declared variable, never a direct value.
 * @param v Pointer to a `var` structure.
 * @param value Value to assign.
 * @return 0 on success, 1 on failure.
 */
int assign(var *v, void *value) {
    switch (v->type) {
        case _int:
            v->value._int = *(int *) value;
            break;
        case _float:
            v->value._float = *(float *) value;
            break;
        case _char:
            v->value._char = *(char *) value;
            break;
        case _string:
            assignString(v, (char *) value);
            break;
        default:
            return 1;
            break;
    }
    return 0;
}

void var2var(var* v, var* v2){
    switch(v->type){
        case(_int):
            //assign(v,&v2->value._int);
            switch(v2->type){
                case _float:{
                    v->value._int = (int)v2->value._float;
                    break;
                }
                case _char:{
                    v->value._int = (int)v2->value._char;
                    break;
                }
                case _string:{
                    //RAISE ERROR
                    break;
                }default:{
                    v->value._int = v2->value._int;
                    break;
                }
            }
            break;
        case(_float):
            switch(v2->type){
                case _int:{
                    v->value._float = (float)v2->value._int;
                    break;
                }
                case _char:{
                    v->value._float = (float)v2->value._char;
                    break;
                }
                case _string:{
                    //RAISE ERROR
                    break;
                }
                default:{
                    v->value._float = v2->value._float;
                    break;
                }
            }
            break;

        case(_char):
            switch(v2->type){
                case _int:{
                    v->value._char = (char)v2->value._int;
                    break;
                }
                case _float:{
                    v->value._char = (char)v2->value._float;
                    break; 
                }
                case _string:{
                    // RAISE ERROR
                    break;
                }
                default:{
                    v->value._char = v2->value._char;
                    break;
                }
            }
            break;

        case(_string):
            switch(v2->type){
                case _float:
                case _int:
                case _char:
                    //RAISE ERROR
                    break;
                case _string:{
                    assignString(v,v2->value._string);
                }
            }
            break;
        case(_array):{
            switch(v2->type){
                case _array:
                    v->value._array = v2->value._array;
                    break;
                default:
                    //RAISE ERROR
                    break;
            }
            break;
        }
        
        default:
            break;
    }
}

//THIS FUNCTION DOES NOT WORK ACTUALLY. NEED TO BE FIXED!!!
void destroyVar(var* v){
    if(v->type == _array){
        for(int i = 0; i < v->value._array->length; i++){
            destroyVar(&(v->value._array->values[i].value));
        }
    } else {
        if(v != NULL){
            //free(v);
        }
            
    }
}

/**
 * @brief Display the value based on the type
 * @param v Pointer to a `var` structure.
 */


void display(var* v) {
    if (v == NULL) {
        printf("Null variable error\n");
        return;
    }

    switch (v->type) {
        case _int:
            printf("%d\n", v->value._int);
            break;
        case _float:
            printf("%f\n", v->value._float);
            break;
        case _char:
            printf("%c\n", v->value._char);
            break;
        case _string:
            printf("%s\n", v->value._string);
            break;
        case _array:
            for (int i = 0; i < v->value._array->length; i++) {
                if(v->value._array->values[i].type == _array){
                    printf("\nSubarray :%d\n",i);
                }
                display(&v->value._array->values[i]);
                
            }
            break;
        default:
            printf("Unknown type: %d\n", v->type);
            break;
    }
}


var* newArrayVar(int size, varType type) {
    var* res = malloc(sizeof(var));
    res->value._array = malloc(sizeof(array));

    res->value._array->capacity = size == 0 ? 1 : size;
    res->value._array->values = malloc(size * sizeof(var));
    res->value._array->length = size;
    res->value._array->type = type;
    for(int i = 0; i < size; i++){
        res->value._array->values[i].type = type;
        switch(type){
            case _int:{
                res->value._array->values[i].value._int = 0;
                break;
            }
            case _float:{
                res->value._array->values[i].value._float = 0.0;
                break;
            }
            case _string:{
                assignString(&res->value._array->values[i].value, "");
                break;
            }
            
        }
    }

    res->type = _array;
    return res;
}


var* getVarPointerFromArray(var* array, int index){
    if(index >= array->value._array->length){
        //RAISE ERROR
        printf("__OUT OF INDEX ERROR__\n");
        return NULL;
    }
    return &(array->value._array->values[index]);
}


void appendToArrayVar(var *tab, var val) {
    assert(tab->type == _array);
    array *arr = tab->value._array;
    if (arr->length < arr->capacity) {
        arr->values[arr->length++] = val;
    } else {
        arr->capacity *= 2;
        arr->values = realloc(arr->values, arr->capacity);
        arr->values[arr->length++] = val;
    }
}
