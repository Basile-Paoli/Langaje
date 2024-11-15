#include "interpreter.h"

/**
* Function that replace the value of the variable in a node by its value in the hashmaps stack.
* Raise error if the value doesn't exist
*/

var* subsituteValue(astNode* value, hmStack* stack, error *err){
    int hmIndex = isInStackDownwards(stack,value->value.variable);
    
    if(hmIndex == -1){
        err->value = ERR_NOT_FOUND;
        char *msg = malloc(strlen("Value not found : %s") + 50);
        sprintf(msg, "Value not found : %s", value->value.variable);

        assignErrorMessage(err, msg);
        free(msg);

        printf("Value not found : %s\n",value->value.variable);
        
    } else {
        var* tmp = malloc(sizeof(var));
        var tmp2 = *(var*)hm_get(stack->stack[hmIndex],value->value.variable);
        tmp->type = tmp2.type;
        if(tmp->type != _string){
            tmp->value = tmp2.value;
        } else {
            assignString(tmp, tmp2.value._string);
        }
        
        return tmp;
    }

    
}

/**
* Function that calculate the result of a mathematical operation for two values.
* Directly replaces the value of the operation node, returns void.
*/

astNode* calculateNode(astNode** values, astNode* node,hmStack* stack, int valuesAmount, error *err){
    operator op = node->value.operator;
    error err_op;
    err_op.value = ERR_SUCCESS;
    int hasSubsituted = 0;
    var var1;
    
    if(values[0]->type == VARIABLE){   
        var1 = *subsituteValue(values[0],stack, err);
        hasSubsituted = 1;
    } else if(values[0]->type == VALUE){
        var1 = values[0]->value.value;
    } else {
        var1 = *values[0]->value.referencedValue;
    }
    var var2;
    if(valuesAmount > 1){
        if(values[1]->type == VARIABLE){
            var2 = *subsituteValue(values[1],stack,err);
        } else if(values[1]->type == VALUE){
            var2 = values[1]->value.value;
        } else {
            var2 = *values[1]->value.referencedValue;
        }
    }
    astNode* tmpNode = malloc(sizeof(astNode));
    tmpNode->type = VALUE;
    

    switch(op){
        case ADDITION:{
            // Return error msg if array values not of same type
            if(var1.type == _array && var2.type == _array) {
                if (var1.value._array->values[0].type != var2.value._array->values[0].type) {
                    err_op.value = ERR_TYPE;
                    assignErrorMessage(&err_op, "Elements array must be of same type");
                    break;
                }
            }

            tmpNode->value.value =  add(&var1,&var2, &err_op);
            break;
        }
        case SUBTRACTION:{
            tmpNode->value.value =  substract(&var1,&var2,&err_op);
            break;
        }
        case MULTIPLICATION:{
            tmpNode->value.value =  multiply(&var1,&var2,&err_op);
            break;
        }
        case DIVISION:{
            tmpNode->value.value =  divide(&var1,&var2,&err_op);
            break;
        }
        case EQUAL:{
            tmpNode->value.value = isEqual(&var1,&var2,0,&err_op);
            break;
        }
        case NOT_EQUAL:{
            tmpNode->value.value = isEqual(&var1,&var2,1,&err_op);
            break;
        }
        case GREATER:{
            tmpNode->value.value = isGreater(&var1,&var2,1,&err_op);
            break;
        }
        case GREATER_EQUAL:{
            tmpNode->value.value = isGreater(&var1,&var2,0,&err_op);
            break;
        }
        case LESS:{
            tmpNode->value.value = isLesser(&var1,&var2,1,&err_op);
            break;
        }
        case LESS_EQUAL:{
            tmpNode->value.value = isLesser(&var1,&var2,0,&err_op);
            break;
        }
        case OR:{
            tmpNode->value.value = valueOr(&var1,&var2,&err_op);
            break;
        }
        case AND:{
            tmpNode->value.value = valueAnd(&var1,&var2,&err_op);
            break;
        }
        case NOT:{
            tmpNode->value.value = valueReverse(&var1,&err_op);
            break;
        }
        case SUBSCRIPT:{
            //If it has substituted means value is in &var1 else it's in values[0].value.referencedValue 
            //We work with pointer because we go edit directly the memory of the array.
            if(hasSubsituted == 1){
                tmpNode->value.referencedValue = getVarPointerFromArray(&var1,var2.value._int,err);
            } else {
                tmpNode->value.referencedValue = getVarPointerFromArray(values[0]->value.referencedValue,var2.value._int,err);
            }
        tmpNode->type = POINTER;
            
    
            break;
        }
        case UNARY_MINUS:{
            tmpNode->value.value = unaryMinus(&var1,&err_op);
            break;
        }
        default:{
            err->value = ERR_UNKNOWN_OPERATOR;
            assignErrorMessage(err, "Unknown operator");
            //printf("UNKNOWN OPERATOR\n");
            break;
        }
    }

    if(err_op.value != ERR_SUCCESS){
        // Get the error message if one of the basic function doesn't work
        err->value = err_op.value;
        err->message = malloc(strlen(err_op.message));
        sprintf(err->message, "%s", err_op.message);
        //return NULL;
    }

    return tmpNode;
}


