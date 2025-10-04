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
    JUMP    =   11,
    IN      =   12,
    HLT     =   0
};

enum Registrs {
    AX = 1,
    BX = 2,
    CX = 3
};

void CommandsCheck (void) {
    switch (PUSH) {
        case PUSH:
            break;

        case OUT:
            break;

        case ADD:
            break;

        case SUB:
            break;

        case MVL:
            break;

        case DIV:
            break;

        case POW:
            break;

        case SQRT:
            break;

        case HLT:
            break;

        default:
            break;
    }
}
/*switch (Commands) {
    case PUSH:
        break;

    case OUT:
        break;

    case ADD:
        break;

    case SUB:
        break;

    case MVL:
        break;

    case DIV:
        break;

    case POW:
        break;

    case SQRT:
        break;

    case HLT:
        break;

    default:
        break;
}
}*/

#endif // COMMANDS_H_INCLUDED
