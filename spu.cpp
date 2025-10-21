#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "spu.h"
#include "../include/commands.h"
#include "lib/stack/stack.h"
#include "math.h"
#include "assert.h"
#include "lib/onegin/buffer.h"

#define SEREGA stderr

//=============================================================================

int CheckVersionAndSignature(SPU_t *spu, FILE *file_ptr) {

    if (spu == NULL || file_ptr == NULL)
        return Spu_Pointer_Error;

    int version = 0;
    fread(&version, 1, sizeof(int), file_ptr);

    int signature = 0;
    fread(&signature, 1, sizeof(int), file_ptr);

    if (version != DEFAULT_INFORMATION.version) {

        printf("Version check is wrong.\n");
        return Spu_Version_Error;
    }

    if (signature != DEFAULT_INFORMATION.signature) {

        printf("Signature check is wrong.\n");
        return Spu_Signature_Error;
    }

    return No_Spu_Error;
}

//-----------------------------------------------------------------------------

int CodeCtor(SPU_t *spu, size_t file_size, FILE *file_ptr) {

    if (spu == NULL || file_ptr == NULL)
        return Spu_Pointer_Error;

    spu->code = (int *) calloc(1, file_size);
    fread(spu->code, 1, file_size, file_ptr);

    spu->commands_count = file_size / COMMAND_SIZE;
    spu->counter = 0;

    return No_Spu_Error;
}

//-----------------------------------------------------------------------------

int RAMCtor(SPU_t *spu) {

    if (spu == NULL)
        return Spu_Pointer_Error;

    spu->RAM = (int *) calloc(RAM_DEFAULT_SIZE, sizeof(int));
    spu->ram_size = RAM_DEFAULT_SIZE;

    return No_Spu_Error;
}

//-----------------------------------------------------------------------------

int SpuCtor(const char* file_name, SPU_t *spu) {

    if (spu == NULL || file_name == NULL)
        return Spu_Pointer_Error;

    FILE *file_ptr = fopen(file_name, "rb");

    int file_size = GetFileSize(file_name);

    int signature_error = CheckVersionAndSignature(spu, file_ptr);
    if (signature_error != No_Spu_Error)
        return signature_error;

    CodeCtor(spu, file_size, file_ptr);

    StackCtor(&spu->stk, 10);
    StackCtor(&spu->ret_addr, 1);

    RAMCtor(spu);

    fclose(file_ptr);

    Return_Spu_Error(spu);
}

//=============================================================================

