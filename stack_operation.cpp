#include "stack_operation.h"
#include "enumerate_statuses.h"
#include "struct_of_stack.h"
#include "typedefine.h"
#include "consts.h"
#include <stdio.h>

#define STACK_DUMP(stack) stack_dump((stack), __FILE__, __LINE__, __func__)
#define STACK_OK(stack) if (stack_ok(stack) != SUCCESS_STACK) { STACK_DUMP(stack);}


static void set_canaries (canary_t* left_canary, canary_t* right_canary);

static statuses_stack_ok stack_ok (int descryptor);

static statuses stack_dump (int descryptor, const char* curr_file, const int curr_line, const char* curr_func);

static void clean_right_data (int descryptor);

static void rehash_stack_and_data (int descryptor);

static unsigned int hash_djb2 (const char* hashable, size_t size_hashable);

static statuses set_default_data_poizon (int descryptor);

static statuses stack_reallocation (int descryptor, const int parametr);

static void inicialase_descryptor_array ();

static void inicialse_stack_array ();

static void resize_stack_descpryptor ();

static void inicialse_array_of_hashes ();

static void open_log_file ();

static void close_log_file ();




static int lenght_of_array = 2;

static const int DEDAULT_VALUE_DESCRYPTOR_ARRAY = 900090009;

static int count_of_active_stack = 0;

static my_stack* array_of_stack = nullptr;

static int* array_of_descryptor = nullptr;

static hashes* array_of_hashes = nullptr;

static FILE* LOG_FILE = nullptr;




static void open_log_file (){

    LOG_FILE = fopen("logs.txt", "a+");

    assert(LOG_FILE != nullptr);

    fprintf(LOG_FILE, "logs is begun\n\n");
}

static void close_log_file (){

    fprintf(LOG_FILE, "logs was end\n");

    fclose(LOG_FILE);
}

void log_start (){

    open_log_file ();
}

void log_end (){

    close_log_file ();
}

static void inicialase_descryptor_array (){

    array_of_descryptor = (int*) calloc(lenght_of_array, sizeof(int));
}

static void inicialse_stack_array (){

    array_of_stack = (my_stack*) calloc(lenght_of_array, sizeof(my_stack));
}

static void inicialse_array_of_hashes (){

    array_of_hashes = (hashes*) calloc(lenght_of_array, sizeof(hashes));
}


void inicialase_stack_descpryptor (){//TODO проверки

    inicialase_descryptor_array ();
    inicialse_stack_array ();
    inicialse_array_of_hashes ();

    for (int i = 0; i < lenght_of_array; i++){

        array_of_descryptor[i] = DEDAULT_VALUE_DESCRYPTOR_ARRAY;
    }
}


static void resize_stack_descpryptor (){

    #ifdef INCREMENT_DYNAMIC

    lenght_of_array++;

    #else

    lenght_of_array *= 2;

    #endif

    array_of_descryptor = (int*) realloc(array_of_descryptor, sizeof(int)*lenght_of_array);

    array_of_stack = (my_stack*) realloc(array_of_stack, sizeof(my_stack)*lenght_of_array);

    array_of_hashes = (hashes*)  realloc(array_of_hashes,  sizeof(hashes)*lenght_of_array);

    for (int i = lenght_of_array/2; i < lenght_of_array; i++){

        array_of_descryptor[i] = DEDAULT_VALUE_DESCRYPTOR_ARRAY;
    }
}

static bool descryptor_in_array (int* index_stack, int descryptor){

    for (int i = 0; i < lenght_of_array; i++){

        if (array_of_descryptor[i] == descryptor){

            *index_stack = i;

            return true;
        }
    }

    return false;
}

