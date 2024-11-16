#include "__builtins__.h"

void __builtinToMap__(hm* functionMap, error* err){
    function* functions[] = {
        newFunctionPrototype("print",        _void,     __print__,      1, err, (fakeFunctionParam[]){{"message", _string}}),
        newFunctionPrototype("strlen",       _int,      __strlen__,     1, err, (fakeFunctionParam[]){{"string", _string}}),
        newFunctionPrototype("arrlen",       _int,      __arrlen__,     1, err, (fakeFunctionParam[]){{"array", _array}}),
        newFunctionPrototype("randint",      _int,      __randint__,    2, err, (fakeFunctionParam[]){{"min", _int}, {"max", _int}}),
        newFunctionPrototype("randfloat",    _float,    __randfloat__,  2, err, (fakeFunctionParam[]){{"min", _float}, {"max", _float}}),
        newFunctionPrototype("system",       _int,      __system__,     1, err, (fakeFunctionParam[]){{"command", _string}}),
        newFunctionPrototype("input",        _string,   __input__,      1, err, (fakeFunctionParam[]){{"message", _string}}),
        newFunctionPrototype("randchoice",   _int,      __randchoice__, 1, err, (fakeFunctionParam[]){{"array", _array}}),
        newFunctionPrototype("fread",        _string,   __fread__,      1, err, (fakeFunctionParam[]){{"filename", _string}}),
        newFunctionPrototype("fwrite",       _int,      __fwrite__,     3, err, (fakeFunctionParam[]){{"filename", _string}, {"content", _string}, {"method", _string}}),
        newFunctionPrototype("split",        _array,    __split__,      2, err, (fakeFunctionParam[]){{"string", _string}, {"delimiter", _string}}),
        newFunctionPrototype("range",        _array,    __range__,      3, err, (fakeFunctionParam[]){{"start", _int}, {"end", _int}, {"step", _int}}),
        newFunctionPrototype("append",      _array,     __append__,     2, err, (fakeFunctionParam[]){{"array", _array}, {"toAdd", _void}}),
        newFunctionPrototype("pop",        _array,     __pop__,        1, err, (fakeFunctionParam[]){{"array", _array}})

    };


    for (int i = 0; i < sizeof(functions) / sizeof(function*); i++) {
        hm_set(functionMap, functions[i]->name, functions[i]);
    }

    if (err->value != ERR_SUCCESS) {
        return;
    }
    
}

