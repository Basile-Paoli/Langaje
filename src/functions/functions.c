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
 * ASSIGN MESSAGE TO ERROR
 *
 * */
int assignErrorMessage(error *err, char *msg){
    err->value = ERR_SYNTAX;
    err->message = malloc(strlen(msg) + 1);
    strcpy(err->message, msg);

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
var add(var *var1, var *var2, error *err){
    var result;
    result.type = _int;
    err->value = ERR_SUCCESS;
    char buffer[20];

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
            else if(var2->type == _char || var2->type == _int){
                result.type = var2->type == _char ? _char : _int;
                int resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (int, float, string, char)");
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
            else if(var2->type == _float || var2->type == _int || var2->type == _char){
                result.type = _float ;
                float resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (int, float, string, char)");
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
            }else if (var2->type == _float) {
                result.type = _float;
                float resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else if(var2->type == _int){
                result.type = _char;
                int resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (int, float, string, char)");
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
                else {
                    assignErrorMessage(err, "Please specify a valid type (int, float, string, char)");
                    break;
                }
                char *resultValue;
                concat(&result, var1->value._string, buffer, &resultValue);
                assign(&result, resultValue);
                free(resultValue);
            }
            break;
        default:
            assignErrorMessage(err, "Please specify a valid type (int, float, char, string)");
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
var substract(var *var1, var *var2, error *err){
    var result;
    result.type = _int;
    err->value = ERR_SUCCESS;

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
            else {
                assignErrorMessage(err, "Please specify a valid type (int, float, char)");
            }
            break;

        case _float:
            if(var2->type == _float || var2->type == _int || var2->type == _char) {
                result.type = _float;
                float resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (int, float, char)");
            }
            break;

        case _char:
            if(var2->type == _float){
                result.type = _float;
                float resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else if(var2->type == _int || var2->type == _char){
                result.type = _int;
                int resultValue = getNumericValue(var1) - getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (int, float, char)");
            }
            break;
        default:
            assignErrorMessage(err, "Please specify a valid type (int, float, char)");
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
var multiply(var *var1, var *var2, error *err){
    var result;
    result.type = _int;
    err->value = ERR_SUCCESS;

    switch(var1->type){
        case _int:
            if(var2->type == _float){
                result.type = _float;
                float resultValue = getNumericValue(var1) * getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else if(var2->type == _int) {
                result.type = _int;
                int resultValue = getNumericValue(var1) * getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (float or int)");
            }
            break;

        case _float:
            if(var2->type == _float || var2->type == _int){
                result.type = _float;
                float resultValue = getNumericValue(var1) * getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (float or int)");
            }
            break;

        default:
            assignErrorMessage(err, "Please specify a valid type (float or int)");
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
var divide(var *var1, var *var2, error *err){
    var result;
    result.type = _int;
    err->value = ERR_SUCCESS;

    switch(var1->type){
        case _int: case _float:
            if(getNumericValue(var2) == 0){
                assignErrorMessage(err,"Divider cannot be equal to 0");
            }
            else if(var2->type == _int || var2->type == _float) {
                result.type = _float;
                float resultValue = getNumericValue(var1) / getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (float or int)");
            }
            break;

        default:
            assignErrorMessage(err, "Please specify a valid type (float or int)");
            break;
    }

    return result;
}

/*
 *
 * MODULO FUNCTION
 *
 */
var modulo(var *var1, var *var2, error *err){
    var result;
    result.type = _int;
    err->value = ERR_SUCCESS;

    switch(var1->type){
        case _int:
            if(getNumericValue(var2) == 0){
                assignErrorMessage(err,"Divider cannot be equal to 0");
            }
            else if(var2->type == _int) {
                result.type = _int;
                int resultValue = var1->value._int % var2->value._int;
                assign(&result, &resultValue);
            }
            else {
                assignErrorMessage(err, "Parameters must be integers");
            }
            break;
        default:
            assignErrorMessage(err, "Parameters must be integers");
            break;
    }

    return result;
}

/*
 *
 * POWER FUNCTION
 *
 */

var power(var *var1, var *var2, error *err){
    var result;
    result.type = _int;
    err->value = ERR_SUCCESS;

    switch (var1->type){
        case _int: case _float:
            if(var2->type == _int || var2->type == _float){
               result.type = (var2->type == _float || var1->type == _float) ? _float : _int;
               if(var2->type == _float || var1->type == _float) {
                   float resultValue = pow(getNumericValue(var1), getNumericValue(var2));
                   assign(&result, &resultValue);
               } else {
                   int resultValue = pow(getNumericValue(var1), getNumericValue(var2));
                   assign(&result, &resultValue);
               }
            }
            else {
                assignErrorMessage(err, "Please specify a valid type (float or int)");
            }
            break;
        default:
            assignErrorMessage(err, "Please specify a valid type (float or int)");
            break;
    }

    return result;
}

/*
 *
 * AND FUNCTION
 *
 */
var logicalAnd(var *var1, var *var2, error *err){
    var result;
    result.type = _int;
    err->value = ERR_SUCCESS;

    if(var1->type == _int && var2->type == _int){
        int resultValue = var1->value._int & var2->value._int;
        assign(&result, &resultValue);
    } else {
        assignErrorMessage(err, "Invalid type: Both variables must be of type int.");
    }

    return result;
}

/*
 *
 * OR FUNCTION
 *
 */
var logicalOr(var *var1, var *var2, error *err){
    var result;
    result.type = _int;
    err->value = ERR_SUCCESS;

    if(var1->type == _int && var2->type == _int){
        int resultValue = var1->value._int | var2->value._int;
        assign(&result, &resultValue);
    } else {
        assignErrorMessage(err, "Invalid type: Both variables must be of type int.");
    }

    return result;
}


/*
 *
 * SQRT FUNCTION
 *
 */

var squareroot(var *var1, error *err){
    var result;
    result.type = _float;
    err->value = ERR_SUCCESS;

    if(var1->type == _int || var1->type == _float){
        float resultValue = sqrtf(getNumericValue(var1));
        assign(&result, &resultValue);
    } else {
        assignErrorMessage(err, "Please specify a valid type (float or int)");
    }

    return result;
}

/*
*
* EQUAL FUNCTION
*
*/
var isEqual(var* v, var* v2,int reversed, error* err){
    printf("tset1\n");
    var result;
    result.type = _int;
    switch(v->type){
        case _int:
        case _float:
        case _char:{
            if(v2->type == _string){
                result.value._int = 0;
                return result;
            }

            float value1 = getNumericValue(v);
            float value2 = getNumericValue(v2);
            if(reversed == 1){
                result.value._int = value1 != value2;
            } else {
                result.value._int = value1 == value2;
            }
            
            return result;
            break;     
        }
        case _string:{
            if(v2->type != _string){
                result.value._int = 0;
                return result;
            }
            
            break;
        }
    }
}

/*
*
* GREATER FUNCTION
*
*/
var isGreater(var* v, var* v2, int strict, error* err){
    var result;
    result.type = _int;
    if(v->type == _string || v2->type == _string){
        result.value._int = 0;
        return result;
    }

    float value1 = getNumericValue(v);
    float value2 = getNumericValue(v2);
    if(strict == 1){
        result.value._int = value1 > value2;
        return result;
    } else {
        result.value._int = value1 >= value2;
        return result;
    }
}

/*
*
* LESSER FUNCTION
*
*/
var isLesser(var* v, var* v2, int strict, error* err){
    var result;
    result.type = _int;
    if(v->type == _string || v2->type == _string){
        result.value._int = 0;
        return result;
    }

    float value1 = getNumericValue(v);
    float value2 = getNumericValue(v2);
    if(strict == 1){
        result.value._int = value1 < value2;
        return result;
    } else {
        result.value._int = value1 <= value2;
        return result;
    }
}