statuses stack_ctor (int descryptor, const char* name_stack, int number_line,
                     const char* name_func, const char* name_file){

    int index_stack = 0;

    if (descryptor_in_array(&index_stack, descryptor)){

        return STACK_WAS_INITIALIZED_BEFORE;
    }

    count_of_active_stack++;

    if (count_of_active_stack > lenght_of_array){

        resize_stack_descpryptor ();
    }

    #ifdef CANARY

    elem_t* data_of_Stack = (elem_t*) calloc(DEFAULT_STACK_SIZE * sizeof(elem_t) + 2 * sizeof(canary_t),1);

    if (data_of_Stack == nullptr){

        return NO_MEMORY;
    }

    canary_t* left_data_canary  = (canary_t*)(data_of_Stack);
    canary_t* right_data_canary = (canary_t*)((char*) data_of_Stack + DEFAULT_STACK_SIZE * sizeof(elem_t) + sizeof(canary_t));

    set_canaries (left_data_canary, right_data_canary);

    set_canaries (&((array_of_stack[index_stack]).left_canary), &((array_of_stack[index_stack]).right_canary));

    elem_t* first_data = (elem_t*)((char*) data_of_Stack + sizeof(canary_t));

    (array_of_stack[index_stack]).data = first_data;

    #else

    elem_t* data_of_Stack = (elem_t*) calloc(DEFAULT_STACK_SIZE*sizeof(elem_t), 1);

    if (data_of_Stack == nullptr){

        return NO_MEMORY;
    }

    (array_of_stack[index_stack]).data = data_of_Stack;

    #endif

    set_default_data_poizon (index_stack);

    array_of_descryptor[index_stack]             = descryptor;

    (array_of_stack[index_stack]).Size           = 0;
    (array_of_stack[index_stack]).capacity       = DEFAULT_STACK_SIZE;
    (array_of_stack[index_stack]).status         = IS_INITIALIZED;
    (array_of_stack[index_stack]).name_of_stack  = name_stack;
    (array_of_stack[index_stack]).name_of_func   = name_func;
    (array_of_stack[index_stack]).count_of_line  = number_line;
    (array_of_stack[index_stack]).file           = name_file;

    #ifdef HASH_PROTECTION

    rehash_stack_and_data (index_stack);

    #endif

    STACK_OK (index_stack);

    return SUCCESS;
}


statuses stack_push (int descryptor, elem_t value){

    int index_stack = 0;

    if (!(descryptor_in_array(&index_stack, descryptor))){

        return NOT_DESCRYPTOR_EXISTS;
    }

    STACK_OK (index_stack);

    if ((array_of_stack[index_stack]).Size == (array_of_stack[index_stack]).capacity - 1){

        stack_reallocation(index_stack, UP);
    }

    (array_of_stack[index_stack]).data[(array_of_stack[index_stack]).Size] = value;

    ((array_of_stack[index_stack]).Size)++;

    #ifdef HASH_PROTECTION

    rehash_stack_and_data (index_stack);

    #endif

    STACK_OK (index_stack);

    return SUCCESS;
}


statuses stack_pop (int descryptor, elem_t* value){

    int index_stack = 0;

    if (!(descryptor_in_array(&index_stack, descryptor))){

        return NOT_DESCRYPTOR_EXISTS;
    }

    STACK_OK (index_stack);

    if (((array_of_stack[index_stack]).Size) >= 1){

        ((array_of_stack[index_stack]).Size)--;

        *value = *((array_of_stack[index_stack]).data + (array_of_stack[index_stack]).Size);

        *((array_of_stack[index_stack]).data + (array_of_stack[index_stack]).Size) = POIZON_VALUE;
    }

    else {

        return ERROR;
    }

    if (((((array_of_stack[index_stack]).capacity)/4) > (array_of_stack[index_stack]).Size) && ((array_of_stack[index_stack]).Size != 0)){

        stack_reallocation(index_stack, DOWN);
    }

    #ifdef HASH_PROTECTION

    rehash_stack_and_data (index_stack);

    #endif

    STACK_OK (index_stack);

    return SUCCESS;
}


statuses stack_dtor (int descryptor){

    int index_stack = 0;

    if (!(descryptor_in_array(&index_stack, descryptor))){

        return NOT_DESCRYPTOR_EXISTS;
    }

    STACK_OK (index_stack);

    #ifdef CANARY

    free((canary_t *)(array_of_stack[index_stack]).data - 1);

    #else

    free((array_of_stack[index_stack]).data);

    #endif

    (array_of_stack[index_stack]).data     = nullptr;
    (array_of_stack[index_stack]).capacity = DEFAULT_STACK_SIZE;
    (array_of_stack[index_stack]).Size     = 0;
    (array_of_stack[index_stack]).status   = DESTRUCTED;

    count_of_active_stack--;

    return SUCCESS;
}


