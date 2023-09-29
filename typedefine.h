#ifndef typedefine
#define typedefine

#include <stdio.h>
#include <stdlib.h>

typedef char elem_t;/// if you use char type, you must change poizon value to char

typedef long long int canary_t;

const elem_t POIZON_VALUE = 91919; ///

#define CANARY  /// to enable canary protection
#define HASH_PROTECTION/// to enable hash_protection
#define OUTPUT_PARAMETR "%c" /// you must connect type of output with typeof elem_t
#define INCREMENT_DYNAMIC

#endif
