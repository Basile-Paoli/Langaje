#include "interpreter.h"

void subsituteValue(astNode* value, hmStack* stack){
    int hmIndex = isInStackDownwards(stack,value->value.variable);
    
    if(hmIndex == -1){
        //RAISE ERROR
        return;
    }

    var tmp = *(var*)hm_get(stack->stack[hmIndex],value->value.variable);
    value->type = VALUE;
    value->value.value = tmp;    
}

void calculate(astNode** values, astNode* node,hmStack* stack){
    operator op = node->value.operator;
    error err;

    if(values[0]->type == VARIABLE)subsituteValue(values[0],stack);    
    if(values[1]->type == VARIABLE)subsituteValue(values[1],stack);

    var var1 = values[0]->value.value;
    var var2 = values[1]->value.value;
    
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
    }
}

int assignValueToHashmap(astNode* nodeToAssign, astNode* valueToAssign, hmStack* stack){

    printf("TYPE:%d",nodeToAssign->type);
    if(nodeToAssign->type == VARIABLE){
        int hmIndex = isInStackDownwards(stack,nodeToAssign->value.variable);
        if(hmIndex > -1){
            var* tmp = (var*)hm_get(stack->stack[hmIndex],nodeToAssign->value.variable);
            var2var(tmp, &(valueToAssign->value.value));  
            printf("Changing var");
        }
    
    } else if(nodeToAssign->type == INITIALIZATION){

        int hmIndex = isInStackUpwards(stack, nodeToAssign->value.variable);
        if(hmIndex == -1){
            var* newVar = malloc(sizeof(var));
            newVar->type = nodeToAssign->value.initialization.type;    
            var2var(newVar,&(valueToAssign->value.value));
            hm_set(stack->stack[stack->length-1], nodeToAssign->value.initialization.name, newVar);
            printf("Declaring var");
        }
    }
 
}

astNode* compute(astNode* node, hmStack* stack){

    if(node->childrenCount == 0){
       
        // if(node->type == VARIABLE){
        //     int hmIndex = isInStackDownwards(stack,node->value.variable);
            
        //     if(hmIndex == -1){
        //         //RAISE ERROR
        //         return NULL;
        //     }

        //     var tmp = *(var*)hm_get(stack->stack[hmIndex],node->value.variable);
        //     node->type = VALUE;
        //     node->value.value = tmp;
        // }
        return node; //Send the whole node back
    }   

    astNode** values = malloc(sizeof(astNode*) * node->childrenCount + 1);
    for(int i = 0; i < node->childrenCount; i++){
        
        
        //IF WE ARE ON THEN
        if(i == 1 && node->type == CONDITION && values[0]->value.value.value._int == 1){
            values[i] = compute(node->children[i], stack);
        } else if(i == 2 && node->type == CONDITION && values[0]->value.value.value._int == 0){
            values[i] = compute(node->children[i], stack);
        } else {
            values[i] = compute(node->children[i], stack);

        }

        
    }

    if(node->type == OPERATOR && node->value.operator == ASSIGNMENT){
        
        assignValueToHashmap(values[0],values[1],stack);
    } else {
        
       calculate(values,node,stack);
       free(values);
       return node;
    }
}



int run(InstructionBlock* program, hmStack* stack){
    //Withotu stack memory management
    hm* hashmap = hm_create();
    hmStackPush(stack,hashmap);

    for(int i = 0; i < program->instructionsCount; i++){
        printf("Starting compute %d \n",i);
        
        compute(program->instructions[i], stack);
        printf("\nResult : ");
        display((var*)hm_get(hashmap,"a"));
        printf("\nHM len : %d\n",hm_len(hashmap));
        printf("Ending compute %d \n",i);
    }
    
    hmStackPop(stack);
    
    return 0;
}