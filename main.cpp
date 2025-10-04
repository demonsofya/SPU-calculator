#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <assert.h>

#include "buffer.h"
#include "stack.h"
#include "commands.h"
#include "spu.h"

int main()
{
    int command = 0;

    SPU_t spu = {};
    SpuCtor("task_bin.txt", &spu);

    bool flag = true;

    while (spu.counter < spu.commands_count && flag) {
//printf("Itarration of bin cycle number %d. Curr command is %d\n", counter, code[counter]);

        switch(spu.code[spu.counter]){

            case PUSH:
                StackPush(&spu.stk, spu.code[++spu.counter]);
                break;

            case ADD: {
                int first_num = 0;
                int second_num = 0;

                StackPop(&spu.stk, &first_num);
                StackPop(&spu.stk, &second_num);

                StackPush(&spu.stk, first_num + second_num);

                break;
            }

            case SUB: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&spu.stk, &second_num);
                StackPop(&spu.stk, &first_num);

                StackPush(&spu.stk, first_num - second_num);

                break;
            }

            case DIV: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&spu.stk, &second_num);
                StackPop(&spu.stk, &first_num);

                StackPush(&spu.stk, first_num / second_num);

                break;
            }

            case OUT: {
                int result = 0;
                StackPop(&spu.stk, &result);

                printf("Result is %d\n", result);

                break;
            }

            case MVL: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&spu.stk, &second_num);
                StackPop(&spu.stk, &first_num);

                StackPush(&spu.stk, first_num * second_num);

                break;
            }

            case POW: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&spu.stk, &second_num);
                StackPop(&spu.stk, &first_num);

                StackPush(&spu.stk, pow(first_num, second_num));

                break;
            }

            case SQRT: {
                int num = 0;

                StackPop(&spu.stk, &num);
                StackPush(&spu.stk, sqrt(num));

                break;
            }

            case POPREG:
                switch(spu.code[++spu.counter]) {
                    case AX:
                        StackPop(&spu.stk, &spu.AX);
                        break;

                    case BX:
                        StackPop(&spu.stk, &spu.BX);
                        break;

                    case CX:
                        StackPop(&spu.stk, &spu.CX);
                        break;

                    default:
                        break;
                }

                break;

            case PUSHREG:
                switch(spu.code[++spu.counter]) {
                    case AX:
                        StackPush(&spu.stk, spu.AX);
                        break;

                    case BX:
                        StackPush(&spu.stk, spu.BX);
                        break;

                    case CX:
                        StackPush(&spu.stk, spu.CX);
                        break;

                    default:
                        break;
                }

                break;

            case IN: {
                int num = 0;
                scanf("%d", &num);

                StackPush(&spu.stk, num);

                break;
            }

            case HLT:
                flag = false;
                break;

            default:
                printf("Something got wrong. Calculating is ended.\n");
                flag = false;

        }

        spu.counter++;
        /*if (code[counter] == PUSH)
            StackPush(&stk_bin, code[++counter]);

        else if (code[counter] == ADD) {
            int first_num = 0;
            int second_num = 0;

            StackPop(&stk_bin, &first_num);
            StackPop(&stk_bin, &second_num);

            StackPush(&stk_bin, first_num + second_num);
        }

        else if (code[counter] == SUB) {
            int second_num = 0;
            int first_num = 0;

            StackPop(&stk_bin, &second_num);
            StackPop(&stk_bin, &first_num);

            StackPush(&stk_bin, first_num - second_num);
        }

        else if (code[counter] == DIV) {
            int second_num = 0;
            int first_num = 0;

            StackPop(&stk_bin, &second_num);
            StackPop(&stk_bin, &first_num);

            StackPush(&stk_bin, first_num / second_num);
        }

        else if (code[counter] == OUT) {
            int result = 0;
            StackPop(&stk_bin, &result);

            printf("Result is %d\n\n", result);

        }

        else if (code[counter] == MVL) {
            int second_num = 0;
            int first_num = 0;

            StackPop(&stk_bin, &second_num);
            StackPop(&stk_bin, &first_num);

            StackPush(&stk_bin, first_num * second_num);
        }

        else if (code[counter] == POW) {
            int second_num = 0;
            int first_num = 0;

            StackPop(&stk_bin, &second_num);
            StackPop(&stk_bin, &first_num);

            StackPush(&stk_bin, pow(first_num, second_num));
        }

        else if (code[counter] == SQRT) {
            int num = 0;

            StackPop(&stk_bin, &num);

            StackPush(&stk_bin, sqrt(num));
        }

        else if (code[counter] == HLT)
            break;

        else
            break;

        counter++;*/
    }

    SpuDtor(&spu);

    //-----------------------------------------------------------------------------
//printf("Half of programm is solved\n");
    //-----------------------------------------------------------------------------


    size_t file_size = 0;
    char *buffer = GetBuffer("task_rewrite.txt", &file_size);
    int ptr_array_size = StringsCount(buffer);
    char **text_ptr_array = GetStringPtrArray(buffer, ptr_array_size);

    assert(buffer);
    assert(text_ptr_array);

    Stack_t stk = {};
    StackCtor(&stk, 10);

    int counter = 0;
    flag = true;
    while (counter < ptr_array_size && flag) {
        sscanf(text_ptr_array[counter], "%d", &command);
//printf("Itarration of another cycle number %d. Curr command is %d\n", counter, command);

        switch (command) {
            case PUSH: {
                int curr_number = 0;
                sscanf(text_ptr_array[counter], "%*d %d", &curr_number);

                StackPush(&stk, curr_number);
                break;
            }

            case ADD: {
                int first_num = 0;
                int second_num = 0;

                StackPop(&stk, &first_num);
                StackPop(&stk, &second_num);

                StackPush(&stk, first_num + second_num);
                break;
            }

            case SUB: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&stk, &second_num);
                StackPop(&stk, &first_num);

                StackPush(&stk, first_num - second_num);
                break;
            }

            case DIV: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&stk, &second_num);
                StackPop(&stk, &first_num);

                StackPush(&stk, first_num / second_num);
                break;
            }

            case OUT: {
                int result = 0;
                StackPop(&stk, &result);

                printf("Result is %d\n\n", result);
                break;
            }

            case MVL: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&stk, &second_num);
                StackPop(&stk, &first_num);

                StackPush(&stk, first_num * second_num);
                break;
            }

            case POW: {
                int second_num = 0;
                int first_num = 0;

                StackPop(&stk, &second_num);
                StackPop(&stk, &first_num);

                StackPush(&stk, pow(first_num, second_num));
                break;
            }

            case SQRT: {
                int num = 0;

                StackPop(&stk, &num);

                StackPush(&stk, sqrt(num));
                break;
            }

            case HLT:
                flag = false;
                break;

            default:
                printf("Something got wrong. Calculating is ended.\n");
                flag = false;
        }

        counter++;
    }



    return 0;
}
