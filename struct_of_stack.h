#ifndef struct_of_stack
#define struct_of_stack

#include "consts.h"

#include "typedefine.h"

struct my_stack {

    long long int left_canary = 0;

    Elem_T* data = nullptr;

    size_t capacity = default_stack_size;

    const char* name_of_stack = nullptr;

    const char* name_of_func = nullptr;

    const int count_of_line = 0;

    const char* file = nullptr;

    size_t Size = 0;

    bool is_stack_initialase = false;

    long long int right_canary = 0;
};


#endif
