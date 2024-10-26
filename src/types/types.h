//
// Created by Mael on 02/10/2024.
//

#ifndef LANGAJE_TYPES_H
#define LANGAJE_TYPES_H


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

int assignString(var* v, char* value);
int assign(var* v, void* value);
void display(var* v);

void var2var(var* v, var* v2);

var* newArrayVar(int size, varType type);
void appendToArrayVar(var *tab, var val);

#endif //LANGAJE_TYPES_H
