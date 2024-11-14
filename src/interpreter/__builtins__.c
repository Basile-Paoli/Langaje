#include "__builtins__.h"

void __builtinToMap__(hm* functionMap, error* err){
    function* functions[] = {
        declare__print__(err),
        declare__strlen__(err),
        declare__arrlen__(err),
        declare__randint__(err),
        declare__randfloat__(err),
        declare__system__(err),
        declare__input__(err),
        declare__randchoice__(err),
        declare__fread__(err),
        declare__fwrite__(err),
        declare__split__(err)
    };


    for (int i = 0; i < sizeof(functions) / sizeof(function*); i++) {
        hm_set(functionMap, functions[i]->name, functions[i]);
    }

    if (err->value != ERR_SUCCESS) {
        return;
    }
    
}

function *declare__print__(error *err){
    fakeFunctionParam message = {"message", _string};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 1);
    params[0] = message;
    return newFunctionPrototype("print", _void, params, __print__, 1, err);
}
void call__print__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));

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

function *declare__strlen__(error *err){
    fakeFunctionParam string = {"string", _string};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 1);
    params[0] = string;
    return newFunctionPrototype("strlen", _int, params, __strlen__, 1, err);
}
void call__strlen__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));

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

function *declare__arrlen__(error *err){
    fakeFunctionParam array = {"array", _array};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 1);
    params[0] = array;
    return newFunctionPrototype("arrlen", _int, params, __arrlen__, 1, err);
}
void call__arrlen__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));

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

function *declare__randint__(error *err){
    fakeFunctionParam min = {"min", _int};
    fakeFunctionParam max = {"max", _int};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 2);
    params[0] = min;
    params[1] = max;
    return newFunctionPrototype("randint", _int, params, __randint__, 2, err);
}
void call__randint__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));

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

function *declare__randfloat__(error *err){
    fakeFunctionParam min = {"min", _float};
    fakeFunctionParam max = {"max", _float};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 2);
    params[0] = min;
    params[1] = max;
    return newFunctionPrototype("randfloat", _float, params, __randfloat__, 2, err);
}
void call__randfloat__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));

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

function *declare__system__(error *err){
    fakeFunctionParam command = {"command", _string};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 1);
    params[0] = command;
    return newFunctionPrototype("system", _int, params, __system__, 1, err);
}
void call__system__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));

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

function* declare__input__(error* err){
    fakeFunctionParam message = {"message", _string};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 1);
    params[0] = message;
    return newFunctionPrototype("input", _string, params, __input__, 1, err);
}
void call__input__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));

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

function* declare__randchoice__(error* err){
    fakeFunctionParam array = {"array", _array};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 1);
    params[0] = array;
    return newFunctionPrototype("randchoice", _int, params, __randchoice__, 1, err);
}
void call__randchoice__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));

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

function* declare__fread__(error* err){
    fakeFunctionParam filename = {"filename", _string};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 1);
    params[0] = filename;
    return newFunctionPrototype("fread", _string, params, __fread__, 1, err);
}
void call__fread__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));

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

function* declare__fwrite__(error* err){
    fakeFunctionParam filename = {"filename", _string};
    fakeFunctionParam content = {"content", _string};
    fakeFunctionParam method = {"method", _string};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 3);
    params[0] = filename;
    params[1] = content;
    params[2] = method;
    return newFunctionPrototype("fwrite", _int, params, __fwrite__, 3, err);
}
void call__fwrite__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));

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

function* declare__split__(error* err){
    fakeFunctionParam string = {"string", _string};
    fakeFunctionParam delimiter = {"delimiter", _string};
    fakeFunctionParam* params = malloc(sizeof(fakeFunctionParam) * 2);
    params[0] = string;
    params[1] = delimiter;
    return newFunctionPrototype("split", _array, params, __split__, 2, err);
}
void call__split__(hmStack* fStack, error* err) {
    var* newVar = malloc(sizeof(var));

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