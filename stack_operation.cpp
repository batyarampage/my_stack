#include "stack_operation.h"
#include "enumerate_statuses.h"
#include "struct_of_stack.h"
#include "typedefine.h"
#include "consts.h"

#define STACK_DUMP(stack) stack_dump((stack), __FILE__, __LINE__, __func__)
#define STACK_OK(stack) if (stack_ok(stack) != SUCCESS_STACK) { STACK_DUMP(stack);}


static void set_canaries (canary_t* left_canary, canary_t* right_canary);

static statuses_stack_ok stack_ok (struct my_stack* Stack);

static statuses stack_dump (struct my_stack* Stack, const char* curr_file, const int curr_line, const char* curr_func);

static void clean_right_data (struct my_stack* Stack);

static void rehash_stack_and_data (struct my_stack* Stack);

static unsigned int hash_djb2 (const char* hashable, size_t size_hashable);

static statuses set_default_data_poizon (struct my_stack* Stack);

static statuses stack_reallocation (struct my_stack* Stack, const int parametr);


static hashes main_hashes = {};


statuses stack_ctor (struct my_stack* Stack, const char* name_stack, int number_line,
                     const char* name_func, const char* name_file){


    if (Stack->status == IS_INITIALIZED){

        return STACK_WAS_INITIALIZED_BEFORE;
    }

    #ifdef CANARY

    elem_t* data_of_Stack = (elem_t*) calloc(DEFAULT_STACK_SIZE * sizeof(elem_t) + 2 * sizeof(canary_t),1);

    if (data_of_Stack == nullptr){

        return NO_MEMORY;
    }

    canary_t* left_data_canary  = (canary_t*)(data_of_Stack);
    canary_t* right_data_canary = (canary_t*)((char*) data_of_Stack + DEFAULT_STACK_SIZE * sizeof(elem_t) + sizeof(canary_t));

    set_canaries (left_data_canary, right_data_canary);

    set_canaries (&(Stack->left_canary), &(Stack->right_canary));

    elem_t* first_data = (elem_t*)((char*) data_of_Stack + sizeof(canary_t));

    Stack->data = first_data;

    #else

    elem_t* data_of_Stack = (elem_t*) calloc(DEFAULT_STACK_SIZE*sizeof(elem_t), 1);

    if (data_of_Stack == nullptr){

        return NO_MEMORY;
    }

    Stack->data = data_of_Stack;

    #endif

    set_default_data_poizon (Stack);

    Stack->Size           = 0;
    Stack->capacity       = DEFAULT_STACK_SIZE;
    Stack->status         = IS_INITIALIZED;
    Stack->name_of_stack  = name_stack;
    Stack->name_of_func   = name_func;
    Stack->count_of_line  = number_line;
    Stack->file           = name_file;

    #ifdef HASH_PROTECTION

    rehash_stack_and_data (Stack);

    #endif

    STACK_OK (Stack);

    return SUCCESS;
}


statuses stack_push (struct my_stack* Stack, elem_t value){

    STACK_OK (Stack);

    if (Stack->Size == Stack->capacity - 1){

        stack_reallocation(Stack, UP);
    }

    *(Stack->data + Stack->Size) = value;

    (Stack->Size)++;

    #ifdef HASH_PROTECTION

    rehash_stack_and_data (Stack);

    #endif

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

    *(Stack->data + Stack->Size) = POIZON_VALUE;

    if ((((Stack->capacity)/4) > Stack->Size) && (Stack->Size != 0)){

        stack_reallocation(Stack, DOWN);
    }

    #ifdef HASH_PROTECTION

    rehash_stack_and_data (Stack);

    #endif

    STACK_OK (Stack);

    return SUCCESS;
}


statuses stack_dtor (struct my_stack* Stack){

    STACK_OK (Stack);

    if (Stack == nullptr){

        return ERROR;
    }

    #ifdef CANARY

    free((canary_t *)Stack->data - 1);

    #else

    free(Stack->data);

    #endif

    Stack->data     = nullptr;
    Stack->capacity = DEFAULT_STACK_SIZE;
    Stack->Size     = 0;
    Stack->status   = DESTRUCTED;

    return SUCCESS;
}


static void set_canaries (canary_t* left_canary, canary_t* right_canary){

    assert(left_canary  != nullptr);
    assert(right_canary != nullptr);

    *left_canary  = DEFAULT_CANARY_VALUE;
    *right_canary = DEFAULT_CANARY_VALUE;
}


static statuses_stack_ok stack_ok (struct my_stack* Stack){

    if (Stack == nullptr){

        return INCORRECT_STACK;
    }

    if (Stack->data == nullptr){

        return INCORRECT_DATA;
    }

    if ((Stack->capacity) < (Stack->Size)){

        return INCORRECT_DIFF_SIZE_CAPACITY;
    }

    if ((Stack->capacity) == 0){

        return INCORRECT_CAPACITY;
    }

    #ifdef CANARY

    if (((Stack->left_canary) != DEFAULT_CANARY_VALUE) || ((Stack->right_canary) != DEFAULT_CANARY_VALUE)){

        return INCORRECT_CANARY;
    }

    canary_t* left_data_canary  = (canary_t*)(Stack->data - 1);
    canary_t* right_data_canary = (canary_t*)(Stack->data + Stack->capacity);

    if (((*left_data_canary) != DEFAULT_CANARY_VALUE) || ((*right_data_canary) != DEFAULT_CANARY_VALUE)){

        return INCORRECT_CANARY;
    }

    #endif

    unsigned int hash_stack_now = hash_djb2((char*) Stack, sizeof(*Stack));

    if (main_hashes.hash_stack == hash_stack_now){

        return INCORRECT_HASH;
    }

    unsigned int hash_data_now = hash_djb2((char*) Stack->data-sizeof(canary_t), sizeof(canary_t)*2+Stack->capacity*(sizeof(elem_t)));

    if (hash_data_now == main_hashes.hash_data){

        return INCORRECT_HASH;
    }

    return SUCCESS_STACK;
}


