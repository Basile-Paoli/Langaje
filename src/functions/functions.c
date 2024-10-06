//
// Define base functions to add, sub, divide, ..
//

#include "functions.h"

/*
 *
 * GET NUMERIC VALUE
 *
 */
float getNumericValue(var* v) {
    switch (v->type) {
        case _int:    return (float)v->value._int;
        case _float:  return v->value._float;
        case _char:   return (float)v->value._char;
        default:      return 0;
    }
}

/*
 *
 * STRING CONCAT
 *
 */
int concat(var *result, char *var1, char *var2, char** resultValue){
    result->type = _string;
    result->value._string = NULL;

    *resultValue = malloc(strlen(var1) + strlen(var2) + 1);

    strcpy(*resultValue, var1);
    strcat(*resultValue, var2);

    return 0;
}

/*
 *
 * ADD FUNCTION
 * int + int = int / int + float = float / int + char = char / int + str = str
 * float = int = float / float + float = float / float + char = float / float + str = str
 * char + char = str / char + int = char / char + float = float / char + str = str
 * str + int/char/float = str
 *
 */
var add(var *var1, var *var2){
    var result;
    char buffer[20];

    // NOTE : gestion d'erreur pour les tableaux
    // Define the type of var to return
    switch (var1->type) {
        case _int:
            if(var2->type == _float) {
                result.type = _float;
                float resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else if(var2->type == _string) {
                sprintf(buffer, "%d", var1->value._int);
                char *resultValue;

                concat(&result, buffer, var2->value._string, &resultValue);
                assign(&result, resultValue);
                free(resultValue);
            }
            else {
                result.type = var2->type == _char ? _char : _int;
                int resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        case _float:
            if(var2->type == _string) {
                sprintf(buffer, "%f", var1->value._float);
                char *resultValue;

                concat(&result, buffer, var2->value._string, &resultValue);
                assign(&result, resultValue);
                free(resultValue);
            }
            else{
                result.type = _float ;
                float resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        case _char:
            // Faire verification que tjrs dans le tableau ascii
            if(var2->type == _string) {
                sprintf(buffer, "%c", var1->value._char);
                char *resultValue;

                concat(&result, buffer, var2->value._string, &resultValue);
                assign(&result, resultValue);
                free(resultValue);
            }
            else if(var2->type == _char) {
                result.type = _string;
                result.value._string = NULL;
                char *resultValue = malloc(3 * sizeof(char));
                sprintf(resultValue, "%c%c", var1->value._char, var2->value._char);

                assign(&result, resultValue);
                free(resultValue);
            }else {
                if (var2->type == _float) {
                    result.type = _float;
                    float resultValue = getNumericValue(var1) + getNumericValue(var2);
                    assign(&result, &resultValue);
                } else {
                    result.type = _char;
                    int resultValue = getNumericValue(var1) + getNumericValue(var2);
                    assign(&result, &resultValue);
                }
            }
            break;

        case _string:
            if(var2->type == _string) {
                char *resultValue;

                concat(&result, var1->value._string, var2->value._string, &resultValue);
                assign(&result, resultValue);
                free(resultValue);
            } else {
                if (var2->type == _int)
                    sprintf(buffer, "%d", var2->value._int);
                else if (var2->type == _float)
                    sprintf(buffer, "%f", var2->value._float);
                else if (var2->type == _char)
                    sprintf(buffer, "%c", var2->value._char);

                char *resultValue;
                concat(&result, var1->value._string, buffer, &resultValue);
                assign(&result, resultValue);
                free(resultValue);
            }
            break;
        default:
            // Modifier ce cas quand on aura la gestion d'erreur
            result.type = _int;
            int resultValue = 0;
            assign(&result, &resultValue);
            break;
    }

    return result;
}

/*
 *
 * SUBSTRACT FUNCTION
 * int - int = int / int - float = float / int - char = int
 * float - int = float / float - char = float / float - float = float
 * char - int = int / char - float = float / char - char = int
 *
 */
var subtract(var *var1, var *var2){
    var result;

    switch(var1->type){
        case _int:
            if(var2->type == _int){
                result.type = _int;
                int resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else if(var2->type == _float){
                result.type = _float;
                float resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else if(var2->type == _char){
                result.type = _int;
                int resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        case _float:
            if(var2->type == _float || var2->type == _int || var2->type == _char) {
                result.type = _float;
                float resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        case _char:
            if(var2->type == _float){
                result.type = _float;
                float resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            } else {
                result.type = _int;
                int resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;
        default:
            result.type = _int;
            int resultValue = 0;
            assign(&result, &resultValue);
            break;
    }

    return result;
}

/*
 *
 * MULTIPLY FUNCTION
 * int * int = int / int * float = float
 * float * float = float / float * int = float
 *
 */
var multipy(var *var1, var *var2){
    var result;

    switch(var1->type){
        case _int:
            if(var2->type == _float){
                result.type = _float;
                float resultValue = getNumericValue(var1) * getNumericValue(var2);
                assign(&result, &resultValue);
            } else {
                result.type = _int;
                int resultValue = getNumericValue(var1) * getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        case _float:
            if(var2->type == _float || var2->type == _int){
                result.type = _float;
                float resultValue = getNumericValue(var1) * getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        default:
            result.type = _int;
            int resultValue = 0;
            assign(&result, &resultValue);
            break;
    }

    return result;
}

/*
 *
 * DIVIDE FUNCTION
 * float
 *
 */
var divide(var *var1, var *var2){
    var result;

    switch(var1->type) {
        case _int: case _float:
            if(getNumericValue(var2) > 0) {
                result.type = _float;
                float resultValue = getNumericValue(var1) / getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        default:
            result.type = _int;
            int resultValue = 0;
            assign(&result, &resultValue);
            break;
    }

    return result;
}