/*
* Function that declare an array based on a ARRAY node in the AST.
* Returns the array
*/
var* declareArray(astNode* node, initType* type, hmStack* stack, hm* functionMap, error *err){
    if(type->type !=  _array){
        return &node->value.value;
    } else {
        
        if(node->childrenCount > type->arraySize && type->arraySize != 0){
            err->value = ERR_OUT_OF_BOUNDS;
        
            char *msg = malloc(strlen("Array size too large. Expected maximum size: %d, but received: %d") + 40);
            sprintf(msg, "The number of elements in the array exceeds the specified size limit. Expected maximum size: %d, but received: %d", type->arraySize, node->childrenCount);

            assignErrorMessage(err, msg);
            free(msg);

            return NULL;
        }
        //Declare array (types.c)
        var* arr = newArrayVar(node->childrenCount, type->elementsType->type);
        for(int i = 0; i < node->childrenCount; i++){
            //For each children call the function recursively
            var* subVar = declareArray(node->children[i],type->elementsType, stack, functionMap, err);
            if(subVar == NULL)return NULL;

            astNode* tmp = computeNode(node->children[i],stack,functionMap,err);
            
            if(tmp->type == POINTER){
                subVar->type = tmp->value.referencedValue->type;   
                var2var(subVar,tmp->value.referencedValue,err);
            } else if (tmp->type == VALUE){
                subVar->type = tmp->value.value.type;   
                var2var(subVar, &tmp->value.value,err);
            }
            if(subVar->type != arr->value._array->type){
                err->value = ERR_TYPE;

                char *msg = malloc(strlen("Wrong type specified in array, expected %d, got %d") + 50);
                sprintf(msg, "Wrong type specified in array, expected %s, got %s", getVarTypeName(arr->value._array->type), getVarTypeName(subVar->type));

                assignErrorMessage(err, msg);
                free(msg);
                return NULL;
            }
            //Assign the subvar to the array slot  (either NODE so value OR Subarray)
            var2var(&arr->value._array->values[i],subVar, err);
        }
        arr->type = type->type;
        return arr;
    }
}

/*
* Function that declares an empty array based on initialization node.
*/
var* declareEmptyArray(astNode* node, error *err){
    initType* curr = &node->value.initialization.type;
    
    //Declare main array
    var* arr = newArrayVar(node->value.initialization.type.arraySize, node->value.initialization.type.elementsType->type);

    //Go to next subarray if there's one
    initType oldCurr = *curr;
    curr = curr->elementsType;
    var* currArray = arr;
    while(curr->type == _array){
        var* newArr;
        for(int i = 0; i < oldCurr.arraySize; i++){
            newArr = newArrayVar(curr->arraySize,curr->elementsType->type);
            newArr->type = oldCurr.type;

            var2var(&currArray->value._array->values[i],newArr, err);
        }

        oldCurr = *curr;
        curr = curr->elementsType;
        currArray = newArr;
    }    
    return arr;
}

