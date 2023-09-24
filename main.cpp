#include "stack_operation.h"
#include "enumerate_statuses.h"
#include "struct_of_stack.h"
#include "consts.h"
#include <stdio.h>

#define

int main (){

    log_start ();

    struct my_stack Stack = {};

    stack_ctor(&Stack);

    for (int i = 0; i < 100; i+=2){

        stack_push(&Stack, &i);
    }

    for (int i = 0; i < 100; i+=2){

        int param = 0;

        stack_pop(&Stack, &param);

        printf("%d ", param);
    }

    printf("\n");

    stack_dtor(&Stack);

    log_end ();

    return 0;
}
