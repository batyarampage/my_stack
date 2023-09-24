#ifndef stack_operation
#define stack_operation

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "enumerate_statuses.h"
#include "struct_of_stack.h"
#include "consts.h"
#include "typedefine.h"

extern FILE* LOG_FILE;

statuses stack_ctor (struct my_stack* Stack);

statuses stack_push (struct my_stack* Stack, Elem_T* value);

statuses stack_pop  (struct my_stack* Stack, Elem_T* value);

statuses stack_dtor (struct my_stack* Stack);

statuses_stack_ok stack_ok (struct my_stack* Stack);


#endif
