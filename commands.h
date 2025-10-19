#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

const int INFORMATION_MEMORY_COUNT = 2;

struct Command_t {
    char    command_name[10];
    int     command_number;
}; // массив структур

enum Commands {
    HLT             =   0,
    PUSH            =   1,
    OUT             =   2,
    ADD             =   3,
    SUB             =   4,
    MUL             =   5,
    DIV             =   6,
    POW             =   7,
    SQRT            =   8,
    PUSHREG         =   9,
    POPREG          =   10,
    JUMP            =   11,
    IN              =   12,
    JB              =   13,  // Если строго меньше
    JBE             =   14,  // меньше
    JA              =   15,  // строго Больше
    JAE             =   16,  //  больше
    JE              =   17,  // Если равно
    JNE             =   18,  // Если неравно
    CALL            =   19,
    RET             =   20,
    PUSHM           =   21,
    POPM            =   22,
    DRAW            =   23,
    COMMANDS_COUNT  =   24
};

const Command_t commands_array[COMMANDS_COUNT] = {

    {"HLT",     HLT     },
    {"PUSH",    PUSH    },
    {"OUT",     OUT     },
    {"ADD",     ADD     },
    {"SUB",     SUB     },
    {"MUL",     MUL     },
    {"DIV",     DIV     },
    {"POW",     POW     },
    {"SQRT",    SQRT    },
    {"PUSHREG", PUSHREG },
    {"POPREG",  POPREG  },
    {"JUMP",    JUMP    },
    {"IN",      IN      },
    {"JB",      JB      },
    {"JBE",     JBE     },
    {"JA",      JA      },
    {"JAE",     JAE     },
    {"JE",      JE      },
    {"JNE",     JNE     },
    {"CALL",    CALL    },
    {"RET",     RET     },
    {"PUSHM",   PUSHM   },
    {"POPM",    POPM    },
    {"DRAW",    DRAW    }

};

enum Information {
    Version     = 6,
    Constant    = 451
};

struct Register_t {
    char reg_name[3];
    int reg_num;
};

enum Registers {
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
    RV = 4,  // return value  -- for functions
    REGISTERS_COUNT = 5
};

const Register_t registers_array[REGISTERS_COUNT] = {
    {"AX", AX},
    {"BX", BX},
    {"CX", CX},
    {"DX", DX},
    {"RV", RV}
};

#endif // COMMANDS_H_INCLUDED