static void set_canaries (canary_t* left_canary, canary_t* right_canary){

    assert(left_canary  != nullptr);
    assert(right_canary != nullptr);

    *left_canary  = DEFAULT_CANARY_VALUE;
    *right_canary = DEFAULT_CANARY_VALUE;
}


static statuses_stack_ok stack_ok (int descryptor){

    if ((array_of_stack[descryptor]).data == nullptr){

        return INCORRECT_DATA;
    }

    if (((array_of_stack[descryptor]).capacity) < ((array_of_stack[descryptor]).Size)){

        return INCORRECT_DIFF_SIZE_CAPACITY;
    }

    if (((array_of_stack[descryptor]).capacity) == 0){

        return INCORRECT_CAPACITY;
    }

    #ifdef CANARY

    if ((((array_of_stack[descryptor]).left_canary) != DEFAULT_CANARY_VALUE) || (((array_of_stack[descryptor]).right_canary) != DEFAULT_CANARY_VALUE)){

        return INCORRECT_CANARY;
    }

    canary_t* left_data_canary  = (canary_t*)((array_of_stack[descryptor]).data - 1);
    canary_t* right_data_canary = (canary_t*)((array_of_stack[descryptor]).data + (array_of_stack[descryptor]).capacity);//TODO внимание

    assert(left_data_canary  != nullptr);
    assert(right_data_canary != nullptr);

    if (((*left_data_canary) != DEFAULT_CANARY_VALUE) && ((*right_data_canary) != DEFAULT_CANARY_VALUE)){

        return INCORRECT_CANARY;
    }

    #endif

    #ifdef HASH_PROTECTION

    unsigned int hash_stack_now = hash_djb2((char*) &(array_of_stack[descryptor]), sizeof(my_stack));

    if ((array_of_hashes[descryptor]).hash_stack != hash_stack_now){

        return INCORRECT_HASH;
    }

    unsigned int hash_data_now = hash_djb2((char*) (array_of_stack[descryptor]).data-sizeof(canary_t), sizeof(canary_t)*2+(array_of_stack[descryptor]).capacity*(sizeof(elem_t)));

    if (hash_data_now == (array_of_hashes[descryptor]).hash_data){

        return INCORRECT_HASH;
    }

    #endif

    return SUCCESS_STACK;
}


static statuses stack_dump (int descryptor, const char* curr_file, const int curr_line, const char* curr_func){

    assert(curr_file != nullptr);
    assert(curr_func != nullptr);

    fprintf(LOG_FILE, "stack %s from %s (%d) %s\n", (array_of_stack[descryptor]).name_of_stack, (array_of_stack[descryptor]).file, (array_of_stack[descryptor]).count_of_line,
            (array_of_stack[descryptor]).name_of_func);

    fprintf(LOG_FILE, "called from %s (%d) %s\n{\n", curr_file, curr_line, curr_func);

    #ifdef CANARY

    fprintf(LOG_FILE, "\tleft stack canary: %lld \n \tright stack canary: %lld \n\n \tleft data canary: %lld \n \tright data canary: %lld \n\n",
            (array_of_stack[descryptor]).left_canary, (array_of_stack[descryptor]).right_canary,*(canary_t*)((char*) (array_of_stack[descryptor]).data - sizeof(canary_t)),
            *(canary_t*)((char*)(array_of_stack[descryptor]).data + (array_of_stack[descryptor]).capacity*sizeof(elem_t)));
    #endif

    fprintf(LOG_FILE, "\tsize = %u\n \tcapacity = %u\n \tdata[%p]\n\n",
                        (array_of_stack[descryptor]).Size, (array_of_stack[descryptor]).capacity, (array_of_stack[descryptor]).data);

    #ifdef HASH_PROTECTION

    fprintf(LOG_FILE, "\tStack_hash = %u\n \tData_hash  = %u \n\n",
                        (array_of_hashes[descryptor]).hash_stack, (array_of_hashes[descryptor]).hash_data);

    #endif

    fprintf(LOG_FILE, "\t{\n");

    for (size_t i = 0; i < (array_of_stack[descryptor]).Size; i++){

        fprintf(LOG_FILE,"\t\t*[%lu] = " OUTPUT_PARAMETR " \n", i, (array_of_stack[descryptor]).data[i]);
    }

    for (size_t i = (array_of_stack[descryptor]).Size; i < (array_of_stack[descryptor]).capacity; i++){

        fprintf(LOG_FILE," \t\t [%lu] = " OUTPUT_PARAMETR " (POISON)\n", i, (array_of_stack[descryptor]).data[i]);
    }

    fprintf(LOG_FILE, "\t}\n");

    fprintf(LOG_FILE, "}\n\n\n\n");


    return SUCCESS;
}


