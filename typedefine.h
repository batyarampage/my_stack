#ifndef typedefine
#define typedefine

#include <stdio.h>
#include <stdlib.h>

typedef long long int elem_t;/// if you use char type, you must change poizon value to char

typedef long long int canary_t;

const elem_t POIZON_VALUE = 91919; ///

#define CANARY  /// to enable canary protection
/// to enable hash_protection
#define OUTPUT_PARAMETR "%lld" /// you must connect type of output with typeof elem_t

#endif
