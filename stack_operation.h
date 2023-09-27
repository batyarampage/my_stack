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

statuses stack_ctor (struct my_stack* Stack, const char* name_stack1, int number_line,
                     const char* name_func1, const char* name_file1);

statuses stack_push (struct my_stack* Stack, elem_t value);

statuses stack_pop  (struct my_stack* Stack, elem_t* value);

statuses stack_dtor (struct my_stack* Stack);


#endif
