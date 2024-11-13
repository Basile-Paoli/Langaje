#include "__builtins__.h"


void __builtinToMap__(hm* functionMap, error* err){
    function* __example__ = declare__example(err);
    hm_set(functionMap, __example__->name, __example__);
}


function* declare__example(error* err){
    fakeFunctionParam zz = {"a",_int};
    fakeFunctionParam zzzz = {"b",_int};
    fakeFunctionParam* p = malloc(sizeof(fakeFunctionParam) * 2);
    p[0] = zz;
    p[1] = zzzz;
    return newFunctionPrototype("example",_void,p,__print__,2,err);
}

