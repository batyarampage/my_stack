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

    inicialase_stack_descpryptor ();

    printf("aboba1\n");

    /*STACK_CTOR(&Stack);

    for (size_t i = 0; i < 100; i++){

        stack_push(&Stack, i);
    }

    printf("aboba\n");

    for (size_t i = 0; i < 100; i++){

        long long int param = 0;

        stack_pop(&Stack, &param);

        printf("%lld rez\n", param);

    }

    stack_dtor(&Stack);*/

    log_end ();

    return 0;
}
