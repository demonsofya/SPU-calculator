#ifndef SPU_H_INCLUDED
#define SPU_H_INCLUDED

#include "stack.h"
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
    Stack_t stk = {};
    Stack_t ret_addr = {};
    int *RAM    = NULL;
    int ram_size = 0;
    int *code   = NULL;
    int commands_count = 0;
    int counter = 0;
    int registers[10] = {};
    int version   = 6;
    int signature = 451;
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

int RunPushreg(SPU_t *spu, int command);
int RunPopreg(SPU_t *spu, int command);

int RunOut(SPU_t *spu, int command);
int RunIn(SPU_t *spu, int command);

int RunCall(SPU_t *spu, int command);
int RunRet(SPU_t *spu, int command);

int RunPushM(SPU_t *spu, int command);
int RunPopM(SPU_t *spu, int command);

int RunDraw(SPU_t *spu, int command);

int RunPush(SPU_t *spu, int command);

//-----------------------------------------------------------------------------

int RunCurrentCommand(SPU_t *spu, int curr_command);

int SpuRun(SPU_t *spu);

//-----------------------------------------------------------------------------

int SpuVerify(SPU_t *spu);
void SpuDump(SPU_t *spu, const char *file_name, const char *function_name, int line_number);

//-----------------------------------------------------------------------------

static Command_Function_t Commands_fuctions_array[COMMANDS_COUNT] = {

    {HLT,     NULL                      },
    {PUSH,    RunPush                   },
    {OUT,     RunOut                    },
    {ADD,     RunBinaryMathOperation    },
    {SUB,     RunBinaryMathOperation    },
    {MUL,     RunBinaryMathOperation    },
    {DIV,     RunBinaryMathOperation    },
    {POW,     RunBinaryMathOperation    },
    {SQRT,    RunUnaryMathOperation     },
    {PUSHREG, RunPushreg                },
    {POPREG,  RunPopreg                 },
    {JUMP,    RunJumpWhitoutCondition   },
    {IN,      RunIn                     },
    {JB,      RunJumpWithCondition      },
    {JBE,     RunJumpWithCondition      },
    {JA,      RunJumpWithCondition      },
    {JAE,     RunJumpWithCondition      },
    {JE,      RunJumpWithCondition      },
    {JNE,     RunJumpWithCondition      },
    {CALL,    RunCall                   },
    {RET,     RunRet                    },
    {PUSHM,   RunPushM                  },
    {POPM,    RunPopM                   },
    {DRAW,    RunDraw                   }

};

//=============================================================================

#endif // SPU_H_INCLUDED
