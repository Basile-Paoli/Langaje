#include "__builtins__.h"

char *replace_str(char *str) {
    char *newStr = (char *) malloc(strlen(str) + 1);
    if (newStr == NULL) {
        return NULL;
    }

    int i = 0;
    int j = 0;
    while (str[i] != '\0') {
        if (str[i] == '\\' && str[i + 1] == 'n') {
            newStr[j] = '\n';
            i++;
        } else if (str[i] == '\\' && str[i + 1] == 't') {
            newStr[j] = '\t';
            i++;
        } else {
            newStr[j] = str[i];
        }
        i++;
        j++;
    }
    newStr[j] = '\0';

    return newStr;
}

void display__print(var* value, error* err, int level) {
    switch (value->type) {
    case _TMPString:
        char* str = getString(value,err);
        str = replace_str(str);
        printf("%s", str);
        free(str);
        break;
    case _int:
        printf("%d", value->value._int);
        break;
    case _float:
        printf("%f", value->value._float);
        break;
    case _char:
        printf("%c", value->value._char);
        break;
    case _string:
        printf("%s", value->value._string);
        break;
    case _array:
        for (int i = 0; i < value->value._array->length; i++) {
            display__print(&value->value._array->values[i], err, level + 1);
        }
        break;
    default:
        break;
    }
}


void __builtinToMap__(hm* functionMap, error* err){
    function* functions[] = {
        newFunctionPrototype("print",        _void,     __print__,      1, err, (fakeFunctionParam[]){{"entry", _string}}),
        newFunctionPrototype("len",          _int,      __len__,        1, err, (fakeFunctionParam[]){{"entry", _string}}),
        newFunctionPrototype("randint",      _int,      __randint__,    2, err, (fakeFunctionParam[]){{"min", _int}, {"max", _int}}),
        newFunctionPrototype("randfloat",    _float,    __randfloat__,  2, err, (fakeFunctionParam[]){{"min", _float}, {"max", _float}}),
        newFunctionPrototype("system",       _int,      __system__,     1, err, (fakeFunctionParam[]){{"command", _string}}),
        newFunctionPrototype("input",        _string,   __input__,      1, err, (fakeFunctionParam[]){{"message", _string}}),
        newFunctionPrototype("randchoice",   _int,      __randchoice__, 1, err, (fakeFunctionParam[]){{"array", _array}}),
        newFunctionPrototype("fread",        _string,   __fread__,      1, err, (fakeFunctionParam[]){{"filename", _string}}),
        newFunctionPrototype("fwrite",       _int,      __fwrite__,     3, err, (fakeFunctionParam[]){{"filename", _string}, {"content", _string}, {"method", _string}}),
        newFunctionPrototype("split",        _array,    __split__,      2, err, (fakeFunctionParam[]){{"string", _string}, {"delimiter", _string}}),
        newFunctionPrototype("range",        _array,    __range__,      3, err, (fakeFunctionParam[]){{"start", _int}, {"end", _int}, {"step", _int}}),
        newFunctionPrototype("append",       _array,    __append__,     2, err, (fakeFunctionParam[]){{"array", _array}, {"toAdd", _void}}),
        newFunctionPrototype("pop",          _array,    __pop__,        1, err, (fakeFunctionParam[]){{"array", _array}}),
        newFunctionPrototype("type",         _string,   __type__,       1, err, (fakeFunctionParam[]){{"entry", _void}}),
        newFunctionPrototype("ord",          _int,      __ord__,        1, err, (fakeFunctionParam[]){{"entry", _char}}),
        newFunctionPrototype("char",         _char,     __char__,       1, err, (fakeFunctionParam[]){{"entry", _int}})
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

    var* message = (var*)hm_get(fStack->stack[0], "entry");
    
    display__print(message, err, 0);

    newVar->value._int = 0;
}

void call__len__(hmStack* fStack, error* err) {

    var* newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var* entry = (var*)hm_get(fStack->stack[0], "entry");
    
    switch (entry->type) {
        case _TMPString:
            char *tmp = getString(entry,err);
            newVar->value._int = strlen(tmp);
            free(tmp);
            break;
        case _array:
            newVar->value._int = entry->value._array->length;
            break;
        default:
            err->value = ERR_TYPE;
            char *str = strdup("len function expect 1 parameter: entry(TMPString or Array). Got -> ");
            str = realloc(str, strlen(str) + strlen(getVarTypeName(entry->type) + 1));
            strcat(str, getVarTypeName(entry->type));
            assignErrorMessage(err, str);
            free(str);
            break;
    }

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
    char* str = getString(command,err);
    str = replace_str(str);
    
    newVar->value._int = system(command->value._string);
    
    free(str);
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
    str = replace_str(str);

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
    char *str = getString(filename,err);
    str = replace_str(str);

    FILE* file = fopen(str, "r");
    if (file == NULL) {
        err->value = ERR_FILE;
        assignErrorMessage(err, "File not found\n");
        return;
    }
    free(str);

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
    char *filenameStr = getString(filename,err);
    filenameStr = replace_str(filenameStr);
    char *contentStr = getString(content,err);
    contentStr = replace_str(contentStr);
    char *methodStr = getString(method,err);
    methodStr = replace_str(methodStr);

    if (strcmp(methodStr, "w") != 0 && strcmp(methodStr, "a") != 0) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "fwrite function expect method to be 'w' or 'a'\n");
        return;
    }

    FILE* file = fopen(filenameStr, methodStr);
    if (file == NULL) {
        err->value = ERR_FILE;
        assignErrorMessage(err, "File not found\n");
        return;
    }

    fwrite(contentStr, 1, strlen(contentStr), file);

    fclose(file); free(filenameStr); free(contentStr); free(methodStr);

    newVar->value._int = 0;
}

