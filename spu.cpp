#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "spu.h"
#include "../include/commands.h"
#include "stack.h"
#include "math.h"

#define SEREGA stderr


int SpuCtor(const char* file_name, SPU_t *spu) {

    FILE *file_ptr = fopen(file_name, "rb");
    struct stat file_info = {};
    stat(file_name, &file_info);

    //spu->version = 6;
    //spu->signature = 451;

    int version = 0;
    fread(&version, 1, sizeof(int), file_ptr);
    int signature = 0;
    fread(&signature, 1, sizeof(int), file_ptr);

    if (spu->version != version) {
        printf("Version check is wrong.\n");
        return Spu_Version_Error;
    }
    if (spu->signature != signature) {
        printf("Signature check is wrong.\n");
        return Spu_Signature_Error;
    }

    spu->code = (int *) calloc(1, file_info.st_size);
    fread(spu->code, 1, file_info.st_size, file_ptr);
    spu->commands_count = file_info.st_size / sizeof(int);
    spu->counter = 0;

    StackCtor(&spu->stk, 10);
    StackCtor(&spu->ret_addr, 1);

    spu->RAM = [];

    fclose(file_ptr);

    Return_Spu_Error(spu);
}

void SpuRun(SPU_t *spu) {

    bool end_of_reading = true;

    while (spu->counter < spu->commands_count && end_of_reading) {
//printf("Itarration of bin cycle number %d. Curr command is %d\n", spu->counter, spu->code[spu->counter]);

        Break_If_Spu_Error(spu);

//SpuDump(spu, __FILE__, __FUNCTION__, __LINE__);

        switch(spu->code[spu->counter++]) {

            case PUSH:
                StackPush(&spu->stk, spu->code[spu->counter++]);
                break;

            case ADD: {
                int first_num = 0;
                int second_num = 0;

                StackPop(&spu->stk, &first_num);
                StackPop(&spu->stk, &second_num);

                StackPush(&spu->stk, first_num + second_num);

                break;
            }

            case SUB: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&spu->stk, &second_num);
                StackPop(&spu->stk, &first_num);

                StackPush(&spu->stk, first_num - second_num);

                break;
            }

            case DIV: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&spu->stk, &second_num);
                StackPop(&spu->stk, &first_num);

                StackPush(&spu->stk, first_num / second_num);

                break;
            }

            case OUT: {
                int result = 0;
                StackPop(&spu->stk, &result);

                printf("\nResult is %d\n\n", result);

                break;
            }

            case MUL: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&spu->stk, &second_num);
                StackPop(&spu->stk, &first_num);

                StackPush(&spu->stk, first_num * second_num);

                break;
            }

            case POW: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&spu->stk, &second_num);
                StackPop(&spu->stk, &first_num);

                StackPush(&spu->stk, pow(first_num, second_num));

                break;
            }

            case SQRT: {
                int num = 0;

                StackPop(&spu->stk, &num);
                StackPush(&spu->stk, sqrt(num));

                break;
            }

            case POPREG:
                switch(spu->code[spu->counter++]) {
                    case AX:
                        StackPop(&spu->stk, &spu->AX);
                        break;

                    case BX:
                        StackPop(&spu->stk, &spu->BX);
                        break;

                    case CX:
                        StackPop(&spu->stk, &spu->CX);
                        break;

                    case DX:
                        StackPop(&spu->stk, &spu->CX);
                        break;

                    case RV:
                        StackPop(&spu->stk, &spu->RV);

                    default:
                        break;
                }

                break;

            case PUSHREG:
                switch(spu->code[spu->counter++]) {
                    case AX:
                        StackPush(&spu->stk, spu->AX);
                        break;

                    case BX:
                        StackPush(&spu->stk, spu->BX);
                        break;

                    case CX:
                        StackPush(&spu->stk, spu->CX);
                        break;

                    case DX:
                        StackPush(&spu->stk, spu->CX);
                        break;

                    case RV:
                        StackPush(&spu->stk, spu->RV);

                    default:
                        break;
                }

                break;

            case IN: {
                printf("Enter number\n");
                int num = 0;
                scanf("%d", &num);

                StackPush(&spu->stk, num);

                break;
            }

            case JUMP: {
                spu->counter = spu->code[spu->counter];

                break;                                    // +2 это потому что версия и константа
            }                                             // убери науъй это пожалуйста->

            case JB: {
                int num_1 = 0;
                int num_2 = 0;
                StackPop(&spu->stk, &num_2);
                StackPop(&spu->stk, &num_1);

                if (num_1 < num_2)
                    spu->counter = spu->code[spu->counter];  // :)
                else
                    spu->counter++;

                break;
            }

            case JBE: {
                int num_1 = 0;
                int num_2 = 0;
                StackPop(&spu->stk, &num_2);
                StackPop(&spu->stk, &num_1);

                if (num_1 <= num_2)
                    spu->counter = spu->code[spu->counter];  // ;D
                else
                    spu->counter++;

                break;
            }

            case JA: {
                int num_1 = 0;
                int num_2 = 0;
                StackPop(&spu->stk, &num_2);
                StackPop(&spu->stk, &num_1);

                if (num_1 > num_2)
                    spu->counter = spu->code[spu->counter];  // X(
                else
                    spu->counter++;

                break;
            }

            case JAE: {
                int num_1 = 0;
                int num_2 = 0;
                StackPop(&spu->stk, &num_2);
                StackPop(&spu->stk, &num_1);

                if (num_1 >= num_2)
                    spu->counter = spu->code[spu->counter];
                else
                    spu->counter++;

                break;
            }

            case JE: {
                int num_1 = 0;
                int num_2 = 0;
                StackPop(&spu->stk, &num_2);
                StackPop(&spu->stk, &num_1);

                if (num_1 == num_2)
                    spu->counter = spu->code[spu->counter];
                else
                    spu->counter++;

                break;
            }

            case JNE: {
                int num_1 = 0;
                int num_2 = 0;
                StackPop(&spu->stk, &num_2);
                StackPop(&spu->stk, &num_1);

                if (num_1 != num_2)
                    spu->counter = spu->code[spu->counter];
                else
                    spu->counter++;

                break;
            }

            case CALL:
                StackPush(&spu->ret_addr, spu->counter + 1);
                spu->counter = spu->code[spu->counter];

                break;

            case RET: {
                int addres = 0;
                StackPop(&spu->ret_addr, &addres);

                spu->counter = addres;

                break;
            }

            case PUSHM: {
                switch(spu->code[spu->counter++]) {
                    case AX:
                        StackPush(&spu->stk, RAM[spu->AX]);
                        break;

                    case BX:
                        StackPush(&spu->stk, RAM[spu->BX]);
                        break;

                    case CX:
                        StackPush(&spu->stk, RAM[spu->CX]);
                        break;

                    case DX:
                        StackPush(&spu->stk, RAM[spu->DX]);
                        break;

                    case RV:
                        RAM[spu->RV] = num;
                        break;

                    default:
                        break;
                }

                break;
            }

            case POPM: {
                switch(spu->code[spu->counter++]) {
                    int num = 0;
                    StackPop(&spu->stk, &num);

                    case AX:
                        RAM[spu->AX] = num;
                        break;

                    case BX:
                        RAM[spu->BX] = num;
                        break;

                    case CX:
                        RAM[spu->CX] = num;
                        break;

                    case DX:
                        RAM[spu->DX] = num;
                        break;

                    case RV:
                        RAM[spu->RV] = num;
                        break;

                    default:
                        break;
                }

                break;
            }

            case HLT:
                end_of_reading = false;
                break;

            default:
                printf("Something got wrong. Calculating is ended.\n");
                end_of_reading = false;
                break;

        }

    }
}

void SpuDtor(SPU_t *spu) {

    StackDtor(&spu->stk);
    /*
    if (spu->code != NULL) {
        for (int counter = 0; counter < spu->commands_count; counter++)
            spu->code[counter] = POISON;

    }
    */
    free(spu->code);

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

    // check file_name and function_name if NULL

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

    fprintf(SEREGA, "RV = %d, AX = %d, BX = %d, CX = %d\n\n", spu->RV, spu->AX, spu->BX, spu->CX);

    fprintf(SEREGA, "-----------------------\nCode Array: \n");

    for (int counter = 0; counter < spu->commands_count; counter++)
        fprintf(SEREGA, "%02d ", spu->code[counter]);

    fprintf(SEREGA, "\n");

    for (int counter = 0; counter < 2*spu->counter - 1; counter++)
        ;

    fprintf(SEREGA, "^^\n\n===============================\n\n");

}
