//
// Define base functions to add, sub, divide, ..
//

#include "functions.h"

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
                float resultValue = var1->value._int + var2->value._float;
                assign(&result, &resultValue);
            }
            else if(var2->type == _string) {
                result.type = _string;
                sprintf(buffer, "%d", var1->value._int);

                char *resultValue = malloc(strlen(buffer) * strlen(var2->value._string) + 1);
                strcpy(resultValue, buffer);
                strcat(resultValue, var2->value._string);
                assign(&result, &resultValue);
            }
            else if(var2->type == _char){
                result.type = _char;
                int resultValue = var1->value._int + var2->value._char;
                assign(&result, &resultValue);
            }
            else {
                result.type = _int;
                int resultValue = var1->value._int + var2->value._int;
                assign(&result, &resultValue);
            }
            break;
        case _float:
            if(var2->type == _string)
                result.type = _string;
            else if(var2->type == _char)
                result.type = _char;
            else
                result.type = _float;
            break;
        case _char:
            if(var2->type == _string)
                result.type = _string;
            else if(var2->type == _float)
                result.type = _float;
            else
                result.type = _char;
            break;
        case _string:
            result.type = _string;
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

    a.type = _int;
    a.value._int = 1;

    b.type = _float;
    b.value._float = 4.0;

    c = add(&a, &b);

    display(&c);
    return 0;
}