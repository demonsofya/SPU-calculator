#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

enum Commands {
    PUSH    =   1,
    OUT     =   2,
    ADD     =   3,
    SUB     =   4,
    MVL     =   5,
    DIV     =   6,
    POW     =   7,
    SQRT    =   8,
    PUSHREG =   9,
    POPREG  =   10,
    JUMP    =   20,
    IN      =   11,
    JB      =   30,  // Если меньше
    JBE     =   31,  // Строго меньше
    JA      =   40,  // Больше
    JAE     =   41,  // Строго больше
    JE      =   50,  // Если равно
    JNE     =   51,  // Если неравно
    HLT     =   0
};

enum Registrs {
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};

#endif // COMMANDS_H_INCLUDED
