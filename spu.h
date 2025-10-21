#ifndef SPU_H_INCLUDED
#define SPU_H_INCLUDED

#include "lib/stack/stack.h"
#include "../include/commands.h"

//#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

//=============================================================================

#define Break_If_Spu_Error(spu)                                         \
    {                                                                   \
        int error = SpuVerify(spu);                                     \
        if (error != 0) {                                               \
            SpuDump(spu, __FILE__, __FUNCTION__, __LINE__);             \
            break;                                                      \
        }                                                               \
    }

#define Return_If_Spu_Error(spu)                                        \
    {                                                                   \
        int error = SpuVerify(spu);                                     \
        if (error != 0) {                                               \
            SpuDump(spu, __FILE__, __FUNCTION__, __LINE__);             \
            return error;                                               \
        }                                                               \
    }

#define Return_Spu_Error(spu)                                           \
    {                                                                   \
        int error = SpuVerify(spu);                                     \
        if (error != 0)                                                 \
            SpuDump(spu, __FILE__, __FUNCTION__, __LINE__);             \
        return error;                                                   \
    }

//-----------------------------------------------------------------------------

const size_t COMMAND_SIZE = sizeof(int);

const size_t RAM_DEFAULT_SIZE = 121;

//-----------------------------------------------------------------------------

struct SPU_t {

    Stack_t stk;
    Stack_t ret_addr;
    int *RAM;
    int ram_size;
    int *code;
    int commands_count;
    int counter;
    int registers[10];
    int version;
    int signature;
};

enum SpuErr_t {

    No_Spu_Error        = 0,
    Stack_Error         = 1 << 0,
    Code_Error          = 1 << 1,
    Counter_Error       = 1 << 2,
    Spu_Pointer_Error   = 1 << 3,
    Spu_Size_Error      = 1 << 4,
    Spu_Version_Error   = 1 << 5,
    Spu_Signature_Error = 1 << 6,
    Spu_Command_Error   = 1 << 7
};

struct Command_Function_t {

    int     command_num;
    int     (*command_function_ptr) (SPU_t *spu, int command);
};

//-----------------------------------------------------------------------------

int SpuCtor(const char* file_name, SPU_t *spu);
int CodeCtor(SPU_t *spu, size_t file_size, FILE *file_ptr);
int RAMCtor(SPU_t *spu);

void SpuDtor(SPU_t *spu);

int CheckVersionAndSignature(SPU_t *spu, FILE *file_ptr);

//-----------------------------------------------------------------------------

int RunBinaryMathOperation(SPU_t *spu, int command);
int RunUnaryMathOperation(SPU_t *spu, int command);

int RunJumpWithCondition(SPU_t *spu, int command);
int RunJumpWhitoutCondition(SPU_t *spu, int command);

int RunPushRegister(SPU_t *spu, int command);
int RunPopRegister(SPU_t *spu, int command);

int RunOut(SPU_t *spu, int command);
int RunIn(SPU_t *spu, int command);

int RunCall(SPU_t *spu, int command);
int RunRetern(SPU_t *spu, int command);

int RunPushMemory(SPU_t *spu, int command);
int RunPopMemory(SPU_t *spu, int command);

int RunDraw(SPU_t *spu, int command);

int RunPush(SPU_t *spu, int command);

//-----------------------------------------------------------------------------

int RunCurrentCommand(SPU_t *spu, int curr_command);

int SpuRun(SPU_t *spu);

//-----------------------------------------------------------------------------

int SpuVerify(SPU_t *spu);
void SpuDump(SPU_t *spu, const char *file_name, const char *function_name, int line_number);

//-----------------------------------------------------------------------------

const Command_Function_t Commands_fuctions_array[COMMANDS_COUNT] = {

    {HALT_COMMAND,              NULL                      },
    {PUSH_COMMAND,              RunPush                   },
    {OUTPUT_COMMAND,            RunOut                    },
    {ADDICTION_COMMAND,         RunBinaryMathOperation    },
    {SUBTRACTION_COMMAND,       RunBinaryMathOperation    },
    {MULTIPLICATION_COMMAND,    RunBinaryMathOperation    },
    {DIVISION_COMMAND,          RunBinaryMathOperation    },
    {POW_COMMAND,               RunBinaryMathOperation    },
    {SQUARE_ROOT_COMMAND,       RunUnaryMathOperation     },
    {PUSH_REGISTER_COMMAND,     RunPushRegister           },
    {POP_REGISTER_COMMAND,      RunPopRegister            },
    {JUMP_COMMAND,              RunJumpWhitoutCondition   },
    {IN_COMMAND,                RunIn                     },
    {JUMP_BELOW_COMMAND,        RunJumpWithCondition      },
    {JUMP_BELOW_EQUAL_COMMAND,  RunJumpWithCondition      },
    {JUMP_ABOVE_COMMAND,        RunJumpWithCondition      },
    {JUMP_ABOVE_EQUAL_COMMAND,  RunJumpWithCondition      },
    {JUMP_EQUAL_COMMAND,        RunJumpWithCondition      },
    {JUMP_NOT_EQUAL_COMMAND,    RunJumpWithCondition      },
    {CALL_FUNCTION_COMMAND,     RunCall                   },
    {RETERN_COMMAND,            RunRetern                 },
    {PUSH_MEMORY_COMMAND,       RunPushMemory             },
    {POP_MEMORY_COMMAND,        RunPopMemory              },
    {DRAW_COMMAND,              RunDraw                   }

};

// ���� ������ ����� ������������ �����, ��� � ��������� ��� ������� � ��������

//=============================================================================

#endif // SPU_H_INCLUDED
