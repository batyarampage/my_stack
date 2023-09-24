#include "stack_operation.h"
#include "enumerate_statuses.h"
#include "struct_of_stack.h"
#include "consts.h"
#include "logs.h"
#include "typedefine.h"
#include <stdio.h>
#include <stdlib.h>


#define STACK_CTOR(stack) stack_ctor((stack), #stack, __LINE__, __func__, __FILE__)

int main (){

    log_start ();

    struct my_stack Stack = {};

    STACK_CTOR(&Stack);

    printf("ok0\n");

    for (int i = 0; i < 100; i+=10){

        stack_push(&Stack, &i);
    }

    for (int i = 0; i < 100; i+=10){

        int param = 0;

        stack_pop(&Stack, &param);

        printf("%d ", param);
    }

    printf("\n");

    stack_dtor(&Stack);

    log_end ();

    return 0;
}
