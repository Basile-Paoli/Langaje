//
// Created by Mael on 02/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../ast/ast.h"
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
    if(v->value._TMPstring != NULL ){
        free(v->value._TMPstring->chars);
        free(v->value._TMPstring);
        v->value._TMPstring = NULL;
    }
    if (v->value._TMPstring == NULL || len <= v->value._TMPstring->length) {
        declareString(v,len + 3);
    }
    if (v->value._TMPstring == NULL) {
        return 1;
    }

    v->value._TMPstring->length = len + 1;

    for(int i = 0; i < len; i++){
        v->value._TMPstring->chars[i].value._char = value[i];
    }
    v->value._TMPstring->chars[len].value._char = '\0';

    return 0;
}

int declareString(var* v, int size){
    string* str = malloc(sizeof(string));
    str->length = size;
    str->chars = malloc(sizeof(var) * size);
    for(int i = 0; i < size; i++){
        str->chars[i].type = _char;
        str->chars[i].value._char = '\0';
    }

    v->type = _TMPString;
    v->value._TMPstring = str;
    return 0;
}

char* getString(var* v, error* err){
    char* str = malloc(sizeof(char) * v->value._TMPstring->length);
    for(int i = 0; i < v->value._TMPstring->length; i++){
        str[i] = v->value._TMPstring->chars[i].value._char;
    }

    return str;
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

    if(err->value == ERR_SUCCESS) {
        switch (v->type) {
            case (_int):
                //assign(v,&v2->value._int);
                switch (v2->type) {
                    case _float: {
                        v->value._int = (int) v2->value._float;
                        break;
                    }
                    case _char: {
                        v->value._int = (int) v2->value._char;
                        break;
                    }
                    case _string: {
                        err->value = ERR_TYPE;
                        assignErrorMessage(err, "Can't convert string into integer");
                        break;
                    }
                    default: {
                        v->value._int = v2->value._int;
                        break;
                    }
                }
                break;
            case (_float):
                switch (v2->type) {
                    case _int: {
                        v->value._float = (float) v2->value._int;
                        break;
                    }
                    case _char: {
                        v->value._float = (float) v2->value._char;
                        break;
                    }
                    case _string: {
                        err->value = ERR_TYPE;
                        assignErrorMessage(err, "Can't convert string into float");
                        break;
                    }
                    default: {
                        v->value._float = v2->value._float;
                        break;
                    }
                }
                break;

            case (_char):
                switch (v2->type) {
                    case _int: {
                        v->value._char = (char) v2->value._int;
                        break;
                    }
                    case _float: {
                        v->value._char = (char) v2->value._float;
                        break;
                    }
                    case _string: {
                        v->value._char = (char) v2->value._string[0];
                        break;
                    }
                    default: {
                        v->value._char = v2->value._char;
                        break;
                    }
                }
                break;

            case (_string):
                switch (v2->type) {
                    /*
                    case _float:
                    case _int:
                    case _char:
                        err->value = ERR_TYPE;
                        assignErrorMessage(err, "Expected string, other given");
                        break;
                        */
                    case _string: {

                        assignString(v, v2->value._string);
                        break;
                    }
                    case _TMPString: {
                        char* str = getString(v2,err);
                        assignString(v,str);
                        free(str);
                        break;
                    }
                    default: {
                        err->value = ERR_TYPE;
                        assignErrorMessage(err, "Expected string, other given");
                        break;
                    }
                }
                break;
            case (_array): {
                switch (v2->type) {
                    case _array:{
                        var* vvv = copyArray(v2,err);
                        v->type = vvv->type;
                        v->value = vvv->value;
                        break;  
                    }
                    default:
                        err->value = ERR_TYPE;
                        assignErrorMessage(err, "Expected array, other given");
                        break;
                }
                break;
            }
            case (_TMPString): {
                switch(v2->type) {
                    case _TMPString:{
                        char* str = getString(v2,err);
                        assignString(v,str);
                        free(str);
                        break;
                    }
                    case _string: {
                        assignString(v, v2->value._string);
                        break;
                    }
                    default:
                        err->value = ERR_TYPE;
                        assignErrorMessage(err, "Expected string, other given");
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
}

var* copyArray(var* originalArray, error* err){
    var* tmp = newArrayVar(originalArray->value._array->capacity, originalArray->value._array->type);
    
    if(originalArray->value._array->type == _array){
        for(int i = 0; i < originalArray->value._array->capacity; i++){
            tmp->value._array->values[i] = *copyArray(&originalArray->value._array->values[i], err);
        }
    } else {
        for(int i = 0; i < originalArray->value._array->capacity; i++){
            tmp->value._array->values[i] = originalArray->value._array->values[i];
        }
    }

    return tmp;
}

/* 
* Function that destroy variables 
*/
void destroyVar(var* v){
    if(v->type == _array && v->value._array->type == _array){
        destroyVar(v->value._array->values);
    } else {
        if(v != NULL){
            free(v);
        }
            
    }
}

void destroyFunction(function* f){
    if(f != NULL){
        free(f->name);
        free(f);
    }
}

/**
 * @brief Display the value based on the type
 * @param v Pointer to a `var` structure.
 */


void display(var* v, error *err, int indentLevel) {
    if (v == NULL) {
        err->value = ERR_NOT_FOUND;
        assignErrorMessage(err, "Null variable error");
        return;
    }
    for(int i = 0; i < indentLevel; i++){
        printf("\t");
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
            printf("\"%s\"\n", v->value._string);
            break;
        case _TMPString:
            char* str = getString(v,err);
            printf("\"%s\"\n", str);
            free(str);
            break;        
        case _array:
            for (int i = 0; i < v->value._array->length; i++) {
                if(v->value._array->values[i].type == _array){
                    for(int tab = 0; tab < indentLevel+1; tab++){
                        printf("\t");
                    }
                    printf("Subarray %s\n",getVarTypeName(v->value._array->values[i].value._array->type));
                }
                display(&v->value._array->values[i], err, indentLevel);
                
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
                res->value._array->values[i].value._string = malloc(sizeof(char));
                assignString(&res->value._array->values[i], "");

                break;
            }
            
        }
    }
    res->type = _array;
    return res;
}

function* newFunctionPrototype(char* name, varType type, __builtinFunction__ __builtinId__, int parametersCount, error* err, fakeFunctionParam* parameters){
    function* f = malloc(sizeof(function));
    if (f == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return NULL;
    }
    f->isBuiltin = 1;
    f->instructions = NULL;
    f->type = type;
    f->parametersCount = parametersCount;
    functionParameter* p = malloc(sizeof(functionParameter) * parametersCount);
    if (p == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return NULL;
    }

    for(int i = 0; i < parametersCount; i++){
        p[i].name = malloc(sizeof(char) * (strlen(parameters[i].name) + 1));
        if (p[i].name == NULL) {
            err->value = ERR_MEMORY;
            assignErrorMessage(err, "Memory allocation error\n");
            return NULL;
        }

        strcpy(p[i].name,parameters[i].name);
        p[i].type.type = parameters[i].type;
    }
    f->parameters = p;
    f->name = malloc(sizeof(char) * (strlen(name) + 1));
    if (f->name == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return NULL;
    }

    f->__builtinIdentifier__ = __builtinId__;
    strcpy(f->name,name);
    return f;
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

var* getCharValueFromString(var* string, int index, error* err){
    return &(string->value._TMPstring->chars[index]);
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
        case _TMPString:
            return "TMPString";
        default:
            return "void";
    }
}

