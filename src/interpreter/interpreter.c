#include "interpreter.h"


void calculate(astNode** values, astNode* node){
    operator op = node->value.operator;
    error err;
    
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

int assignValueToHashmap(astNode* nodeToAssign, astNode* valueToAssign, hm* hashmap){
    
    if(nodeToAssign->type == INITIALIZATION){

        printf("Creating new hm entry : ");
        //penser a les free apres stp bro oublie pas
        var* newVar = malloc(sizeof(var));
        newVar->type = nodeToAssign->value.initialization.type;    
        var2var(newVar,&(valueToAssign->value.value));
        
        printf("%s\n",hm_set(hashmap, nodeToAssign->value.initialization.name, newVar));
    } else if(nodeToAssign->type == VARIABLE){

        printf("Setting new hm entry : ");

        var* tmp = (var*)hm_get(hashmap,nodeToAssign->value.variable);
        var2var(tmp, &(valueToAssign->value.value));
    }
    return 0;
}

astNode* compute(astNode* node, hm* hashmap){
    printf("Current node : %d %d\n",node->type,node->childrenCount);
    if(node->childrenCount == 0){
       
        if(node->type == VARIABLE){
             
            var tmp = *(var*)hm_get(hashmap,node->value.variable);
            node->type = VALUE;
            node->value.value = tmp;
        }
        return node; //Send the whole node back
    }   

    astNode** values = malloc(sizeof(astNode*) * node->childrenCount + 1);
    for(int i = 0; i < node->childrenCount; i++){
        
        
        //IF WE ARE ON THEN
        if(i == 1 && node->type == CONDITION && values[0]->value.value.value._int == 1){
            values[i] = compute(node->children[i], hashmap);
        } else if(i == 2 && node->type == CONDITION && values[0]->value.value.value._int == 0){
            values[i] = compute(node->children[i], hashmap);
        } else {
            printf("test2 : I : %d Child amount : %d Node type : %d\n",i,node->childrenCount, node->type);
            values[i] = compute(node->children[i], hashmap);
            printf("test3\n");

        }

        
    }

    if(node->type == OPERATOR && node->value.operator == ASSIGNMENT){
        
        assignValueToHashmap(values[0],values[1],hashmap);
    } else {
        
       calculate(values,node);
       free(values);
       return node;
    }
}



int run(InstructionBlock* program){
    //Withotu stack memory management
    hm* hashmap = hm_create();

    for(int i = 0; i < program->instructionsCount; i++){
        printf("Starting compute %d \n",i);
        
        compute(program->instructions[i], hashmap);
        printf("\nResult : ");
        display((var*)hm_get(hashmap,"a"));
        printf("\nHM len : %d\n",hm_len(hashmap));
        printf("Ending compute %d \n",i);
    }
        
    hm_free(hashmap);
    
    return 0;
}