/*
* Function that declares an empty var based on initialization node.
*/
var* declareVar(astNode* node, error *err){
    var* newVar = malloc(sizeof(var));
    newVar->type = node->value.initialization.type.type;
    switch(newVar->type){
        case _int:{
            newVar->value._float = 0;
            break;
        }
        case _float:{
            newVar->value._float = 0.0;
            break;
        }
        case _string:{
            assignString(newVar,"");
            break;
        }
        default:
            err->value = ERR_TYPE;
            break;
    }

    return newVar;
}

/**
* Function that either : 
*   Create a new variable in the top hashmap of the stack, if the value doesn't exist in an other hashmap 
*   Change the value of a variable if it's found in the stack 
* Return 1 on success, 0 on failure.
*/

int assignValueToHashmap(astNode* nodeToAssign, astNode* valueToAssign, hmStack* stack, hm* functionMap, error *err){

    if(nodeToAssign->type == VARIABLE || nodeToAssign->type == INITIALIZATION){
        if(valueToAssign->type == ARRAY){
            var* newArr = declareArray(valueToAssign, &nodeToAssign->value.initialization.type, stack,functionMap, err);

            if(newArr == NULL){
                //RAISE ERROR MAYBE?
                return 0;
            }
            int hmIndex = isInStackDownwards(stack,nodeToAssign->value.variable);
            if(hmIndex > -1){
                var* oldArr = (var*)hm_get(stack->stack[hmIndex],nodeToAssign->value.initialization.name);
                var2var(oldArr,newArr,err);
                return 1;
            }

            err->value = ERR_NOT_FOUND;
            assignErrorMessage(err, "Value not found in HM");

            return 0;


        } else {

            int hmIndex = isInStackDownwards(stack,nodeToAssign->value.variable);

            if(hmIndex > -1){
                var* tmp = (var*)hm_get(stack->stack[hmIndex],nodeToAssign->value.variable);
                if(valueToAssign->type == VALUE || valueToAssign->type == POINTER){
                    if(valueToAssign->type != POINTER){
                        var2var(tmp, &(valueToAssign->value.value), err);
                    } else{
                        var2var(tmp, valueToAssign->value.referencedValue, err);
                    }
                } else {
                    var tmpVar = *subsituteValue(valueToAssign, stack, err);
                    var2var(tmp, &(tmpVar), err);
                }
                
                return 1;
            }

            err->value = ERR_NOT_FOUND;
            assignErrorMessage(err, "Value not found in HM");

            return 0;
        }
    } else {
        //FOR ARRAYS SUBSCRIPT
        if(nodeToAssign->type  == POINTER){
            if(valueToAssign->type == POINTER){
                var2var(nodeToAssign->value.referencedValue,valueToAssign->value.referencedValue,err);
            } else {
                var2var(nodeToAssign->value.referencedValue,&valueToAssign->value.value,err);
            }
            
            return 1;
        }

        err->value = ERR_NOT_FOUND;
        assignErrorMessage(err, "Value not found in HM");

        return 0;
    }
    return 0;
}


int initializeValueInHM(astNode* node,hmStack* stack, error *err){
    int hmIndex = isInStackDownwards(stack, node->value.variable);
    if(hmIndex > - 1){
        err->value = ERR_ALREADY_EXISTS;
        assignErrorMessage(err, "Can't define a variable that already exists");

        return 0;
    }
    if(node->value.initialization.type.type == _array){
        var* newVar = declareEmptyArray(node, err);
        hm_set(stack->stack[stack->length-1],node->value.initialization.name,newVar);
    } else {
        var* newVar = declareVar(node, err);
        hm_set(stack->stack[stack->length-1],node->value.initialization.name,newVar);
        return 1;
    }

}

