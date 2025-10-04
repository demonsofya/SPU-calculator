#ifndef SPU_H_INCLUDED
#define SPU_H_INCLUDED

#include "stack.h"


//=============================================================================


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

struct SPU_t {
    Stack_t stk;
    int *code;
    int commands_count;
    int counter;
    int AX;
    int BX;
    int CX;
};

enum SpuErr_t {
    No_Spu_Error        = 0,
    Stack_Error         = 1 << 0,
    Code_Error          = 1 << 1,
    Counter_Error       = 1 << 2,
    Spu_Pointer_Error   = 1 << 3,
    Spu_Size_Error      = 1 << 4
};

void SpuCtor(const char* file_name, SPU_t *spu);
void SpuDtor(SPU_t *spu);

#endif // SPU_H_INCLUDED
