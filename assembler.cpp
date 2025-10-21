#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lib/onegin/buffer.h"
//#include "buffer.h"
#include "../include/commands.h"
#include "assembler.h"
#include "string.h"

//=============================================================================

char **GetStringsPtrArrayFromFile(const char *file_name, int *ptrs_to_code_lines_array_size) {

    assert(file_name);
    assert(ptrs_to_code_lines_array_size);

    size_t file_size = 0;
    char *buffer = GetBuffer(file_name, &file_size);

    assert(buffer);

    *ptrs_to_code_lines_array_size = StringsCount(buffer);
    char **strings_ptr_array = GetStringPtrArray(buffer, *ptrs_to_code_lines_array_size);

//ON_DEBUG(printf("First string is %s\n\n", strings_ptr_array[0]));

    assert(strings_ptr_array);

    return strings_ptr_array;
}

//=============================================================================

void CountHashTable() {

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)
        commands_array[curr_num].hash = CountStringHashDJB2(commands_array[curr_num].command_name);

    for (int curr_num = 0; curr_num < REGISTERS_COUNT; curr_num++)
        registers_array.hash = CountStringHashDJB2
}

//=============================================================================

void FillCurrentOpcode(int *output_arr, char *command_string,
                       int *labels, int *command_num, char *argument_string, int *error) {

    assert(output_arr);
    assert(command_string);
    assert(labels);
    assert(command_num);

ON_DEBUG(printf("Curr command is %s\n", command_string));

    int curr_hash = CountStringHashDJB2(command_string);

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)

        //if (strcmp(command_string, commands_array[curr_num].command_name) == 0) {
        if (commands_array[curr_num].hash == curr_hash) {
            output_arr[(*command_num)++] = commands_array[curr_num].command_number;

            if (commands_array[curr_num].agrument_type == NUMBER_ARGUMENT)
                FillCommandWithNumberArgument(output_arr, argument_string, labels, command_num, error);

            else if (commands_array[curr_num].agrument_type == REGISTER_ARGUMENT)
                FillCommandWithRegisterArgiment(output_arr, argument_string, labels, command_num, error);

            else if (commands_array[curr_num].agrument_type == RAM_ARGUMENT)
                FillCommandWithRAMArgiment(output_arr, argument_string, labels, command_num, error);

            return;

        }

    *error |= COMMAND_ASM_ERROR;
}

//=============================================================================

int CheckRegister(char *reg, int *error) {

    if (reg == NULL)
        return REGISTER_ASM_ERROR;

ON_DEBUG(printf("Register is %s, size is %d\n\n", reg, strlen(reg)));

    for (int curr_num = 0; curr_num < REGISTERS_COUNT; curr_num++) {

        if (strcmp(reg, registers_array[curr_num].reg_name) == 0)
            return registers_array[curr_num].reg_num;

ON_DEBUG(printf("Curr register number %d is %s\n", curr_num, registers_array[curr_num].reg_name));

    }

    *error |= REGISTER_ASM_ERROR;
ON_DEBUG(printf("Error with register. Register is %s\n", reg));
    return REGISTER_ASM_ERROR;
}


//=============================================================================


void FillCommandWithNumberArgument(int *output_arr, char *command_string,
                                   int *labels, int *command_num, int *error) {

    assert(output_arr);
    assert(command_string);
    assert(labels);
    assert(command_num);

    int curr_number = 0, readen_elements_count = 0;

    char label = '\0';
    sscanf(command_string, " %c", &label);

    if (label == ':') {
        char curr_string_label[10] = "";
        readen_elements_count = sscanf(command_string, " %*c%s", curr_string_label);

        curr_number = CountStringHashDJB2(curr_string_label);
        //readen_elements_count = sscanf(command_string, " %*c%d", &curr_number);
        output_arr[(*command_num)++] = labels[curr_number];

    } else {
        readen_elements_count = sscanf(command_string, "%d", &curr_number);
        output_arr[(*command_num)++] = curr_number;
    }

    if (readen_elements_count != 1)
        *error |= NUMBER_ARGUMENT_ASM_ERROR;
}


