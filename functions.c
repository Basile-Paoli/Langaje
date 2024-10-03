//
// Define base functions to add, sub, divide, ..
//

#include "functions.h"

/*
 *
 * GET NUMERIC VALUE
 *
 * */
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
 * ADD FUNCTION
 * int + int = _int
 * int + float = _float
 * int + str = _str
 *
 */
var add(var *var1, var *var2){
    var result;
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
                result.type = _string;
                sprintf(buffer, "%d", var1->value._int);

                char *resultValue = malloc(strlen(buffer) + strlen(var2->value._string) + 1);
                strcpy(resultValue, buffer);
                strcat(resultValue, var2->value._string);

                // POUR L'INSTANT JE MET DES PRINTF LE TEMPS DE FIX LE ASSIGN
                printf("%s\n", resultValue);
                //assign(&result, &resultValue);
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
                result.type = _string;
                sprintf(buffer, "%f", var1->value._float);

                char *resultValue = malloc(strlen(buffer) + strlen(var2->value._string) + 1);
                strcpy(resultValue, buffer);
                strcat(resultValue, var2->value._string);

                // POUR L'INSTANT JE MET DES PRINTF LE TEMPS DE FIX LE ASSIGN
                printf("%s\n", resultValue);
                //assign(&result, &resultValue);
                free(resultValue);
            }
            else{
                result.type = _float ;
                float resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        case _char:
            if(var2->type == _string) {
                result.type = _string;
                sprintf(buffer, "%c", var1->value._char);

                char *resultValue = malloc(strlen(buffer) + strlen(var2->value._string) + 1);
                strcpy(resultValue, buffer);
                strcat(resultValue, var2->value._string);

                // POUR L'INSTANT JE MET DES PRINTF LE TEMPS DE FIX LE ASSIGN
                printf("%s\n", resultValue);
                //assign(&result, &resultValue);
                free(resultValue);
            }
            else if(var2->type == _char) {
                result.type = _string;
                char *resultValue = malloc(3 * sizeof(char));
                sprintf(resultValue, "%c%c", var1->value._char, var2->value._char);

                // POUR L'INSTANT JE MET DES PRINTF LE TEMPS DE FIX LE ASSIGN
                printf("%s\n", resultValue);
                //assign(&result, &resultValue);
                free(resultValue);
            }
            else if(var2->type == _float) {
                result.type = _float;
                float resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            else {
                result.type = _char;
                int resultValue = getNumericValue(var1) + getNumericValue(var2);
                assign(&result, &resultValue);
            }
            break;

        case _string:
            result.type = _string;
            if(var2->type == _int){
                result.type = _string;
                sprintf(buffer, "%d", var2->value._int);

                char *resultValue = malloc(strlen(buffer) + strlen(var1->value._string) + 1);
                strcpy(resultValue, var1->value._string);
                strcat(resultValue, buffer);

                // POUR L'INSTANT JE MET DES PRINTF LE TEMPS DE FIX LE ASSIGN
                printf("%s\n", resultValue);
                //assign(&result, &resultValue);
                free(resultValue);
            }
            else if(var2->type == _float){
                result.type = _string;
                sprintf(buffer, "%f", var2->value._float);

                char *resultValue = malloc(strlen(buffer) + strlen(var1->value._string) + 1);
                strcpy(resultValue, var1->value._string);
                strcat(resultValue, buffer);

                // POUR L'INSTANT JE MET DES PRINTF LE TEMPS DE FIX LE ASSIGN
                printf("%s\n", resultValue);
                //assign(&result, &resultValue);
                free(resultValue);
            }
            else if(var2->type == _char){
                sprintf(buffer, "%c", var2->value._char);

                char *resultValue = malloc(strlen(var1->value._string) + strlen(buffer) + 1);

                strcpy(resultValue, var1->value._string);
                strcat(resultValue, buffer);

                // POUR L'INSTANT JE MET DES PRINTF LE TEMPS DE FIX LE ASSIGN
                printf("%s\n", resultValue);
                //assign(&result, &resultValue);
                free(resultValue);
            }
            else {
                char *resultValue = malloc(strlen(var1->value._string) + strlen(var2->value._string) + 1);
                strcpy(resultValue, var1->value._string);
                strcat(resultValue, var2->value._string);

                printf("%s\n", resultValue);
                free(resultValue);
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

int main() {
    var a;
    var b;
    var c;


    a.type = _char;
    a.value._char = 'A';


    b.type = _string;
    b.value._string = malloc(strlen("test") + 1);
    b.value._string = "test";

    /*


    b.type = _float;
    b.value._float= 1.0;

    b.type = _int;
    b.value._int= 1;


    b.type = _char;
    b.value._char= 'A';

    b.type = _string;
    b.value._string = malloc(strlen("test") + 1);
    b.value._string = "test";
     */

    c = add(&a, &b);

    display(&c);
    return 0;
}