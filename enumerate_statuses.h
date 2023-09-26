#ifndef enumerate_statuses
#define enumerate_statuses

enum statuses {

    SUCCESS = 1,
    ERROR = -1,
    STACK_WAS_INICIALASED_BEFORE = -3,
    NO_MEMORY = -5,
    SIZE = 7
};

enum statuses_stack_ok {

    INCORRECT_STACK              = -1,
    INCORRECT_CAPACITY           = 1,
    INCORRECT_SIZE               = 2,
    INCORRECT_DIFF_SIZE_CAPACITY = 3,
    INCORRECT_DATA               = 4,
    INCORRECT_CANARY             = 6,
    INCORRECT_HASH               = 7,
    SUCCESS_STACK                = 5
};

enum conddition_stack {

    NOT_INICIALED = 0,
    IS_INICIALED = 1,
    DESTRUCTED = 2

};

#endif