//-----------------------------------------------------------------------------

void FillCommandWithRegisterArgiment(int *output_arr, char *command_string,
                                     int *labels, int *command_num, int *error) {

    assert(output_arr);
    assert(command_string);
    assert(labels);
    assert(command_num);

    int readen_elements_count = 0;

    if (*command_num < 0)
        return;

    char reg[10] = "";

    readen_elements_count = sscanf(command_string, " %s", reg);

    if (readen_elements_count != 1)
        *error |= REGISTER_ASM_ERROR;

    output_arr[(*command_num)++] = CheckRegister(reg, error);
}


//-----------------------------------------------------------------------------

void FillCommandWithRAMArgiment(int *output_arr, char *command_string,
                                int *labels, int *command_num, int *error) {

    assert(output_arr);
    assert(command_string);
    assert(labels);
    assert(command_num);

    if (*command_num < 0)
        return;

    char reg[10] = "";
    char end_symbol = '\0';

    int readen_elements_count = 0;

    readen_elements_count = sscanf(command_string, " [%[A-Z]%c", reg, &end_symbol);
ON_DEBUG(printf("Curr RAM register is %s\n", reg));

    if (end_symbol != ']' || readen_elements_count != 2) {
        *error |= SINTAXYS_ASM_ERROR;
ON_DEBUG(printf("Wrong register sintaxys.\n"));
        (*command_num)++;

    } else {
        output_arr[(*command_num)++] = CheckRegister(reg, error);
    }
}

//=============================================================================

bool CheckIfCommandLabel(char *command) {

    assert(command);

    if (command[0] == ':')
        return true;

    return false;
}

//-----------------------------------------------------------------------------

void FillLabel(char *command, int *labels, int command_num, int *counter, int *error) {

    assert(command);
    assert(labels);
    assert(counter);

    int label_number = 0, readen_elements_count = 0;

    char curr_string_label[10] = "";
    readen_elements_count = sscanf(command, " %*c%s", curr_string_label);

    label_number = CountStringHashDJB2(curr_string_label);
        //readen_elements_count = sscanf(command_string, " %*c%d", &curr_number);
        //output_arr[(*command_num)++] = labels[curr_number];

    if (readen_elements_count != 1)
        *error |= LABELS_ASM_ERROR;

    labels[label_number] = command_num;
    (*counter)++;

ON_DEBUG(printf("Label number is %d, current command is %d\n", label_number, command_num));

}

//=============================================================================

int FillCodeArray(int *output_arr, int ptrs_to_code_lines_array_size,
                  char **ptrs_to_code_lines_array, int *labels) {

    assert(output_arr);
    assert(ptrs_to_code_lines_array_size);
    assert(labels);

    CountHashTable();

    char command_string[10] = "";
    int counter = 0, command_num = 0;

    while (counter < ptrs_to_code_lines_array_size) {
        int command_size = 0;
        sscanf(ptrs_to_code_lines_array[counter], "%s%n", command_string, &command_size);

        int error = NO_ASM_ERROR;

        if (strcmp(command_string, "") == 0) {
            counter++;
            continue;
        }

        if (CheckIfCommandLabel(command_string)) {
            FillLabel(command_string, labels, command_num, &counter, &error);
            continue;
        }

        FillCurrentOpcode(output_arr, command_string, labels, &command_num,
                          ptrs_to_code_lines_array[counter] + command_size, &error);

        if (error != NO_ASM_ERROR) {
            ErrorDump(&error);
            LabelsDump(labels);

            return command_num;
        }

        counter++;

    }

    return command_num;
}

//-----------------------------------------------------------------------------

bool CheckStrings() {
    bool result = true;

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)
        if (commands_array[curr_num].command_number != curr_num) {
            printf("Error. Position in array is %d, command is %s, command number is %d\n",
                    curr_num, commands_array[curr_num].command_name, commands_array[curr_num].command_number);

            result = false;
        }

    return result;
}

