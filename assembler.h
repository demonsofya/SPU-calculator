#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include "commands.h"

//-----------------------------------------------------------------------------

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

const int LABELS_ARRAY_SIZE = 10;

//-----------------------------------------------------------------------------

char **GetStringsPtrArrayFromFile(const char *file_name, int *ptr_array_size);
void FillCurrentOpcode(int *output_arr, char *command_string, int *labels, int *command_num);

//-----------------------------------------------------------------------------

void FillCurrentOpcode(int *output_arr, char *command_string, int *labels,
                       int *command_num, char *argument_string, int *error);

//-----------------------------------------------------------------------------

void FillCommandWithNumberArgument(int *output_arr, char *command_string,
                                   int *labels, int *command_num, int *error);
int FillCodeArray(int *output_arr, int ptr_array_size, char **text_ptr_array, int *labels);
void FillCommandWithRegisterArgiment(int *output_arr, char *command_string,
                                     int *labels, int *command_num, int *error);
void FillCommandWithRAMArgiment(int *output_arr, char *command_string,
                                int *labels, int *command_num, int *error);
void FillLabel(char *command, int *labels, int command_num, int *counter, int *error);

//-----------------------------------------------------------------------------

void LabelsDump(int *labels);
void ErrorDump(int *error);

//-----------------------------------------------------------------------------

int *CreateCodeArray(const char *input_file_name, int *commands_count);

void CreateBinaryFile(const char *file_name, int commands_count, int *output_arr);
void CreateNormalFile(const char *file_name, int commands_count, int *output_arr);

//-----------------------------------------------------------------------------

bool CheckIfCommandHaveArgument(int command);
bool CheckIfCommandLabel(char *command);

int CheckRegister(char *reg, int *error);

//-----------------------------------------------------------------------------

int CountStringHashDJB2(const char *curr_string);
void CountHashTable();

//=============================================================================

enum AsmErrors {

    NO_ASM_ERROR                = 0,
    COMMAND_ASM_ERROR           = 1 << 0,
    REGISTER_ASM_ERROR          = 1 << 1,
    NUMBER_ARGUMENT_ASM_ERROR   = 1 << 2,
    LABELS_ASM_ERROR            = 1 << 3,
    RAM_ARGUMENT_ERROR          = 1 << 4,
    SINTAXYS_ASM_ERROR          = 1 << 5

};


//-----------------------------------------------------------------------------

struct ASM_t {

    int *output_arr;
    int command_num;
    int labels[LABELS_ARRAY_SIZE];
    char **ptrs_to_code_lines_array;
    int ptrs_to_code_lines_array_size;
    char *curr_command;
    int curr_command_num;
    int *curr_error;
}

//-----------------------------------------------------------------------------

static Command_t commands_array[COMMANDS_COUNT] = {

    {"HLT",     HALT_COMMAND,               NO_ARGUMENT,         },
    {"PUSH",    PUSH_COMMAND,               NUMBER_ARGUMENT,     },
    {"OUT",     OUTPUT_COMMAND,             NO_ARGUMENT,         },
    {"ADD",     ADDICTION_COMMAND,          NO_ARGUMENT,         },
    {"SUB",     SUBTRACTION_COMMAND,        NO_ARGUMENT,         },
    {"MUL",     MULTIPLICATION_COMMAND,     NO_ARGUMENT,         },
    {"DIV",     DIVISION_COMMAND,           NO_ARGUMENT,         },
    {"POW",     POW_COMMAND,                NO_ARGUMENT,         },
    {"SQRT",    SQUARE_ROOT_COMMAND,        NO_ARGUMENT,         },
    {"PUSHREG", PUSH_REGISTER_COMMAND,      REGISTER_ARGUMENT,   },
    {"POPREG",  POP_REGISTER_COMMAND,       REGISTER_ARGUMENT,   },
    {"JUMP",    JUMP_COMMAND,               NUMBER_ARGUMENT,     },
    {"IN",      IN_COMMAND,                 NO_ARGUMENT,         },
    {"JB",      JUMP_BELOW_COMMAND,         NUMBER_ARGUMENT,     },
    {"JBE",     JUMP_BELOW_EQUAL_COMMAND,   NUMBER_ARGUMENT,     },
    {"JA",      JUMP_ABOVE_COMMAND,         NUMBER_ARGUMENT,     },
    {"JAE",     JUMP_ABOVE_EQUAL_COMMAND,   NUMBER_ARGUMENT,     },
    {"JE",      JUMP_EQUAL_COMMAND,         NUMBER_ARGUMENT,     },
    {"JNE",     JUMP_NOT_EQUAL_COMMAND,     NUMBER_ARGUMENT,     },
    {"CALL",    CALL_FUNCTION_COMMAND,      NUMBER_ARGUMENT,     },
    {"RET",     RETERN_COMMAND,             NO_ARGUMENT,         },
    {"PUSHM",   PUSH_MEMORY_COMMAND,        RAM_ARGUMENT,        },
    {"POPM",    POP_MEMORY_COMMAND,         RAM_ARGUMENT,        },
    {"DRAW",    DRAW_COMMAND,               NO_ARGUMENT,         }

};

//-----------------------------------------------------------------------------

#endif // ASSEMBLER_H_INCLUDED
