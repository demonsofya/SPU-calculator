#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED


//-----------------------------------------------------------------------------

#define NEW_COMMAND_ASM(name, number, arg_type, funt_ptr) {name, number, arg_type, CountStringHashDJB2(name), __FILE__, __LINE__, "{#name, #number, #arg_type, CountStringHashDJB2(name), __FILE__, __LINE__}" }
#define NEW_COMMAND_SPU(name, number, arg_type, funt_ptr) {number, func_ptr, __FILE__, __LINE__, "{#number, #func_ptr, __FILE__, __LINE__}" }

#ifdef COMPILE_ASM
    #define NEW_COMMAND NEW_COMMAND_ASM
#endif

#ifdef COMPILE_SPU
    #define NEW_COMMAND NEW_COMMAND_SPU
#endif


struct Header_Info_t {

    int version;
    int signature;
};

const Header_Info_t DEFAULT_INFORMATION = {6, 451};

const int SIZE_OF_HEADER_IN_BYTES = sizeof(DEFAULT_INFORMATION) / sizeof(int);

struct Command_t {
    #ifdef COMPILE_ASM
        char    command_name[10];
        int     command_number;
        int     agrument_type;
        int     hash;
    #endif

    #ifdef COMPILE_SPU
        int     command_number;
        int     (*command_function_ptr) (SPU_t *spu, int command);
    #endif
        const char *file_name_of_source_code;
        int         line_number_in_source_code;
        const char *line_content_in_source_code;
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


const Command_t commands_array[COMMANDS_COUNT] = {

    NEW_COMMAND("HLT",     HALT_COMMAND,               NO_ARGUMENT,         NULL                    ),
    NEW_COMMAND("PUSH",    PUSH_COMMAND,               NUMBER_ARGUMENT,     RunPush                 ),
    NEW_COMMAND("OUT",     OUTPUT_COMMAND,             NO_ARGUMENT,         RunOut                  ),
    NEW_COMMAND("ADD",     ADDICTION_COMMAND,          NO_ARGUMENT,         RunBinaryMathOperation  ),
    NEW_COMMAND("SUB",     SUBTRACTION_COMMAND,        NO_ARGUMENT,         RunBinaryMathOperation  ),
    NEW_COMMAND("MUL",     MULTIPLICATION_COMMAND,     NO_ARGUMENT,         RunBinaryMathOperation  ),
    NEW_COMMAND("DIV",     DIVISION_COMMAND,           NO_ARGUMENT,         RunBinaryMathOperation  ),
    NEW_COMMAND("POW",     POW_COMMAND,                NO_ARGUMENT,         RunBinaryMathOperation  ),
    NEW_COMMAND("SQRT",    SQUARE_ROOT_COMMAND,        NO_ARGUMENT,         RunUnaryMathOperation   ),
    NEW_COMMAND("PUSHREG", PUSH_REGISTER_COMMAND,      REGISTER_ARGUMENT,   RunPushRegister         ),
    NEW_COMMAND("POPREG",  POP_REGISTER_COMMAND,       REGISTER_ARGUMENT,   RunPopRegister          ),
    NEW_COMMAND("JUMP",    JUMP_COMMAND,               NUMBER_ARGUMENT,     RunJumpWhitoutCondition ),
    NEW_COMMAND("IN",      IN_COMMAND,                 NO_ARGUMENT,         RunIn                   ),
    NEW_COMMAND("JB",      JUMP_BELOW_COMMAND,         NUMBER_ARGUMENT,     RunJumpWithCondition    ),
    NEW_COMMAND("JBE",     JUMP_BELOW_EQUAL_COMMAND,   NUMBER_ARGUMENT,     RunJumpWithCondition    ),
    NEW_COMMAND("JA",      JUMP_ABOVE_COMMAND,         NUMBER_ARGUMENT,     RunJumpWithCondition    ),
    NEW_COMMAND("JAE",     JUMP_ABOVE_EQUAL_COMMAND,   NUMBER_ARGUMENT,     RunJumpWithCondition    ),
    NEW_COMMAND("JE",      JUMP_EQUAL_COMMAND,         NUMBER_ARGUMENT,     RunJumpWithCondition    ),
    NEW_COMMAND("JNE",     JUMP_NOT_EQUAL_COMMAND,     NUMBER_ARGUMENT,     RunJumpWithCondition    ),
    NEW_COMMAND("CALL",    CALL_FUNCTION_COMMAND,      NUMBER_ARGUMENT,     RunCall                 ),
    NEW_COMMAND("RET",     RETERN_COMMAND,             NO_ARGUMENT,         RunRetern               ),
    NEW_COMMAND("PUSHM",   PUSH_MEMORY_COMMAND,        RAM_ARGUMENT,        RunPushMemory           ),
    NEW_COMMAND("POPM",    POP_MEMORY_COMMAND,         RAM_ARGUMENT,        RunPopMemory            ),
    NEW_COMMAND("DRAW",    DRAW_COMMAND,               NO_ARGUMENT,         RunDraw                 )

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