int *CreateCodeArray(const char *input_file_name, int *commands_count) {

    assert(input_file_name);
    assert(commands_count);

    int labels[LABELS_ARRAY_SIZE] = {};

    int ptrs_to_code_lines_array_size = 0;
    char **ptrs_to_code_lines_array = GetStringsPtrArrayFromFile(input_file_name,
                                                         &ptrs_to_code_lines_array_size);

ON_DEBUG(printf("Strings count is %d\n\n", ptrs_to_code_lines_array_size));

    int *output_arr = (int *) calloc(ptrs_to_code_lines_array_size * 2 + SIZE_OF_HEADER_IN_BYTES,
                                     sizeof(int));

    int command_num = 0;

    output_arr[0] = DEFAULT_INFORMATION.version;
    output_arr[1] = DEFAULT_INFORMATION.signature;

    for (int i = 0; i < 2; i++)
        command_num = FillCodeArray(output_arr +  SIZE_OF_HEADER_IN_BYTES,
                                    ptrs_to_code_lines_array_size, ptrs_to_code_lines_array, labels);

    *commands_count = command_num;

ON_DEBUG(LabelsDump(labels));

    return output_arr;
}

//=============================================================================

void LabelsDump(int *labels) {

    assert(labels);

    printf("------LABELS---------\n\n");

    for (int curr_num = 0; curr_num < LABELS_ARRAY_SIZE; curr_num++)
        printf("[%d] ", labels[curr_num]);

    printf("\n\n-----------------------\n");
}

//-----------------------------------------------------------------------------

void ErrorDump(int *error) {

    printf("----------ASM DUMP----------\n\n");

    if (*error & COMMAND_ASM_ERROR)
        printf("Wrong command\n");

    if (*error & REGISTER_ASM_ERROR)
        printf("Wrong Register\n");

    if (*error & NUMBER_ARGUMENT_ASM_ERROR)
        printf("Wrong Number Argument\n");

    if (*error & LABELS_ASM_ERROR)
        printf("Wrong Label Argument\n");

    if (*error & RAM_ARGUMENT_ERROR)
        printf("Wrong RAM Argument\n");

    printf("\n--------------------------\n\n");
}

//=============================================================================


void CreateBinaryFile(const char *file_name, int commands_count, int *output_arr) {

    assert(file_name);

    FILE *output_file = fopen(file_name, "wb");

    fwrite(output_arr, sizeof(int), commands_count +  SIZE_OF_HEADER_IN_BYTES, output_file);

    fclose(output_file);
}

//-----------------------------------------------------------------------------

bool CheckIfCommandHaveArgument(int command) {

    for (int curr_num = 0; curr_num < COMMANDS_COUNT; curr_num++)

        if (commands_array[curr_num].command_number == command) {
            if(commands_array[curr_num].agrument_type == NO_ARGUMENT)
                return false;

            return true;
        }

    return false;
}

//-----------------------------------------------------------------------------

void CreateNormalFile(const char *file_name, int commands_count, int *output_arr) {

    assert(file_name);

    FILE *output_file = fopen(file_name, "w");

    fprintf(output_file, "SPU VERSION: %d\n", output_arr[0]);
    fprintf(output_file, "FILE SIGNATURE: %d\n\n", output_arr[1]);

    for (int curr_num = SIZE_OF_HEADER_IN_BYTES;
         curr_num < commands_count + SIZE_OF_HEADER_IN_BYTES; curr_num++) {

        if (CheckIfCommandHaveArgument(output_arr[curr_num]))

            fprintf(output_file, "%d ", output_arr[curr_num++]);

        fprintf(output_file, "%d\n", output_arr[curr_num]);
    }

    fclose(output_file);
}

//=============================================================================

int CountStringHashDJB2DJB2(const char *curr_string) {

    int hash = 0, curr_num++;

    while (curr_string[curr_num] != '\0') {
        hash = (hash << 5) + curr_string[curr_num];

        curr_num++;
    }

    return hash;
}

//=============================================================================