int RunBinaryMathOperation(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int first_num = 0;
    int second_num = 0;

    StackPop(&spu->stk, &second_num);
    StackPop(&spu->stk, &first_num);

    int calculation_result = 0;

    switch(command) {
        case ADDICTION_COMMAND:
            calculation_result = first_num + second_num;
            break;

        case SUBTRACTION_COMMAND:
            calculation_result = first_num - second_num;
            break;

        case DIVISION_COMMAND:
            if (second_num == 0)
                return Spu_Command_Error;
            calculation_result = first_num / second_num;
            break;

        case MULTIPLICATION_COMMAND:
            calculation_result = first_num * second_num;
            break;

        case POW_COMMAND:
            calculation_result = int(pow(first_num, second_num));
            break;

        default:
            return Spu_Command_Error;
            break;
    }


    StackPush(&spu->stk, calculation_result);

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunJumpWithCondition(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int num_1 = 0;
    int num_2 = 0;

    StackPop(&spu->stk, &num_2);
    StackPop(&spu->stk, &num_1);

    bool compare_result = false;

    switch(command) {
        case JUMP_BELOW_COMMAND:
            compare_result = num_1 < num_2;
            break;

        case JUMP_BELOW_EQUAL_COMMAND:
            compare_result = num_1 <= num_2;
            break;

        case JUMP_ABOVE_COMMAND:
            compare_result = num_1 > num_2;
            break;

        case JUMP_ABOVE_EQUAL_COMMAND:
            compare_result = num_1 >= num_2;
            break;

        case JUMP_EQUAL_COMMAND:
            compare_result = num_1 == num_2;
            break;

        case JUMP_NOT_EQUAL_COMMAND:
            compare_result = num_1 != num_2;
            break;

        default:
            return Spu_Command_Error;

    }

    if (compare_result)
        spu->counter = spu->code[spu->counter];
    else
        spu->counter++;

ON_DEBUG(printf("Jump: jump result is %d, numbers are %d and %d\n", compare_result, num_1, num_2));

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunJumpWhitoutCondition(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    spu->counter = spu->code[spu->counter];

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunPushRegister(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int reg_num = spu->code[spu->counter++];
    StackPush(&spu->stk, spu->registers[reg_num]);

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunPopRegister(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int reg_num = spu->code[spu->counter++];
    StackPop(&spu->stk, &spu->registers[reg_num]);

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunOut(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int result = 0;
    StackPop(&spu->stk, &result);

    printf("\nResult is %d\n\n", result);

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunUnaryMathOperation(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int num = 0;
    int calculation_result = 0;

    StackPop(&spu->stk, &num);

    switch(command) {
        case SQUARE_ROOT_COMMAND:
            calculation_result = int(sqrt(num));
            break;

        default:
            return Spu_Command_Error;
            break;
    }

    StackPush(&spu->stk, calculation_result);

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunIn(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    printf("Enter number\n");
    int num = 0;
    scanf("%d", &num);

    StackPush(&spu->stk, num);

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunRetern(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int addres = 0;
    StackPop(&spu->ret_addr, &addres);

    spu->counter = addres;

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunPopMemory(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int num = 0;
    StackPop(&spu->stk, &num);

    int register_number = spu->code[spu->counter++];
    int register_value = spu->registers[register_number];
    spu->RAM[register_value] = num;

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunDraw(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int square_side_size = int(sqrt(spu->ram_size));

    for (int curr_num = 0; curr_num < spu->ram_size; curr_num++) {

        printf("%c", spu->RAM[curr_num]);
        if ((curr_num + 1) % square_side_size == 0)
            printf("\n");
    }

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunPushMemory(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    int register_number = spu->code[spu->counter++];
    int register_value = spu->registers[register_number];
    int RAM_cell_value = spu->RAM[register_value];
    StackPush(&spu->stk, RAM_cell_value);

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunCall(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    StackPush(&spu->ret_addr, spu->counter + 1);
    spu->counter = spu->code[spu->counter];

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int RunPush(SPU_t *spu, int command) {

    Return_If_Spu_Error(spu);

    StackPush(&spu->stk, spu->code[spu->counter++]);

    Return_Spu_Error(spu);
}

//=============================================================================

int RunCurrentCommand(SPU_t *spu, int curr_command) {

    Return_If_Spu_Error(spu);

    /*
    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)

        if (Commands_fuctions_array[curr_num].command_num == curr_command)
            Commands_fuctions_array[curr_num].command_function_ptr(spu, curr_command);
    */

    Commands_fuctions_array[curr_command].command_function_ptr(spu, curr_command);

    Return_Spu_Error(spu);
}

//-----------------------------------------------------------------------------

int SpuRun(SPU_t *spu) {

    Return_If_Spu_Error(spu);

    while (spu->counter < spu->commands_count) {

ON_DEBUG(printf("Itarration of bin cycle number %d. Curr command is %d or %s\n",
                spu->counter, spu->code[spu->counter],
                commands_array[spu->code[spu->counter]].command_name));

        Return_If_Spu_Error(spu);

ON_DEBUG(SpuDump(spu, __FILE__, __FUNCTION__, __LINE__));

        int curr_command = spu->code[spu->counter++];
        int curr_error = 0;

        if (curr_command != HALT_COMMAND)
            curr_error = RunCurrentCommand(spu, curr_command);
        else
            return No_Spu_Error;

        if (curr_error != No_Spu_Error)
            return Spu_Command_Error;
    }

    Return_Spu_Error(spu);
}

//=============================================================================

void SpuDtor(SPU_t *spu) {

    StackDtor(&spu->stk);
    // Работает долго, поэтому убрала
    /*
    if (spu->code != NULL) {
        for (int counter = 0; counter < spu->commands_count; counter++)
            spu->code[counter] = POISON;

    }
    */
    if (spu->code != NULL)
        free(spu->code);

    if (spu->RAM != NULL)
        free(spu->RAM);

    spu->commands_count = POISON;
    spu->counter = POISON;;
    spu->code = NULL;
    spu->RAM = NULL;
}

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

void SpuDump(SPU_t *spu, const char *file_name, const char *function_name, int line_number) {

    int error = SpuVerify(spu);

    if (error & Spu_Pointer_Error || spu == NULL) {      // Same variants, just to be sure

        fprintf(SEREGA, "ERROR! SPU pointer is wrong. Pointer is [%p]\n\n", spu);
        return;
    }

    StackDump(&spu->stk, file_name, function_name, line_number);

    fprintf(SEREGA, "SPU struct [%p]\n", spu);

    if (error & Code_Error) {

        fprintf(SEREGA, "ERROR! Code buffer is wrong. Code pointer is [%p]\n\n", spu->code);
        return;
    }

    fprintf(SEREGA, "Code buffer [%p]\n", spu->code);

    if (error & Spu_Size_Error)
        fprintf(SEREGA, "ERROR! Size is wrong, size is %d\n\n", spu->commands_count);

    if (error & Counter_Error)
        fprintf(SEREGA, "ERROR! Counter is wrong, capacity is %d\n\n", spu->counter);

    if (!(error & Spu_Size_Error) && !(error & Counter_Error))
        fprintf(SEREGA, "Size is %d, counter is %d\n\n", spu->commands_count, spu->counter);

    fprintf(SEREGA, "---------RAM----------\n");
    for (int curr_num = 0; curr_num < spu->ram_size; curr_num++)
        fprintf(SEREGA, "[%d] ", spu->RAM[curr_num]);
    fprintf(SEREGA, "\n\n");

    fprintf(SEREGA, "---------REGISTERS--------\n");
    for (int curr_num = 0; curr_num < REGISTERS_COUNT; curr_num++)
        fprintf(SEREGA, "[%d] ", spu->registers[REGISTERS_COUNT]);
    fprintf(SEREGA, "\n\n");

    fprintf(SEREGA, "-----------------------\nCode Array: \n");

    for (int counter = 0; counter < spu->commands_count; counter++)
        fprintf(SEREGA, "%02d ", spu->code[counter]);

    fprintf(SEREGA, "\n");

    for (int counter = 0; counter < 2*spu->counter - 1; counter++)
        ;

    fprintf(SEREGA, "^^\n\n===============================\n\n");

}

//=============================================================================
