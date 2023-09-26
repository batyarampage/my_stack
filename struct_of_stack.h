#ifndef struct_of_stack
#define struct_of_stack

#include "consts.h"

#include "typedefine.h"

struct my_stack {

    canary_t left_canary = 0;

    size_t capacity = DEFAULT_STACK_SIZE;

    const char* name_of_stack = nullptr;

    const char* name_of_func = nullptr;

    elem_t* data = nullptr;

    unsigned int hash_stack = 0;
    unsigned int hash_data  = 0;

    int count_of_line = 0;

    const char* file = nullptr;

    size_t Size = 0;

    enum conddition_stack status = NOT_INICIALED;

    canary_t right_canary = 0;
};

struct hashes {

    unsigned int hash_stack = 0;
    unsigned int hash_data  = 0;
};


#endif