static void clean_right_data (int descryptor){

    for (size_t i = ((array_of_stack[descryptor]).capacity)/2; i < (array_of_stack[descryptor]).capacity; i++){

        (array_of_stack[descryptor]).data[i] = POIZON_VALUE;
    }
}


static void rehash_stack_and_data (int descryptor){

    (array_of_hashes[descryptor]).hash_data  = hash_djb2((char*) (array_of_stack[descryptor]).data-sizeof(canary_t), sizeof(canary_t)*2+(array_of_stack[descryptor]).capacity*(sizeof(elem_t)));

    (array_of_hashes[descryptor]).hash_stack = hash_djb2((char*) &(array_of_stack[descryptor]), sizeof(my_stack));
}


static unsigned int hash_djb2 (const char* hashable, size_t size_hashable){

    assert(hashable != nullptr);

    unsigned int hash_t = 5381;

    for (size_t i = 0; i < size_hashable; i++){

        int c = 0;

        c = hashable[i++];

        hash_t = ((hash_t << 5) + hash_t) + c;
    }

    return hash_t % (int) 1e9;
}


static statuses set_default_data_poizon (int descryptor){

    for (size_t i = 0; i < DEFAULT_STACK_SIZE; i++){

        (array_of_stack[descryptor]).data[i] = POIZON_VALUE;
    }

    return SUCCESS;
}

static statuses stack_reallocation (int descryptor, const int parametr){

    if (parametr){

        (array_of_stack[descryptor]).capacity *= RE_SIZING;

        #ifdef CANARY

        elem_t* data1 = (elem_t*) realloc((char*) (array_of_stack[descryptor]).data - sizeof(canary_t), (array_of_stack[descryptor]).capacity * sizeof(elem_t) + sizeof(canary_t)*2);

        if (data1 == nullptr){

            return NO_MEMORY;
        }

        (array_of_stack[descryptor]).data = (elem_t*) ((char*) data1 + sizeof(canary_t));

        *(canary_t*)((char*) (array_of_stack[descryptor]).data + (array_of_stack[descryptor]).capacity * sizeof(elem_t)) = DEFAULT_CANARY_VALUE;

        #else

        elem_t* data_backup = (elem_t*) realloc((array_of_stack[descryptor]).data, (array_of_stack[descryptor]).capacity * sizeof(elem_t));

        if (data_backup == nullptr){

            return NO_MEMORY;
        }

        (array_of_stack[descryptor]).data = data_backup;

        #endif

        clean_right_data (descryptor);

        return SUCCESS;
    }

    else {

        (array_of_stack[descryptor]).capacity = ((array_of_stack[descryptor]).capacity)/4;

        #ifdef CANARY

        elem_t* data1 = (elem_t*) realloc((char*) (array_of_stack[descryptor]).data - sizeof(canary_t), (array_of_stack[descryptor]).capacity * sizeof(elem_t) + sizeof(canary_t)*2);

        if (data1 == nullptr){

            return NO_MEMORY;
        }

        (array_of_stack[descryptor]).data = (elem_t*) ((char*) data1 + sizeof(canary_t));

        *(canary_t*)((char*) (array_of_stack[descryptor]).data + (array_of_stack[descryptor]).capacity * sizeof(elem_t)) = DEFAULT_CANARY_VALUE;

        #else

        elem_t* data_backup = (elem_t*) realloc((array_of_stack[descryptor]).data, (array_of_stack[descryptor]).capacity * sizeof(elem_t));

        if (data_backup == nullptr){

            return NO_MEMORY;
        }

        (array_of_stack[descryptor]).data = data_backup;

        #endif

        return SUCCESS;
    }
}