void call__split__(hmStack* fStack, error* err) {

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
    char *stringStr = getString(string,err);
    stringStr = replace_str(stringStr);
    char *delimiterStr = getString(delimiter,err);
    delimiterStr = replace_str(delimiterStr);

    // If the delimiter is empty, we return an array with every character of the string
    if (strlen(delimiterStr) == 0) {
        
        var *newVar = newArrayVar(strlen(stringStr), _string, err);
        if (newVar == NULL) {
            return;
        }

        for (int i = 0; i < strlen(stringStr); i++) {
            newVar->value._array->values[i].type = _string;
            newVar->value._array->values[i].value._string = (char*)malloc(2);
            newVar->value._array->values[i].value._string[0] = stringStr[i];
            newVar->value._array->values[i].value._string[1] = '\0';
        }

        hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

        free(stringStr); free(delimiterStr);

        return;
    }

    // We count the number of occurences of the delimiter in the string
    int count = 0;
    for (int i = 0; i < strlen(stringStr); i++) {
        if (stringStr[i] == delimiterStr[0]) {
            count++;
        }
    }

    // We create the array that will contain the splitted string
    var *newVar = newArrayVar(count + 1, _string, err);
    if (newVar == NULL) {
        return;
    }

    // We split the string using strtok
    char *token = strtok(stringStr, delimiterStr);
    int i = 0;
    while (token != NULL) {
        newVar->value._array->values[i].type = _string;
        newVar->value._array->values[i].value._string = strdup(token);
        token = strtok(NULL, delimiterStr);
        i++;
    }

    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    free(stringStr); free(delimiterStr); free(token);

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

    if(arrayToAppend->type == _string){
        arrayToAppend->type = _TMPString;
    }
    if(arrayToAppend->value._array->type == _string){
        arrayToAppend->value._array->type = _TMPString;
    }
    if(varToAppend->type == _TMPString){
        varToAppend->type = _string;
        varToAppend->value._string = getString(varToAppend,err);
    }

    int arrayLen = arrayToAppend->value._array->capacity;
    var *tmp = newArrayVar(arrayLen + 1, arrayToAppend->value._array->type,err);
    for(int i = 0; i < arrayLen; i++){
        var2var(&tmp->value._array->values[i], &arrayToAppend->value._array->values[i], err);
    }
        

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
    
    for(int i = 0; i < arrayLen; i++){
        var2var(&tmp->value._array->values[i], &arrayToPop->value._array->values[i], err);
    }
    
    

    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", tmp);
}

void call__type__(hmStack *fStack, error *err) {
    var *newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = 1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var *entry = (var *) hm_get(fStack->stack[0], "entry");

    newVar->value._string = strdup(getVarTypeName(entry->type));
    newVar->type = _string;
}

void call__ord__(hmStack *fStack, error *err) {
    var *newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = -1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var *entry = (var *) hm_get(fStack->stack[0], "entry");

    if (entry->type != _TMPString) {
        err->value = ERR_TYPE;
        char *str = strdup("ord function expect 1 parameter: entry(string). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(entry->type) + 1));
        strcat(str, getVarTypeName(entry->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }
    
    char* str = getString(entry,err);
    str = replace_str(str);
    
    if (strlen(str) != 1) {
        err->value = ERR_TYPE;
        assignErrorMessage(err, "ord function expect a string of length 1\n");
        return;
    }

    newVar->value._int = str[0];

    free(str);
}

void call__char__(hmStack *fStack, error *err) {
    var *newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        err->value = ERR_MEMORY;
        assignErrorMessage(err, "Memory allocation error\n");
        return;
    }

    newVar->type = _int;
    newVar->value._int = -1;
    hm_set(fStack->stack[0], "!!$RETURNVALUE$!!", newVar);

    var *entry = (var *) hm_get(fStack->stack[0], "entry");

    if (entry->type != _int) {
        err->value = ERR_TYPE;
        char *str = strdup("char function expect 1 parameter: entry(int). Got -> ");
        str = realloc(str, strlen(str) + strlen(getVarTypeName(entry->type) + 1));
        strcat(str, getVarTypeName(entry->type));
        assignErrorMessage(err, str);
        free(str);
        return;
    }

    newVar->value._string = (char*)malloc(2);
    newVar->value._string[0] = entry->value._int;
    newVar->value._string[1] = '\0';
    newVar->type = _string;
}