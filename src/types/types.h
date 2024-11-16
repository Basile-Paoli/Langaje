//
// Created by Mael on 02/10/2024.
//

#ifndef LANGAJE_TYPES_H
#define LANGAJE_TYPES_H

#include "../errors/errors.h"

//FORWARD DECLARATION : 
typedef struct functionParameter functionParameter;
typedef struct initType initType;
typedef struct InstructionBlock InstructionBlock;

typedef enum varType {
    _int,
    _float,
    _char,
    _string,
    _array,
    _void,
    _TMPString
}varType;

typedef enum __builtinFunction__ {
    __print__,
    __len__,
    __randint__,
    __randfloat__,
    __system__,
    __input__,
    __randchoice__,
    __fread__,
    __fwrite__,
    __split__,
    __range__,
    __append__,
    __pop__,
    __type__
}__builtinFunction__;

typedef struct array{
    varType type;
    struct var* values;
    int capacity;
    int length;
}array;

typedef struct string{
    struct var* chars;
    int length;
}string;

typedef union varValue{
    int _int;
    float _float;
    char _char;
    char* _string;
    string* _TMPstring;
    array* _array;
}varValue;


typedef struct var{
    varType type;
    varValue value;
}var;

typedef struct fakeFunctionParam{
    char* name;
    varType type;
}fakeFunctionParam;


typedef struct function{
    char* name;
    functionParameter* parameters;
    int parametersCount;
    int voidReturn;
    varType type;
    InstructionBlock* instructions;
    char isBuiltin;
    __builtinFunction__ __builtinIdentifier__;
}function;


int assignString(var* v, char* value);
int declareString(var* v, int size);
int assign(var* v, void* value, error *err);
void display(var* v, error *err,int indentLevel);

void var2var(var* v, var* v2, error *err);

var* newArrayVar(int size, varType type,error* err);
void appendToArrayVar(var *tab, var val);
var* getVarPointerFromArray(var* array, int index, error *err);
void destroyVar(var* v);
void destroyFunction(function* f);
char* getVarTypeName(varType type);
function* newFunctionPrototype(char* name, varType type, __builtinFunction__ __builtinId__, int parametersCount, error* err, fakeFunctionParam* parameters);
var* copyArray(var* originalArray, error* err);
char* getString(var* v, error* err);

var* getCharValueFromString(var* string, int index, error* err);

#endif //LANGAJE_TYPES_H
