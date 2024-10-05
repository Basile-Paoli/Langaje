//
// Define base functions to add, sub, divide, ..
//

#ifndef LANGAJE_FUNCTIONS_H
#define LANGAJE_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

var add(var *, var *);
int concat(var *, char *, char *, char**);
float getNumericValue(var*);


#endif //LANGAJE_FUNCTIONS_H
