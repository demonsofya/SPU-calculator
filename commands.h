#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

struct Header_Info_t {

    int version;
    int signature;
};

const Header_Info_t DEFAULT_INFORMATION = {6, 451};

const int SIZE_OF_HEADER_IN_BYTES = sizeof(DEFAULT_INFORMATION) / sizeof(int);

struct Command_t {

    char    command_name[10];
    int     command_number;
    int     agrument_type;
    int     hash;
}; // массив структур

enum Commands {

    HALT_COMMAND                =   0,
    PUSH_COMMAND                =   1,
    OUTPUT_COMMAND              =   2,
    ADDICTION_COMMAND           =   3,
    SUBTRACTION_COMMAND         =   4,
    MULTIPLICATION_COMMAND      =   5,
    DIVISION_COMMAND            =   6,
    POW_COMMAND                 =   7,
    SQUARE_ROOT_COMMAND         =   8,
    PUSH_REGISTER_COMMAND       =   9,
    POP_REGISTER_COMMAND        =   10,
    JUMP_COMMAND                =   11,
    IN_COMMAND                  =   12,
    JUMP_BELOW_COMMAND          =   13,  // Если строго меньше
    JUMP_BELOW_EQUAL_COMMAND    =   14,  // меньше
    JUMP_ABOVE_COMMAND          =   15,  // строго Больше
    JUMP_ABOVE_EQUAL_COMMAND    =   16,  //  больше
    JUMP_EQUAL_COMMAND          =   17,  // Если равно
    JUMP_NOT_EQUAL_COMMAND      =   18,  // Если неравно
    CALL_FUNCTION_COMMAND       =   19,
    RETERN_COMMAND              =   20,
    PUSH_MEMORY_COMMAND         =   21,
    POP_MEMORY_COMMAND          =   22,
    DRAW_COMMAND                =   23,
    COMMANDS_COUNT              =   24
};

enum Commans_arguments_types {

    NO_ARGUMENT         =   0,
    NUMBER_ARGUMENT     =   1,
    REGISTER_ARGUMENT   =   2,
    RAM_ARGUMENT        =   3
};


/*
enum DEFAULT_INFORMATION {
    Version     = 6,
    Constant    = 451
};
*/

struct Register_t {

    char reg_name[10];
    int reg_num;
    int reg_hash;
};

enum Registers {

    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
    RV = 4,  // return value  -- for functions
    REGISTERS_COUNT = 5
};

static Register_t registers_array[REGISTERS_COUNT] = {

    {"AX", AX},
    {"BX", BX},
    {"CX", CX},
    {"DX", DX},
    {"RV", RV}
};

#endif // COMMANDS_H_INCLUDED
