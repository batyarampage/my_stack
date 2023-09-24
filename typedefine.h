#ifndef typedefine
#define typedefine

#include <stdio.h>
#include <stdlib.h>

typedef int Elem_T;

typedef long long int canary;

#define canary_protection

#define STACK_CTOR(Stack) stack_ctor ((Stack), #stack, __LINE__, __FUNC__, __FILE__);

#endif
