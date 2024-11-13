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
    _array
}varType;

typedef struct array{
    varType type;
    struct var* values;
    int capacity;
    int length;
}array;

typedef union varValue{
    int _int;
    float _float;
    char _char;
    char* _string;
    array* _array;
}varValue;


typedef struct var{
    varType type;
    varValue value;
}var;

typedef struct function{
    char* name;
    functionParameter* parameters;
    int parametersCount;
    int voidReturn;
    varType type;
    InstructionBlock* instructions;
    char isBuiltin;
}function;

int assignString(var* v, char* value);
int assign(var* v, void* value, error *err);
void display(var* v, error *err,int indentLevel);

void var2var(var* v, var* v2, error *err);

var* newArrayVar(int size, varType type);
void appendToArrayVar(var *tab, var val);
var* getVarPointerFromArray(var* array, int index, error *err);
void destroyVar(var* v);
void destroyFunction(function* f);
char* getVarTypeName(varType type);

#endif //LANGAJE_TYPES_H
