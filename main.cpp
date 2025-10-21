#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "../include/commands.h"
#include "assembler.h"

int main(int argc, char *argv[]) {

    int commands_count = 0;
    int *output_arr = NULL;
    if (argc > 1)
        output_arr = CreateCodeArray(argv[1], &commands_count);
    else
        output_arr = CreateCodeArray("Task7.txt", &commands_count);


    if (argc > 2)
        CreateBinaryFile(argv[2], commands_count, output_arr);
    else
        CreateBinaryFile("task_bin.txt", commands_count, output_arr);


    CreateNormalFile("task_rewrite.txt", commands_count, output_arr);

    return 0;
}