int runWhileLoop(astNode* node,hmStack* stack,hm* functionMap,error* err){
    astNode condition = *node->children[0];

    InstructionBlock instructions = *(node->children[1]->value.block);
    astNode* result;

    result = computeNode(&condition,stack,functionMap, err);
    int shouldContinue = result->value.value.value._int;
    while(shouldContinue == 1){
        hm* hashmap = hm_create();
        hmStackPush(stack,hashmap);
        if(runInstructionBlock(&instructions,stack,functionMap,err) != 0){
            //printf("%s\n", err->message);
            hmStackPop(stack);
            return 1;
        }
        hmStackPop(stack);
        condition = *(node->children[0]);
        result = computeNode(&condition,stack,functionMap,err);
        shouldContinue = result->value.value.value._int;
    }
    return 0;
}


/**
 * Run for loop condition
 */
int runForLoop(astNode *node, hmStack *stack, hm* functionMap, error *err) {
    //Copy instruction block to run 
    InstructionBlock instructions = *(node->children[node->childrenCount-1]->value.block);

    //Default increment value
    int incrementValue = 1;

    //Create var for incrementation
    var* newVar = malloc(sizeof(var));
    newVar->type = _int;
    newVar->value._int = 0;
    //If we have more than 2 children, compute the start value
    if(node->childrenCount > 2){
        newVar->value._int = computeNode(node->children[0],stack,functionMap,err)->value.value.value._int;
    }
    hm_set(stack->stack[stack->length-1],node->value.variable,newVar);
    int hmIndex = stack->length-1;
    
    //If we have 4 children, means we have a different increment value
    if(node->childrenCount == 4){
        incrementValue = computeNode(node->children[node->childrenCount-2],stack,functionMap,err)->value.value.value._int;
    }
    //Ternary to know which child to select
    astNode conditionNode = node->childrenCount == 4 ? *node->children[node->childrenCount-3] : *node->children[node->childrenCount-2];
    int conditionValue = computeNode(&conditionNode,stack,functionMap,err)->value.value.value._int;
     
    
    int loopIndexValue = newVar->value._int;
    
    if(loopIndexValue > conditionValue){
        while (loopIndexValue > conditionValue) {
            hm* hashmap = hm_create();
            hmStackPush(stack,hashmap);        
            if(runInstructionBlock(&instructions, stack, functionMap,err) != 0){
                hmStackPop(stack);
                return 1;
            };
            newVar->value._int += incrementValue;
            hm_set(stack->stack[hmIndex], node->value.variable, newVar);
            loopIndexValue =  newVar->value._int;
            hmStackPop(stack);
        }
    } else {
        while (loopIndexValue < conditionValue) {
            hm* hashmap = hm_create();
            hmStackPush(stack,hashmap);
            if(runInstructionBlock(&instructions, stack, functionMap,err) != 0){
                hmStackPop(stack);
                return 1;
            };
            newVar->value._int += incrementValue;
            hm_set(stack->stack[hmIndex], node->value.variable, newVar);
            loopIndexValue =  newVar->value._int;
            hmStackPop(stack);
        }
    }
    

    return 0;
}



int declareFunction(astNode* node,hmStack* stack,hm* functionMap,error* err){
    functionDeclarationNode fun = node->value.functionDeclaration;
    if((struct function*)hm_get(functionMap,fun.name) != NULL){
        err->value = ERR_ALREADY_EXISTS;
        assignErrorMessage(err,strcat("Function redefinition : ", fun.name));
        return 0;
    }
    function* funTmp = malloc(sizeof(function));
    funTmp->name = malloc((strlen(fun.name) + 1) * sizeof(char));
    strcpy(funTmp->name,strdup(fun.name));
    funTmp->parameters = fun.parameters;
    funTmp->parametersCount = fun.parametersCount;

    funTmp->type = fun.returnType.type;
    funTmp->voidReturn = fun.voidReturn;
    funTmp->instructions = node->children[0]->value.block;
    funTmp->isBuiltin = 0;

    hm_set(functionMap, fun.name, funTmp);

    return 1;
}

