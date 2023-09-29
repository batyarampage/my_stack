#include "stack_operation.h"
#include "enumerate_statuses.h"
#include "struct_of_stack.h"
#include "consts.h"
#include "typedefine.h"
#include <stdio.h>
#include <stdlib.h>

#define STACK_CTOR(stack) stack_ctor((stack), #stack, __LINE__, __func__, __FILE__)

/*!

to use logs, you must log_start (); before initialase stack and log_end after stack_dtor;

*/


int main (){

    log_start ();

    inicialase_stack_descpryptor ();

    int a = -22;

    elem_t rez = 0;

    printf("aboba1\n");

    STACK_CTOR(a);

    stack_push(a, 'o');

    stack_push(a, 'n');

    stack_pop(a, &rez);

    printf(OUTPUT_PARAMETR, rez);

    stack_pop(a, &rez);

    printf(OUTPUT_PARAMETR, rez);

    stack_dtor(a);

    free_stack_descpryptor ();

    log_end ();

    return 0;
}
