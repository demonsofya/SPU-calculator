#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "spu.h"
#include "stack.h"

#define SEREGA stderr

int SpuCtor(const char* file_name, SPU_t *spu) {

    FILE *file_ptr = fopen(file_name, "rb");
    struct stat file_info = {};
    stat(file_name, &file_info);

    spu->code = (int *) calloc(1, file_info.st_size);
    fread(spu->code, 1, file_info.st_size, file_ptr);
    spu->commands_count = file_info.st_size / sizeof(int);

    StackCtor(&spu->stk, 10);

    fclose(file_ptr);

    Return_Spu_Error(spu);
}

void SpuDtor(SPU_t *spu) {
    StackDtor(&spu->stk);

    if (spu->code != NULL) {
        for (int counter = 0; counter < spu->commands_count; counter++)
            spu->code[counter] = POISON;

        free(spu->code);
    }

    spu->commands_count = POISON;
    spu->counter = POISON;
    spu->AX = POISON;
    spu->BX = POISON;
    spu->CX = POISON;
    spu->code = NULL;
}

int SpuVerify(SPU_t *spu) {

    int error = 0;

    if (spu == NULL)
        return Spu_Pointer_Error;

    if (spu->commands_count < 0)
        error |= Spu_Size_Error;

    if (spu->counter < 0)
        error |= Counter_Error;

    if (StackVerify(&spu->stk) != 0)
        error |= Stack_Error;

    if (spu->code == NULL)
        error |= Code_Error;

    return error;
}

void SpuDump(SPU_t *spu, const char *file_name, const char *function_name, int line_number) {

    int error = SpuVerify(spu);

    if (error & Spu_Pointer_Error || spu == NULL) {      // Same variants, just to be sure

        fprintf(SEREGA, "ERROR! SPU pointer is wrong. Pointer is [%p]\n\n", spu);
        return;
    }

    StackDump(&spu->stk, file_name, function_name, line_number);

    fprintf(SEREGA, "SPU struct [%p]\n", spu);

    if (error & Code_Error) {

        fprintf(SEREGA, "ERROR! Code buffer is wrong. Code pointer is [%p]\n\n", spu->code);    // Working
        return;
    }

    fprintf(SEREGA, "Code buffer [%p]\n", spu->code);

    if (error & Spu_Size_Error)
        fprintf(SEREGA, "ERROR! Size is wrong, size is %d\n\n", spu->commands_count);

    if (error & Counter_Error)
        fprintf(SEREGA, "ERROR! Counter is wrong, capacity is %d\n\n", spu->counter);

    if (!(error & Spu_Size_Error) && !(error & Counter_Error))
        fprintf(SEREGA, "Size is %d, counter is %d\n\n", spu->commands_count, spu->counter);

    fprintf(SEREGA, "-----------------------\nCode Array: \n");

    for (int counter = 0; counter < spu->commands_count; counter++)
        fprintf(SEREGA, "%02d ", spu->code[counter]);

    fprintf(SEREGA, "\n");

    for (int counter = 0; counter < 2*spu->counter - 1; counter++)
        ;

    fprintf(SEREGA, "^^\n\n===============================\n\n");

}
