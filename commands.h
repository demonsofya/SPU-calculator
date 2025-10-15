#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

struct Command_t {
    char name[10] = "";
    int number;
} // массив структур

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
    {"CALL",    CALL    },
    {"RET",     RET     },
    {"PUSHM",   PUSHM   },
    {"POPM",    POPM    }

}

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
    JB              =   13,  // Если меньше
    JBE             =   14,  // Строго меньше
    JA              =   15,  // Больше
    JAE             =   16,  // Строго больше
    JE              =   17,  // Если равно
    JNE             =   18,  // Если неравно
    CALL            =   19,
    RET             =   20,
    PUSHM           =   21,
    POPM            =   22,
    COMMANDS_COUNT  =   23
};

enum Information {
    Version     = 6,
    Constant    = 451
};

enum Registrs {
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4,
    RV = 5  // return value  -- for functions
};

#endif // COMMANDS_H_INCLUDED
