#ifndef struct_of_stack
#define struct_of_stack

#include "consts.h"

#include "typedefine.h"

struct my_stack {

    #ifdef CANARY

    canary_t left_canary;

    #endif

    size_t capacity;

    const char* name_of_stack;

    const char* name_of_func;

    elem_t* data;

    int count_of_line;

    const char* file;

    size_t Size;

    enum conddition_stack status;

    #ifdef CANARY

    canary_t right_canary;

    #endif
};

struct hashes {

    unsigned int hash_stack;
    unsigned int hash_data;
};


#endif
