#include "stack_operation.h"
#include "enumerate_statuses.h"
#include "struct_of_stack.h"
#include "consts.h"
#include "logs.h"
#include "typedefine.h"
#include <stdio.h>
#include <stdlib.h>

#define STACK_CTOR(stack) stack_ctor((stack), #stack, __LINE__, __func__, __FILE__)

/*!

to use logs, you must log_start (); before initialase stack and log_end after stack_dtor;

*/


int main (){

    log_start ();

    my_stack Stack = {};

    STACK_CTOR(&Stack);



    stack_dtor(&Stack);

    log_end ();

    return 0;
}
