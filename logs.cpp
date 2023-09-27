#include "logs.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

FILE* LOG_FILE = nullptr;

void log_start (){

    LOG_FILE = fopen("logs.txt", "a+");

    assert(LOG_FILE != nullptr);

    fprintf(LOG_FILE, "logs is begun\n\n");
}


void log_end (){

    fprintf(LOG_FILE, "logs was end\n");

    fclose(LOG_FILE);
}
