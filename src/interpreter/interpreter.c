#include "interpreter.h"

/**
* Function that replace the value of the variable in a node by its value in the hashmaps stack.
* Raise error if the value doesn't exist
*/
void subsituteValue(astNode* value, hmStack* stack){
    int hmIndex = isInStackDownwards(stack,value->value.variable);
    
    if(hmIndex == -1){
        //RAISE ERROR
        printf("Value not found : %s\n",value->value.variable);
        return;
    }

    var tmp = *(var*)hm_get(stack->stack[hmIndex],value->value.variable);
    value->type = VALUE;
    value->value.value = tmp;    
}

/**
* Function that calculate the result of a mathematical operation for two values.
* Directly replaces the value of the operation node, returns void.
*/
void calculateNode(astNode** values, astNode* node,hmStack* stack, int valuesAmount){
    operator op = node->value.operator;
    error err;

    if(values[0]->type == VARIABLE)subsituteValue(values[0],stack);    
    var var1 = values[0]->value.value;
    var var2;
    if(valuesAmount > 1){
        if(values[1]->type == VARIABLE)subsituteValue(values[1],stack);
        var2 = values[1]->value.value;
    }
    
    
    switch(op){
        case ADDITION:{
            node->value.value =  add(&var1,&var2, &err);
            break;
        }
        case SUBTRACTION:{
            node->value.value =  substract(&var1,&var2,&err);
            break;
        }
        case MULTIPLICATION:{
            node->value.value =  multiply(&var1,&var2,&err);
            break;
        }
        case DIVISION:{
            node->value.value =  divide(&var1,&var2,&err);
            break;
        }
        case EQUAL:{
            node->value.value = isEqual(&var1,&var2,0,&err);
            break;
        }
        case NOT_EQUAL:{
            node->value.value = isEqual(&var1,&var2,1,&err);
            break;
        }
        case GREATER:{
            node->value.value = isGreater(&var1,&var2,1,&err);
            break;
        }
        case GREATER_EQUAL:{
            node->value.value = isGreater(&var1,&var2,0,&err);
            break;
        }
        case LESS:{
            node->value.value = isLesser(&var1,&var2,1,&err);
            break;
        }
        case LESS_EQUAL:{
            node->value.value = isLesser(&var1,&var2,0,&err);
            break;
        }
        case OR:{
            printf("Entering or :");
            node->value.value = valueOr(&var1,&var2,&err);
            printf("%d\n",node->value.value.value._int);
            break;
        }
        case AND:{
            node->value.value = valueAnd(&var1,&var2,&err);
            break;
        }
        case NOT:{
            node->value.value = valueReverse(&var1,&err);
            break;
        }

        default:{
            printf("UNKNOWN OPERATOR\n");
            break;
        }
    }
}

/**
* Function that either : 
*   Create a new variable in the top hashmap of the stack, if the value doesn't exist in an other hashmap 
*   Change the value of a variable if it's found in the stack 
* Return 1 on success, 0 on failure.
*/
int assignValueToHashmap(astNode* nodeToAssign, astNode* valueToAssign, hmStack* stack){

    if(nodeToAssign->type == VARIABLE){
        int hmIndex = isInStackDownwards(stack,nodeToAssign->value.variable);
        printf("Assigning variable in hm : %d \n",hmIndex);
        if(hmIndex > -1){
            var* tmp = (var*)hm_get(stack->stack[hmIndex],nodeToAssign->value.variable);
            var2var(tmp, &(valueToAssign->value.value));  
            return 1;
        }
    
    } else if(nodeToAssign->type == INITIALIZATION){
        int hmIndex = isInStackUpwards(stack, nodeToAssign->value.variable);
        printf("Creating variable in hm : %d \n",hmIndex);
        if(hmIndex == -1){
            var* newVar = malloc(sizeof(var));
            newVar->type = nodeToAssign->value.initialization.type.type;    
            var2var(newVar,&(valueToAssign->value.value));
            hm_set(stack->stack[stack->length-1], nodeToAssign->value.initialization.name, newVar);
            return 1;
        }
    }
    return 0;
 
}

/**
* Function that compute a node of the AST. 
* Recursively call on each child of the node.
* Call a new runInstructionBlock if a block of code is found in the AST.
*/
astNode* computeNode(astNode* node, hmStack* stack){
    if(node->childrenCount == 0){
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
                printf("Run exit code : %d\n",runInstructionBlock(node->children[i]->value.block,stack));
            }
            i+=1;
            
        } else if(i == 2 && node->type == CONDITION && values[0]->value.value.value._int == 0 && node->children[i]->type != CONDITION){
            
            if(node->children[i]->type == BLOCK){
                
                printf("Run exit code : %d\n",runInstructionBlock(node->children[i]->value.block,stack));
            }

            i+=1;
            
        } else{
            printf("Computing node\n");
            values[i] = computeNode(node->children[i], stack);

        }
    }

    if(node->type == OPERATOR && node->value.operator == ASSIGNMENT){
        assignValueToHashmap(values[0],values[1],stack);
    } else if(node->type == OPERATOR && valuesAmount > 0){
        calculateNode(values,node,stack,valuesAmount);
        free(values);
       return node;
    }
}


/**
* Function that allocate a hashmap and push it on top of the stack
* Runs the instruction block sent in parameters.
* After the run, pop the hashmap on top of the stack.
*/
int runInstructionBlock(InstructionBlock* program, hmStack* stack){
    //Withotu stack memory management
    hm* hashmap = hm_create();
    hmStackPush(stack,hashmap);
    printf("\nRunning block\n");
    for(int i = 0; i < program->instructionsCount; i++){
        computeNode(program->instructions[i], stack);
    }
    
    //DEBUG PURPOSE / DEMO PURPOSE UNTIL WE HAVE PRINT FUNCTION
    
    char debugArr[2][255] = {"a", "b"};
    debug(&debugArr,2,stack);


    printf("Stopping block\n");
    hmStackPop(stack);
    printf("Popped hm \n");
    return 0;
}


void debug(char key[][255], int arrSize, hmStack* stack){
    for(int i = 0; i < arrSize; i++){
        if(isInStackDownwards(stack,key[i]) > -1){
            printf("VALUE OF %s : ",key[i]);
            display((var*)hm_get(stack->stack[isInStackDownwards(stack,key[i])],key[i]));
            printf("\n");
        }   
    }
}