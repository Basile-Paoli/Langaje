#include "interpreter.h"


var calculate(var* values, astNode* node){
    operator op = node->value.operator;
    error err;
    switch(op){
        case ADDITION:{
            return add(&values[0],&values[1], &err);
        }
        case SUBTRACTION:{
            return substract(&values[0],&values[1],&err);
        }
        case MULTIPLICATION:{
            return multiply(&values[0],&values[1],&err);
        }
        case DIVISION:{
            return divide(&values[0],&values[1],&err);
        }
    }
    
}


var compute(astNode* node){
    if(node->childrenCount == 0){
        return node->value.value; //Send the whole var back
    }   
    var values[3];
    short valueAmount = 0;
    for(int i = 0; i < node->childrenCount; i++){
        values[valueAmount] = compute(node->children[i]);
        valueAmount++;
    }
    return calculate(values,node);
}




int run(InstructionBlock* program){

    for(int i = 0; i < program->instructionsCount; i++){
        printf("Starting compute %d \n",i);
        var test = compute(program->instructions[i]);
        printf("Result : ");
        display(&test);
        printf("Ending compute %d \n",i);
    }
        
    return 0;
}