static statuses stack_dump (struct my_stack* Stack, const char* curr_file, const int curr_line, const char* curr_func){

    if (Stack == nullptr){

        fprintf(LOG_FILE, "ERROR, pointer on Stack is nullptr\n");
    }

    else {

        fprintf(LOG_FILE, "stack[%p] %s from %s (%d) %s\n", Stack, Stack->name_of_stack, Stack->file, Stack->count_of_line,
                Stack->name_of_func);

        fprintf(LOG_FILE, "called from %s (%d) %s\n{\n", curr_file, curr_line, curr_func);

        #ifdef CANARY

        fprintf(LOG_FILE, "\tleft stack canary: %lld \n \tright stack canary: %lld \n\n \tleft data canary: %lld \n \tright data canary: %lld \n\n",
                Stack->left_canary, Stack->right_canary,*(canary_t*)((char*) Stack->data - sizeof(canary_t)),
                *(canary_t*)((char*)Stack->data + Stack->capacity*sizeof(elem_t)));
        #endif

        fprintf(LOG_FILE, "\tsize = %u\n \tcapacity = %u\n \tdata[%p]\n\n",
                            Stack->Size, Stack->capacity, Stack->data);

        #ifdef HASH_PROTECTION

        fprintf(LOG_FILE, "\tStack_hash = %u\n \tData_hash  = %u \n\n",
                            main_hashes.hash_stack, main_hashes.hash_data);

        #endif

        fprintf(LOG_FILE, "\t{\n");

        for (size_t i = 0; i < Stack->Size; i++){

            fprintf(LOG_FILE,"\t\t*[%lu] = " OUTPUT_PARAMETR " \n", i, *(Stack->data + i));
        }

        for (size_t i = Stack->Size; i < Stack->capacity; i++){

            fprintf(LOG_FILE," \t\t [%lu] = " OUTPUT_PARAMETR " (POISON)\n", i, *(Stack->data + i));
        }

        fprintf(LOG_FILE, "\t}\n");

        fprintf(LOG_FILE, "}\n\n\n\n");
    }

    return SUCCESS;
}


static void clean_right_data (struct my_stack* Stack){

    for (size_t i = (Stack->capacity)/2; i < Stack->capacity; i++){

        Stack->data[i] = POIZON_VALUE;
    }
}


static void rehash_stack_and_data (struct my_stack* Stack){

    assert(Stack != nullptr);

    main_hashes.hash_data = hash_djb2((char*) Stack->data-sizeof(canary_t), sizeof(canary_t)*2+Stack->capacity*(sizeof(elem_t)));

    main_hashes.hash_stack = hash_djb2((char*) Stack, sizeof(*Stack));
}


static unsigned int hash_djb2 (const char* hashable, size_t size_hashable){

    unsigned int hash_t = 5381;

    for (size_t i = 0; i < size_hashable; i++){

        int c = 0;

        c = hashable[i++];

        hash_t = ((hash_t << 5) + hash_t) + c;
    }

    return hash_t % (int) 1e9;
}


static statuses set_default_data_poizon (struct my_stack* Stack){

    for (size_t i = 0; i < DEFAULT_STACK_SIZE; i++){

        Stack->data[i] = POIZON_VALUE;
    }

    return SUCCESS;
}

static statuses stack_reallocation (struct my_stack* Stack, const int parametr){

    if (parametr){

        Stack->capacity *= RE_SIZING;

        #ifdef CANARY

        elem_t* data1 = (elem_t*) realloc((char*) Stack->data - sizeof(canary_t), Stack->capacity * sizeof(elem_t) + sizeof(canary_t)*2);

        if (data1 == nullptr){

            return NO_MEMORY;
        }

        Stack->data = (elem_t*) ((char*) data1 + sizeof(canary_t));

        *(canary_t*)((char*) Stack->data + Stack->capacity * sizeof(elem_t)) = DEFAULT_CANARY_VALUE;

        #else

        elem_t* data_backup = (elem_t*) realloc(Stack->data, Stack->capacity * sizeof(elem_t));

        if (data_backup == nullptr){

            return NO_MEMORY;
        }

        Stack->data = data_backup;

        #endif

        clean_right_data (Stack);

        return SUCCESS;
    }

    else {

        #ifdef CANARY

        elem_t* data1 = (elem_t*) realloc((char*) Stack->data - sizeof(canary_t), ((Stack->capacity)/4) * sizeof(elem_t) + sizeof(canary_t)*2);

        if (data1 == nullptr){

            return NO_MEMORY;
        }

        Stack->data = (elem_t*) ((char*) data1 + sizeof(canary_t));

        *(canary_t*)((char*) Stack->data + Stack->capacity * sizeof(elem_t)) = DEFAULT_CANARY_VALUE;

        #else


        elem_t* data_backup = (elem_t*) realloc(Stack->data, ((Stack->capacity)/4) * sizeof(elem_t));

        if (data_backup == nullptr){

            return NO_MEMORY;
        }

        Stack->data = data_backup;

        #endif

        Stack->capacity = (Stack->capacity)/4;

        return SUCCESS;
    }
}
