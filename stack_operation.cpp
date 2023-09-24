#include "stack_operation.h"
#include "enumerate_statuses.h"
#include "struct_of_stack.h"
#include "typedefine.h"

#define STACK_DUMP(stack) stack_dump((stack), __FILE__, __LINE__, __FUNC__)

#define STACK_OK(stack) if (stack_ok(stack) != SUCCESS_STACK) { STACK_DUMP(stack); return ERROR;}

statuses stack_ctor (struct my_stack* Stack, const char* name_stack, const int number_line,
                     const char* name_func, const char* name_file){

    assert(name_stack != nullptr);

    if (Stack->is_stack_initialase){

        return STACK_WAS_INICIALASED_BEFORE;
    }

    #ifdef canary

    elem_t* data_of_Stack = (elem_t*) calloc(capacity * sizeof(elem_t) + 2 * sizeof(canary_t),1);

    if (data_of_Stack == nullptr){

        return NO_MEMORY;
    }

    canary_t* left_data_canary  = (canary_t*)((char*) data);
    canary_t* right_data_canary = (canary_t*)((char*) data + Stack->capacity * sizeof(elem_t) + sizeof(canary_t));

    set_canaries (left_data_canary, right_data_canary);

    set_canaries (&(Stack->left_canary), &(Stack->right_canary));

    elem_t* first_data = (elem_t*)((char*) data + sizeof(canary_t));

    Stack->data = first_data;

    #else

    elem_t* data_of_Stack = (elem_t*) calloc(default_stack_size, sizeof(elem_t));

    if (data_of_Stack == nullptr){

        return NO_MEMORY;
    }

    Stack->data = data_of_Stack;

    #endif


    Stack->Size                = 0;
    Stack->capacity            = default_stack_size;
    Stack->is_stack_initialase = true;
    Stack->name_of_stack       = name_stack;
    Stack->name_of_func        = name_func;
    Stack->count_of_line       = number_line;
    Stack->file                = name_file;

    STACK_OK (Stack);

    return SUCCESS;
}

statuses stack_push (struct my_stack* Stack, elem_t* value){

    STACK_OK (Stack);

    if (value == nullptr){

        return ERROR;
    }

    if (Stack->Size == Stack->capacity - 1){


        #ifdef canary

        Stack->capacity *= 2;

        elem_t* data1 = (elem_t*) realloc(Stack->data-sizeof(canary_t), Stack->capacity * (sizeof(elem_t) + sizeof(canary_t)*2));

        clean_right_data (data1);

        Stack->data = (elem_t*) ((char*) data1 + Stack->capacity * sizeof(elem_t));

        canary_t* right_data_canary = (canary_t*)((char*) Stack->data + Stack->capacity * sizeof(elem_t));

        *right_data_canary = DEFAULT_CANARY_VALUE;

        #else

        Stack->data = (elem_t*) realloc(Stack->data, Stack->capacity * sizeof(elem_t));

        #endif

        if (Stack->data == nullptr){

            return NO_MEMORY;
        }
    }

    *(Stack->data + Stack->Size) = *value;

    Stack->Size++;

    STACK_OK (Stack);

    return SUCCESS;
}

statuses stack_pop (struct my_stack* Stack, elem_t* value){

    STACK_OK (Stack);

    if (value == nullptr){

        return ERROR;
    }

    (Stack->Size)--;

    *value = *(Stack->data + Stack->Size);

    STACK_OK (Stack);

    return SUCCESS;
}

statuses stack_dtor (struct my_stack* Stack){

    STACK_OK (Stack);

    if (Stack == nullptr){

        return ERROR;
    }

    free(Stack->data);
    Stack->data = nullptr;
    Stack->is_stack_initialase = false;
    Stack->capacity = default_stack_size;
    Stack->Size = 0;

    STACK_OK (Stack);

    return SUCCESS;
}


void set_canaries (canary_t* left_canary, canary_t* right_canary){

    assert(left_canary  != nullptr);
    assert(right_canary != nullptr);

    *left_canary  = DEFAULT_CANARY_VALUE;
    *right_canary = DEFAULT_CANARY_VALUE;
}

statuses_stack_ok stack_ok (struct my_stack* Stack){

    if (Stack == nullptr){

        return INCORRECT_STACK;
    }

    if (Stack->data == nullptr){

        return INCORRECT_DATA;
    }

    if ((Stack->capacity) < (Stack->Size)){

        return INCORRECT_DIFF_SIZE_CAPACITY;
    }

    if ((Stack->Size) < 0){

        return INCORRECT_SIZE;
    }

    if ((Stack->capacity) < 0){

        return INCORRECT_CAPACITY;
    }

    #ifdef canary

    if (((Stack->left_canary) != DEFAULT_CANARY_VALUE) || ((Stack->right_canary) != DEFAULT_CANARY_VALUE)){

        return INCORRECT_CANARY;
    }

    canary_t* left_data_canary  = (canary_t*)((char*) stk->data);
    canary_t* right_data_canary = (canary_t*)((char*) stk->data + stk->capacity * sizeof(elem_t) + sizeof(canary_t));

    if (((*left_data_canary) != DEFAULT_CANARY_VALUE) || ((*right_data_canary) != DEFAULT_CANARY_VALUE)){

        return INCORRECT_CANARY;
    }

    #endif

    return SUCCESS_STACK;
}


statuses stack_dump (struct my_stack* Stack, const char* curr_file, const int curr_line, const char* curr_func){

    if (Stack == nullptr){

        fprintf(LOG_FILE, "ERROR, pointer on Stack is nullptr\n");
    }

    else {

        fprintf(LOG_FILE, "stack[%p] %s from %s (%d) %s\n", Stack, Stack->name_of_stack, Stack->file, Stack->count_of_line,
                Stack->name_of_func);

        fprintf(LOG_FILE, "{\n");

        fprintf(LOG_FILE, "called from %s (%d) %s", curr_file, curr_line, curr_func);

        #ifdef canary

        fprintf(LOG_FILE, "left stack canary: %lld \n \
                           right stack canary: %lld \n\n \
                           left data canary: %lld \n \
                           right data canary: %lld \n\n", Stack->left_canary, Stack->right_canary,
                           (canary_t*)((char*) stk->data), (canary_t*)((char*) stk->data + stk->capacity * sizeof(elem_t) + sizeof(canary_t)));
        #endif

        fprintf(LOG_FILE, "size = %u\n \
                           capacity = %u\n \
                           data[%p]\n{\n", Stack->Size, Stack->capacity, Stack->data);

        ifdef canary

        for (size_t i = 0; i < Stack->Size; i++){

            fprintf("*[%u] = %d", i, *(Stack->data + i));
        }

    }

    return SUCCESS;
}

void clean_right_data (elem_t* data1){


}


