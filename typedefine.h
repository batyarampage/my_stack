#ifndef typedefine
#define typedefine

#include <stdio.h>
#include <stdlib.h>

typedef long long int elem_t;

typedef long long int canary_t;

#define CANARY  /// to enable canary protection
#define HASH_PROTECTION /// to enable hash_protection
#define OUTPUT_PARAMETR "%lld" /// you must connect type of output with typeof elem_t

#endif
