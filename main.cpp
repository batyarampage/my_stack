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

    int a = 1;

    printf("aboba1\n");

    STACK_CTOR(a);

    for (size_t i = 0; i < 100; i++){

        stack_push(a, i);
    }

    printf("aboba\n");

    for (size_t i = 0; i < 100; i++){

        long long int param = 0;

        stack_pop(a, &param);

        printf("%lld \n", param);

    }

    stack_dtor(a);

    log_end ();

    return 0;
}
