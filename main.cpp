#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <assert.h>

#include "buffer.h"
#include "stack.h"
#include "../include/commands.h"
#include "spu.h"

int main() {

    SPU_t spu = {};
    int error = SpuCtor("task_bin.txt", &spu);
    Return_If_Spu_Error(&spu)

//-----------------------------------------------------------------------------
SpuDump(&spu, __FILE__, __FUNCTION__, __LINE__);
//-----------------------------------------------------------------------------

    SpuRun(&spu);

    SpuDtor(&spu);

    return 0;
}
