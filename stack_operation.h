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



statuses stack_ctor (int descryptor, const char* name_stack, int number_line,
                     const char* name_func, const char* name_file);

statuses stack_push (int descryptor, elem_t value);

statuses stack_pop  (int descryptor, elem_t* value);

statuses stack_dtor (int descryptor);

void inicialase_stack_descpryptor ();

void log_start ();

void log_end ();


#endif
