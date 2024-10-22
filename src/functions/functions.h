//
// Define base functions to add, sub, divide, ..
//

#ifndef LANGAJE_FUNCTIONS_H
#define LANGAJE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../types/types.h"
#include "../errors/errors.h"

var add(var *, var *, error *);
var substract(var *, var *, error *);
var multiply(var *, var *, error *);
var divide(var *, var *, error *);
var modulo(var *, var *, error *);
var power(var *, var *, error *);
var logicalAnd(var *, var*, error *);
var logicalOr(var *, var *, error *);
var squareroot(var *, error *);
var isEqual(var*, var*, int, error*);
var isGreater(var*, var*, int, error*);
var isLesser(var*, var*, int, error*);
int concat(var *, char *, char *, char**);
float getNumericValue(var*);
var valueOr(var*, var*, error*);
var valueAnd(var*, var*, error*);


#endif //LANGAJE_FUNCTIONS_H