astNode* runBuiltinFunction(astNode* node, hmStack* stack, hm* functionMap, function* fun, error* err){

    hm* Fhashmap = hm_create();
    hmStack* functionStack = hmStackCreate(1);
    for(int i = 0; i < node->childrenCount; i++){
        astNode* subNode = computeNode(node->children[i],stack,functionMap,err);
        var* tmp = malloc(sizeof(var));
        if(subNode->type == VARIABLE){
            tmp = subsituteValue(subNode, stack, err);
            hm_set(Fhashmap, fun->parameters[i].name, tmp);        
        
        } else {
            tmp->type = subNode->value.value.type;
            var2var(tmp,&subNode->value.value,err);
            hm_set(Fhashmap, fun->parameters[i].name , tmp);
        }
    }

    hmStackPush(functionStack,Fhashmap);

    //ABOVE CREATES A HASHMAP TO BIND PARAMETERS GIVEN BY THE USER, WITH THE KEYS DEFINED IN THE FAKE FUNCTION PROTOTYPE.
    //PASS THE FUNCTIONSTACK TO YOUR PROGRAM TO RETRIEVE THE VALUES
    //RETURN THE VALUE IN !!$RETURNVALUE$!!
    //hm_set(stack->stack[0], "!!$RETURNVALUE$!!", returnValue);
    //RETURN VALUE MUST BE A MALLOC VAR 

    void (*builtinFunctions[])(hmStack*, error*) = {
        [__print__]         = call__print__,
        [__strlen__]        = call__strlen__,
        [__arrlen__]        = call__arrlen__,
        [__randint__]       = call__randint__,
        [__randfloat__]     = call__randfloat__,
        [__system__]        = call__system__,
        [__input__]         = call__input__,
        [__randchoice__]    = call__randchoice__,
        [__fread__]         = call__fread__,
        [__fwrite__]        = call__fwrite__,
        [__split__]         = call__split__,
        [__range__]         = call__range__,
        [__append__]        = call__append__,
        [__pop__]           = call__pop__
    };

    builtinFunctions[fun->__builtinIdentifier__](functionStack, err);

    var* returnValue = (var*)hm_get(Fhashmap, "!!$RETURNVALUE$!!");
    astNode* tmpNode = malloc(sizeof(astNode));
    tmpNode->value.value.type = returnValue->type;
    tmpNode->value.value.value = returnValue->value;

    hmStackPop(functionStack);
    tmpNode->type = VALUE;
    return tmpNode;
}

astNode* runFunction(astNode* node, hmStack* stack, hm* functionMap, error* err){

    function* fun = (struct function*)hm_get(functionMap,node->value.functionCall.name);
    if(fun->isBuiltin == 1){
        //Run builtin function generate its own tmp node
        return runBuiltinFunction(node,stack,functionMap,fun,err);
    } else {
        if(fun == NULL){
            printf("FUN NOT FOUND\n");
            //Error binding doesnt work. need to fix ?
            return NULL;
        }
        if(node->childrenCount < fun->parametersCount){
            printf("__TOO FEW ARGS__\n");
            //Too few args 
            return NULL;
        } else if (node->childrenCount > fun->parametersCount){
            //Too many args
            printf("__TOO MANY ARGS__\n");
            return NULL;
        }

        hm* Fhashmap = hm_create();
        hmStack* functionStack = hmStackCreate(1);
        for(int i = 0; i < node->childrenCount; i++){
            astNode* subNode = computeNode(node->children[i],stack,functionMap,err);
            var* tmp = malloc(sizeof(var));
            if(subNode->type == VARIABLE){
                var* tmpV = subsituteValue(subNode, stack, err);
                tmp->type = tmpV->type;
                var2var(tmp,tmpV,err);
                hm_set(Fhashmap, fun->parameters[i].name, tmp);        
            
            } else {
                tmp->type = subNode->value.value.type;
                var2var(tmp,&subNode->value.value,err);
                hm_set(Fhashmap, fun->parameters[i].name , tmp);
            }
        }

        hmStackPush(functionStack,Fhashmap);
        runInstructionBlock(fun->instructions, functionStack, functionMap, err);
        

        var* returnValue = (var*)hm_get(Fhashmap, "!!$RETURNVALUE$!!");
        astNode* tmpNode = malloc(sizeof(astNode));
        tmpNode->value.value.type = returnValue->type;

        var2var(&tmpNode->value.value, returnValue,err);

        hmStackPop(functionStack);
        tmpNode->type = VALUE;

        return tmpNode;
    }
    

}
/**
* Function that compute a node of the AST. 
* Recursively call on each child of the node.
* Call a new runInstructionBlock if a block of code is found in the AST.
*/