void call__print__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* message = (var*)hm_get(fStack->stack[0], "message");
    
    if (message->type != _TMPString) {
        err->value = ERR_TYPE;
        char *str = strdup("print function expect 1 parameter: message(string). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(message->type)) + 1);
        strcat(str, getVarTypeName(message->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }
    message->value._string = getString(message,err);

    // replace '\n' string by '\n' character
    for (int i = 0; i < strlen(message->value._string); i++) {
        if (message->value._string[i] == '\\' && message->value._string[i + 1] == 'n') {
            message->value._string[i] = '\n';
            for (int j = i + 1; j < strlen(message->value._string); j++) {
                message->value._string[j] = message->value._string[j + 1];
            }
        }
        if (message->value._string[i] == '\\' && message->value._string[i + 1] == 't') {
            message->value._string[i] = '\t';
            for (int j = i + 1; j < strlen(message->value._string); j++) {
                message->value._string[j] = message->value._string[j + 1];
            }
        }
    }

    printf("%s\n", message->value._string);

    newVar->value._int = 0;
}

void call__strlen__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* string = (var*)hm_get(fStack->stack[0], "string");
    
    if (string->type != _TMPString) {
        err->value = ERR_TYPE;
        char *str = strdup("strlen function expect 1 parameter: string(string). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(string->type)) + 1);
        strcat(str, getVarTypeName(string->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }
    char* str = getString(string,err);
    newVar->value._int = strlen(str);
    free(str);
}

void call__arrlen__(hmStack* fStack, error* err) {


    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* array = (var*)hm_get(fStack->stack[0], "array");
    
    if (array->type != _array) {
        err->value = ERR_TYPE;
        char *str = strdup("arrlen function expect 1 parameter: array(array). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(array->type)) + 1);
        strcat(str, getVarTypeName(array->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    newVar->value._int = array->value._array->length;
}

void call__randint__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* min = (var*)hm_get(fStack->stack[0], "min");
    var* max = (var*)hm_get(fStack->stack[0], "max");
    
    if (min->type != _int || max->type != _int) {
        err->value = ERR_TYPE;
        char *str = strdup("randint function expect 2 parameters: min(int), max(int). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(min->type)) + strlen(getVarTypeName(max->type)) + 1);
        strcat(str, getVarTypeName(min->type));
        strcat(str, ", ");
        strcat(str, getVarTypeName(max->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    newVar->value._int = rand() % (max->value._int - min->value._int + 1) + min->value._int;
}

void call__randfloat__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* min = (var*)hm_get(fStack->stack[0], "min");
    var* max = (var*)hm_get(fStack->stack[0], "max");
    
    if (min->type != _float || max->type != _float) {
        err->value = ERR_TYPE;
        char *str = strdup("randfloat function expect 2 parameters: min(float), max(float). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(min->type)) + strlen(getVarTypeName(max->type)) + 1);
        strcat(str, getVarTypeName(min->type));
        strcat(str, ", ");
        strcat(str, getVarTypeName(max->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    newVar->type = _float;

    newVar->value._float = (float)rand() / (float)RAND_MAX * (max->value._float - min->value._float) + min->value._float;
}

void call__system__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* command = (var*)hm_get(fStack->stack[0], "command");
    
    if (command->type != _string) {
        err->value = ERR_TYPE;
        char *str = strdup("system function expect 1 parameter: command(string). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(command->type) + 1));
        strcat(str, getVarTypeName(command->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    newVar->value._int = system(command->value._string);
}

void call__input__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* message = (var*)hm_get(fStack->stack[0], "message");
    if (message->type != _TMPString) {
        err->value = ERR_TYPE;
        char *str = strdup("input function expect 1 parameter: message(string). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(message->type) + 1));
        strcat(str, getVarTypeName(message->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }
    char* str = getString(message,err);


    char* input = (char*)malloc(255);
    printf("%s", str);
    free(str);
    fgets(input, 255, stdin);
    input[strlen(input) - 1] = '\0';

    newVar->type = _string;
    newVar->value._string = (char*)malloc(sizeof(char));
    if (assignString(newVar, input) == 1) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    free(input);
}

void call__randchoice__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* array = (var*)hm_get(fStack->stack[0], "array");
    
    if (array->type != _array) {
        err->value = ERR_TYPE;
        char *str = strdup("randchoice function expect 1 parameter: array(array). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(array->type) + 1));
        strcat(str, getVarTypeName(array->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    newVar->value._int = array->value._array->values[rand() % array->value._array->length].value._int;
}

void call__fread__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* filename = (var*)hm_get(fStack->stack[0], "filename");
    
    if (filename->type != _TMPString) {
        err->value = ERR_TYPE;
        char *str = strdup("fread function expect 1 parameter: filename(string). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(filename->type) + 1));
        strcat(str, getVarTypeName(filename->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }
    filename->value._string = getString(filename,err);

    FILE* file = fopen(filename->value._string, "r");
    if (file == NULL) {
        err->value = ERR_FILE;
        assignErrorMessage(err, "File not found\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    newVar->type = _string;
    newVar->value._string = (char*)malloc(sizeof(char));
    if (assignString(newVar, buffer) == 1) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    free(buffer);
    fclose(file);
}

void call__fwrite__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* filename = (var*)hm_get(fStack->stack[0], "filename");
    var* content = (var*)hm_get(fStack->stack[0], "content");
    var* method = (var*)hm_get(fStack->stack[0], "method");
    
    if (filename->type != _TMPString || content->type != _TMPString || method->type != _TMPString) {
        err->value = ERR_TYPE;
        char *str = strdup("fwrite function expect 3 parameters: filename(string), content(string), method(string). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(filename->type) + strlen(getVarTypeName(content->type) + strlen(getVarTypeName(method->type) + 1))));
        strcat(str, getVarTypeName(filename->type));
        strcat(str, ", ");
        strcat(str, getVarTypeName(content->type));
        strcat(str, ", ");
        strcat(str, getVarTypeName(method->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }
    filename->value._string = getString(filename,err);
    content->value._string = getString(content,err);
    method->value._string = getString(method,err);

    if (strcmp(method->value._string, "w") != 0 && strcmp(method->value._string, "a") != 0) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "fwrite function expect method to be 'w' or 'a'\n");
        return;
    }

    FILE* file = fopen(filename->value._string, method->value._string);
    if (file == NULL) {
        err->value = ERR_FILE;
        assignErrorMessage(err, "File not found\n");
        return;
    }

    fwrite(content->value._string, 1, strlen(content->value._string), file);

    fclose(file);

    newVar->value._int = 0;
}

void call__split__(hmStack* fStack, error* err) {
    var* newVar = newArrayVar(0, _string,err);
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* string = (var*)hm_get(fStack->stack[0], "string");
    var* delimiter = (var*)hm_get(fStack->stack[0], "delimiter");

    if (string->type != _TMPString || delimiter->type != _TMPString) {
        err->value = ERR_TYPE;
        char *str = strdup("split function expect 2 parameters: string(string), delimiter(string). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(string->type) + strlen(getVarTypeName(delimiter->type) + 1)));
        strcat(str, getVarTypeName(string->type));
        strcat(str, ", ");
        strcat(str, getVarTypeName(delimiter->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }
    string->value._string = getString(string,err);
    delimiter->value._string = getString(delimiter,err);

    // If the delimiter is an empty string, we split the string into characters
    if (strlen(delimiter->value._string) == 0) {
        for (int i = 0; i < strlen(string->value._string); i++) {
            if (newVar->value._array->length == newVar->value._array->capacity) {
                newVar->value._array->capacity *= 2;
                newVar->value._array->values = realloc(newVar->value._array->values, sizeof(var) * newVar->value._array->capacity);
                if (newVar->value._array->values == NULL) {
                    err->value = ERR_MEMORY;
                    assignErrorMessage(err, "Memory allocation error\n");
                    return;
                }
            }

            newVar->value._array->values[newVar->value._array->length].type = _string;
            newVar->value._array->values[newVar->value._array->length].value._string = (char*)malloc(2);
            if (assignString(&newVar->value._array->values[newVar->value._array->length], (char[]){string->value._string[i], '\0'}) == 1) {
                err->value = ERR_MEMORY;
                assignErrorMessage(err, "Memory allocation error\n");
                return;
            }

            newVar->value._array->length++;
        }

        newVar->value._array->values = realloc(newVar->value._array->values, sizeof(var) * newVar->value._array->length);
        if (newVar->value._array->values == NULL) {
            err->value = ERR_MEMORY;
            assignErrorMessage(err, "Memory allocation error\n");
            return;
        }

        newVar->value._array->capacity = newVar->value._array->length;
        newVar->type = _array;
        return;
    }

    char* token = strtok(string->value._string, delimiter->value._string);
    while (token != NULL) {
        if (newVar->value._array->length == newVar->value._array->capacity) {
            newVar->value._array->capacity *= 2;
            newVar->value._array->values = realloc(newVar->value._array->values, sizeof(var) * newVar->value._array->capacity);
            if (newVar->value._array->values == NULL) {
                err->value = ERR_MEMORY;
                assignErrorMessage(err, "Memory allocation error\n");
                return;
            }
        }

        newVar->value._array->values[newVar->value._array->length].type = _string;
        newVar->value._array->values[newVar->value._array->length].value._string = (char*)malloc(sizeof(char));
        if (assignString(&newVar->value._array->values[newVar->value._array->length], token) == 1) {
            err->value = ERR_MEMORY;
            assignErrorMessage(err, "Memory allocation error\n");
            return;
        }

        newVar->value._array->length++;
        token = strtok(NULL, delimiter->value._string);
    }

    newVar->value._array->values = realloc(newVar->value._array->values, sizeof(var) * newVar->value._array->length);
    if (newVar->value._array->values == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->value._array->capacity = newVar->value._array->length;
    newVar->type = _array;

}

void call__range__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* start = (var*)hm_get(fStack->stack[0], "start");
    var* end = (var*)hm_get(fStack->stack[0], "end");
    var* increment = (var*)hm_get(fStack->stack[0], "increment");
    
    if (start->type != _int || end->type != _int || increment->type != _int) {
        err->value = ERR_TYPE;
        char *str = strdup("range function expect 3 parameters: start(int), end(int), increment(int). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(start->type) + strlen(getVarTypeName(end->type) + strlen(getVarTypeName(increment->type) + 1))));
        strcat(str, getVarTypeName(start->type));
        strcat(str, ", ");
        strcat(str, getVarTypeName(end->type));
        strcat(str, ", ");
        strcat(str, getVarTypeName(increment->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    array* arr = malloc(sizeof(array));
    arr->type = _int;
    arr->capacity = 1;
    arr->length = 0;
    arr->values = malloc(sizeof(var) * arr->capacity);

    int i = start->value._int;
    while (i < end->value._int) {
        if (arr->length == arr->capacity) {
            arr->capacity *= 2;
            arr->values = realloc(arr->values, sizeof(var) * arr->capacity);
            if (arr->values == NULL) {
                err->value = ERR_MEMORY;
                assignErrorMessage(err, "Memory allocation error\n");
                return;
            }
        }

        arr->values[arr->length].type = _int;
        arr->values[arr->length].value._int = i;

        arr->length++;
        i += increment->value._int;
    }

    newVar->type = _array;
    newVar->value._array = arr;
}

void call__append__(hmStack* fStack, error* err){
    var* arrayToAppend = (var*)hm_get(fStack->stack[0], "array");
    var* varToAppend   = (var*)hm_get(fStack->stack[0], "toAdd");

    if(arrayToAppend->value._array->type != varToAppend->type){
        err->value = ERR_TYPE;
        char *str = strdup("append function expect 2 parameters: array(array), toAdd(type). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(arrayToAppend->value._array->type) + strlen(getVarTypeName(varToAppend->type) + 1)));
        strcat(str, getVarTypeName(arrayToAppend->value._array->type));
        strcat(str, ", ");
        strcat(str, getVarTypeName(varToAppend->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    int arrayLen = arrayToAppend->value._array->length;
    var *tmp = newArrayVar(arrayLen + 1, arrayToAppend->value._array->type,err);

    for(int i = 0; i < arrayLen; i++)
        var2var(&tmp->value._array->values[i], &arrayToAppend->value._array->values[i], err);

    var2var(&tmp->value._array->values[arrayLen], varToAppend, err);

    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", tmp);
}

void call__pop__(hmStack* fStack, error* err){
    var* arrayToPop = (var*)hm_get(fStack->stack[0], "array");

    if(arrayToPop->value._array->length == 0){
        err->value = ERR_TYPE;
        char *str = strdup("pop function expect 1 parameter: array(array) with at least 1 element. Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(arrayToPop->value._array->type) + 1));
        strcat(str, getVarTypeName(arrayToPop->value._array->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    int arrayLen = arrayToPop->value._array->length -1 ;
    var *tmp = newArrayVar(arrayLen, arrayToPop->value._array->type,err);

    for(int i = 0; i < arrayLen; i++)
        var2var(&tmp->value._array->values[i], &arrayToPop->value._array->values[i], err);

    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", tmp);
}