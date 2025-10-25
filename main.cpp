#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <assert.h>

#include "buffer.h"
#include "stack.h"
//#include "../include/commands.h"
#include "spu.h"

int main(int argc, char *argv[]) {

    SPU_t spu = {};

    if (argc > 1)
        SpuCtor(argv[1], &spu);
    else
        SpuCtor("task_bin.txt", &spu);

    Return_If_Spu_Error(&spu)

//-----------------------------------------------------------------------------
    ON_DEBUG(SpuDump(&spu, __FILE__, __FUNCTION__, __LINE__));
//-----------------------------------------------------------------------------

    SpuRun(&spu);

    SpuDtor(&spu);

    return 0;
}