astNode* computeNode(astNode* node, hmStack* stack, hm* functionMap, error *err){

    if(node->childrenCount == 0 && (node->type != INITIALIZATION && node->type != MEMORY_DUMP && node->type != BREAKPOINT)){
        return node; //Send the whole node back
    }   

    astNode** values = malloc(sizeof(astNode*) * node->childrenCount + 1);
    int valuesAmount = 0;
    for(int i = 0; i < node->childrenCount; i++){
        if(node->children[i] == NULL)continue;
        valuesAmount++;


        //IF WE ARE ON THEN
        if(i == 1 && node->type == CONDITION && values[0]->value.value.value._int == 1 && node->children[i]->type != CONDITION){
            if(node->children[i]->type == BLOCK){
                hm* hashmap = hm_create();
                hmStackPush(stack,hashmap);
                if(runInstructionBlock(node->children[i]->value.block, stack, functionMap,err) != 0){
                    return NULL;
                }
                hmStackPop(stack);

            }
            i+=1;
        } else if(i == 2 && node->type == CONDITION && values[0]->value.value.value._int == 0 && node->children[i]->type != CONDITION){
            
            if(node->children[i]->type == BLOCK){
                
                hm* hashmap = hm_create();
                hmStackPush(stack,hashmap);
                if(runInstructionBlock(node->children[i]->value.block, stack, functionMap,err) != 0){
                    return NULL;
                };
                hmStackPop(stack);

            }

            i+=1;
        } else if(node->type == WHILE_LOOP){
            if(runWhileLoop(node,stack,functionMap,err) != 0){
                return NULL;
            };
            break;
        } else if(node->type == FOR_LOOP){
            if(runForLoop(node,stack,functionMap,err) != 0){
                return NULL;
            };
            break;
        } else{
            values[i] = computeNode(node->children[i], stack,functionMap, err);
        }
    }
    
    if(node->type == INITIALIZATION){
        initializeValueInHM(node, stack, err);
        return node;
    } else if((node->type == OPERATOR && node->value.operator == ASSIGNMENT)){
        if(values[0] == NULL || values[1] == NULL){
            //ERROR
            return NULL;
        }
        assignValueToHashmap(values[0], values[1], stack,functionMap, err);
    } else if(node->type == OPERATOR && valuesAmount > 0){
        return calculateNode(values, node, stack, valuesAmount, err);
        free(values);
       return node;
    } else if (node->type == FUNCTION_DECLARATION){
        declareFunction(node,stack,functionMap,err);
        return node;
    } else if (node->type == FUNCTION_CALL) {
        return runFunction(node, stack, functionMap, err);
    } else if(node->type == RETURN){
        //IF VOID RETURN DONT DO THAT WHEN BASILE HAVE FIXED TODO
        var* returnValue = malloc(sizeof(var));
        if(values[0]->type == VARIABLE){
            var* tmpV = subsituteValue(values[0],stack,err);
            returnValue->type = tmpV->type;
            var2var(returnValue,tmpV,err);
        } else if(values[0]->type == VALUE){
            returnValue->type = values[0]->value.value.type;
            var2var(returnValue,&values[0]->value.value,err);
        } else {
            returnValue->type = values[0]->value.referencedValue->type;
            var2var(returnValue,values[0]->value.referencedValue,err);
        }

        hm_set(stack->stack[0], "!!$RETURNVALUE$!!", returnValue);
        return NULL;
    } else if (node->type == MEMORY_DUMP){
        displayHashmap(stack,err);
        return node;
    } else {
        return node;
    }
}


