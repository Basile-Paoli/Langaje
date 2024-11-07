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
int assign(var *v, void *value, error *err) {
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
            err->value = ERR_TYPE;
            assignErrorMessage(err, "Variable must be of type int, float, char or string.");
            return 1;
    }
    return 0;
}

void var2var(var* v, var* v2, error *err){
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
                    err->value = ERR_TYPE;
                    assignErrorMessage(err, "Can't convert string into integer");
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
                    err->value = ERR_TYPE;
                    assignErrorMessage(err, "Can't convert string into float");
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
                    err->value = ERR_TYPE;
                    assignErrorMessage(err, "Can't convert string into char");
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
                /*
                case _float:
                case _int:
                case _char:
                    err->value = ERR_TYPE;
                    assignErrorMessage(err, "Expected string, other given");
                    break;
                    */
                case _string:{
                    assignString(v,v2->value._string);
                }
                default:{
                    err->value = ERR_TYPE;
                    assignErrorMessage(err, "Expected string, other given");
                    break;
                }
            }
            break;
        case(_array):{
            switch(v2->type){
                case _array:
                    v->value._array = v2->value._array;
                    break;
                default:
                    err->value = ERR_TYPE;
                    assignErrorMessage(err, "Expected array, other given");
                    break;
            }
            break;
        }
        
        default:
            err->value = ERR_TYPE;
            assignErrorMessage(err, "Variable must be of type int, float, char, string or array.");
            break;
    }
}

/* 
* Function that destroy variables 
*/
void destroyVar(var* v){
    if(v->type == _array){
        destroyVar(v->value._array->values);
    } else {
        if(v != NULL){
            free(v);
        }
            
    }
}

/**
 * @brief Display the value based on the type
 * @param v Pointer to a `var` structure.
 */


void display(var* v, error *err) {
    if (v == NULL) {
        err->value = ERR_NOT_FOUND;
        assignErrorMessage(err, "Null variable error");
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
                display(&v->value._array->values[i], err);
                
            }
            break;
        default:
            err->value = ERR_TYPE;
            assignErrorMessage(err, "Unknown type, expected float, int, char, string or array");
            break;
    }
}

/**
* Creates an array with default values at the size of the parameter and the type passed.
* returns a pointer to this array.
 */
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
                assignString(&res->value._array->values[i], "");
                break;
            }
            
        }
    }

    res->type = _array;
    return res;
}

/**
* Function that returns the pointer of the array[index]
*/
var* getVarPointerFromArray(var* array, int index, error *err){
    if(index >= array->value._array->length){
        err->value = ERR_OUT_OF_BOUNDS;

        char *msg = malloc(strlen("Array access error: Index %d is out of range for the array of size %d.") + 1);
        sprintf(msg, "Index is out of range for the array of size %d.", array->value._array->length);

        assignErrorMessage(err, msg);
        free(msg);

        return NULL;
    }
    return &(array->value._array->values[index]);
}

//NOT USED OR WORKING I THINK CURRENTLY
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

/*
 * Return the name of the varType
 * Used for error msg
 */
char* getVarTypeName(varType type){
    switch (type) {
        case _int:
            return "integer";
        case _float:
            return "float";
        case _char:
            return "char";
        case _string:
            return "string";
        case _array:
            return "array";
        default:
            return "error";
    }
}