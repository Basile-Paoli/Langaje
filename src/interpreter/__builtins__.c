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
    
    if (message->type != _string) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "print function expect 1 parameter: message(string)\n");
        return;
    }

    printf("%s\n", message->value);

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
    
    if (string->type != _string) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "strlen function expect 1 parameter: string(string)\n");
        return;
    }

    newVar->value._int = strlen(string->value._string);
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
        assignErrorMessage(err, "arrlen function expect 1 parameter: array(array)\n");
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
        assignErrorMessage(err, "randint function expect 2 parameters: min(int), max(int)\n");
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
        assignErrorMessage(err, "randfloat function expect 2 parameters: min(float), max(float)\n");
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
        assignErrorMessage(err, "system function expect 1 parameter: command(string)\n");
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
    
    if (message->type != _string) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "input function expect 1 parameter: message(string)\n");
        return;
    }


    char* input = (char*)malloc(255);
    printf("%s", message->value._string);
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
        assignErrorMessage(err, "randchoice function expect 1 parameter: array(array)\n");
        return;
    }

    newVar->value._int = rand() % array->value._array->length;
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
    
    if (filename->type != _string) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "fread function expect 1 parameter: filename(string)\n");
        return;
    }

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
    
    if (filename->type != _string || content->type != _string || method->type != _string) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "fwrite function expect 3 parameters: filename(string), content(string), method(string)\n");
        return;
    }

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
    var* delimiter = (var*)hm_get(fStack->stack[0], "delimiter");
    
    if (string->type != _string || delimiter->type != _string) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "split function expect 2 parameters: string(string), delimiter(string)\n");
        return;
    }

    array* arr = malloc(sizeof(array));
    arr->type = _string;
    arr->capacity = 1;
    arr->length = 0;
    arr->values = malloc(sizeof(var) * arr->capacity);

    char* token = strtok(string->value._string, delimiter->value._string);
    while (token != NULL) {
        if (arr->length == arr->capacity) {
            arr->capacity *= 2;
            arr->values = realloc(arr->values, sizeof(var) * arr->capacity);
            if (arr->values == NULL) {
                err->value = ERR_MEMORY;
                assignErrorMessage(err, "Memory allocation error\n");
                return;
            }
        }

        arr->values[arr->length].type = _string;
        arr->values[arr->length].value._string = (char*)malloc(sizeof(char));
        if (assignString(&arr->values[arr->length], token) == 1) {
            err->value = ERR_MEMORY;
            assignErrorMessage(err, "Memory allocation error\n");
            return;
        }

        arr->length++;
        token = strtok(NULL, delimiter->value._string);
    }

    newVar->type = _array;
    newVar->value._array = arr;
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
        assignErrorMessage(err, "range function expect 3 parameters: start(int), end(int), increment(int)\n");
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
        assignErrorMessage(err, "Array elements type and element to append must be of same type");
        return;
    }

    int arrayLen = arrayToAppend->value._array->length;
    var *tmp = newArrayVar(arrayLen + 1, arrayToAppend->value._array->type);

    for(int i = 0; i < arrayLen; i++)
        var2var(&tmp->value._array->values[i], &arrayToAppend->value._array->values[i], err);

    var2var(&tmp->value._array->values[arrayLen], varToAppend, err);

    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", tmp);
}

void call__pop__(hmStack* fStack, error* err){
    var* arrayToPop = (var*)hm_get(fStack->stack[0], "array");

    int arrayLen = arrayToPop->value._array->length -1 ;
    var *tmp = newArrayVar(arrayLen, arrayToPop->value._array->type);

    for(int i = 0; i < arrayLen; i++)
        var2var(&tmp->value._array->values[i], &arrayToPop->value._array->values[i], err);

    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", tmp);
}