/**
* Function that allocate a hashmap and push it on top of the stack
* Runs the instruction block sent in parameters.
* After the run, pop the hashmap on top of the stack.
*/

int runInstructionBlock(InstructionBlock* program, hmStack* stack, hm* functionMap, error *err){
    for(int i = 0; i < program->instructionsCount; i++){
        if(computeNode(program->instructions[i], stack, functionMap,err) == NULL){
            return -1;
        };
        // Stop computing if there's an error
        if(err->value != ERR_SUCCESS)
            return 1;
    }
    
    //DEBUG PURPOSE / DEMO PURPOSE UNTIL WE HAVE PRINT FUNCTION

    // displayHashmap(stack,err);
    // displayFunctionMap(functionMap,err);
    return 0;
}

void displayHashmap(hmStack* stack, error* err){
    printf("----------------------MEMORY DUMP-----------------------\n");
    for(int i = 0; i < stack->length; i++){
        for(int j = 0; j < i; j++){
                printf("\t");
            }
        printf("--START OF HM LEVEL %d--\n\n",i);
        hmi iterator = hm_iterator(stack->stack[i]);
        while(hm_next(&iterator) == 1){
            for(int j = 0; j < i; j++){
                printf("\t");
            }
            // printf("%s %s : ",getVarTypeName(((var*)iterator.value)->type),iterator.key);
            char* type = getVarTypeName(((var*)iterator.value)->type);
            if(((var*)iterator.value)->type == _array){
                char* subType = getVarTypeName(((var*)iterator.value)->value._array->type);
                printf("(%s %s) %s : \n",type,subType,iterator.key);
            } else {
                printf("(%s) %s : ",type,iterator.key);
            }
            display((var*)iterator.value,err,0);
        }
        printf("\n");
        for(int j = 0; j < i; j++){
                printf("\t");
            }
        printf("--END OF HM LEVEL %d--\n",i);

    }
    printf("-------------------END OF MEMORY DUMP-------------------\n");
}

void displayFunctionMap(hm* functionMap, error* err){
    hmi iterator = hm_iterator(functionMap);
    printf("----------------------FUNCTION MAP DUMP-----------------------\n");
    while(hm_next(&iterator) == 1){
        if(((function*)iterator.value)->isBuiltin == 1){
            printf("__builtin__ ");
        }
        printf("%s : %s\n",((function*)iterator.value)->name, getVarTypeName(((function*)iterator.value)->type));
        for(int i = 0; i < ((function*)iterator.value)->parametersCount; i++){
            printf("Parameter %d: %s %s\n",i,((function*)iterator.value)->parameters[i].name, getVarTypeName(((function*)iterator.value)->parameters[i].type.type));
        }
        if(((function*)iterator.value)->instructions != NULL){
            printInstructionBlock(((function*)iterator.value)->instructions,0);
        }
        
        printf("\n");
    }
    printf("-------------------END OF FUNCTION MAP DUMP-------------------\n");
}

void debug(char key[][255], int arrSize, hmStack* stack, error *err){
    for(int i = 0; i < arrSize; i++){
        if(isInStackDownwards(stack,key[i]) > -1){
            printf("VALUE OF %s : ",key[i]);
            display((var*)hm_get(stack->stack[isInStackDownwards(stack,key[i])],key[i]), err,0);
            
        }   
    }
}