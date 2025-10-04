#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "spu.h"
#include "stack.h"

void SpuCtor(const char* file_name, SPU_t *spu) {

    FILE *file_ptr = fopen(file_name, "rb");
    struct stat file_info = {};
    stat(file_name, &file_info);

    spu->code = (int *) calloc(1, file_info.st_size);
    fread(spu->code, 1, file_info.st_size, file_ptr);
    spu->commands_count = file_info.st_size / sizeof(int);

    StackCtor(&spu->stk, 10);

    fclose(file_ptr);
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

    if (stk->code == NULL)
        error |= Code_Error;

    return error;
}

int SpuDump(SPU_t *spu, const char *file_name, const char *function_name, int line_number) {

    int error = SpuVerify(spu, file_name, function_name, line_number);

    StackDump(&spu->stk);

    fprintf(
}
