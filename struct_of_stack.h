#ifndef struct_of_stack
#define struct_of_stack

#include "consts.h"

#include "typedefine.h"

struct my_stack {

    canary_t left_canary = 0;

    elem_t* data = nullptr;

    size_t capacity = default_stack_size;

    char* name_of_stack = nullptr;

    char* name_of_func = nullptr;

    int count_of_line = 0;

    char* file = nullptr;

    size_t Size = 0;

    bool is_stack_initialase = false;

    canary_t right_canary = 0;
};


#endif
