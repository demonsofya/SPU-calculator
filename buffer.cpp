#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <Txlib.h>

#include "buffer.h"

size_t GetFileSize(const char *file_name) {

    assert(file_name);

    struct stat file_info = {};
    stat(file_name, &file_info);

    return file_info.st_size;
}

char *GetBuffer(const char *file_name, size_t *file_size) {

    assert(file_name);
    assert(file_size);

    size_t file_full_size = GetFileSize(file_name);
    char *buffer = (char *) calloc((file_full_size + 1), sizeof(char));
    buffer[file_full_size] = '\0';

    FILE *file_ptr = fopen(file_name, "r");

    *file_size = fread(buffer, sizeof(char), file_full_size, file_ptr);

    if (buffer[*file_size - 1] != '\n')
        buffer[*file_size] = '\n';

    buffer[*file_size + 1] = '\0';

    fclose(file_ptr);

    return buffer;
}


// This one is working
int StringsCount(char *buffer) {

    assert(buffer);

    int counter = 0;
    int curr_element = 0;

    while (buffer[curr_element] != '\0') {

        if (buffer[curr_element] == '\n')
            counter++;

        curr_element++;
    }

    return counter;
}


// This one is working, checked
struct StringsInfo *GetStruct_PtrStingsArray(char *buffer, size_t text_size) {

    assert(buffer);

    StringsInfo* text = (StringsInfo *) calloc(text_size, sizeof(StringsInfo));

    assert(text);

    char *curr_pointer = buffer;
//FILE *debug_ptr = fopen("Debug.txt", "w");

    for (size_t curr_num = 0; curr_num < text_size; curr_num++) {


        text[curr_num].string_ptr = curr_pointer;

        curr_pointer = strchr(curr_pointer, '\n');
        curr_pointer++;

        text[curr_num].string_size = curr_pointer - text[curr_num].string_ptr - 1; // \n не считаю

//PrintStringInFile(debug_ptr, text[curr_num].string_ptr);

//printf("\n\nstring number %d is %s, size is %d\n", curr_num + 1, text[curr_num].string_ptr, text[curr_num].string_size   );
   }
//fclose(debug_ptr);

    return text;
}


//=============================================================================
//=============================================================================
// Это потому что для qsort'а в какой-то момент понадобился массив без структур

char **GetStringPtrArray(char *buffer, size_t text_size) {

    assert(buffer);

    char** text = (char **) calloc(text_size, sizeof(StringsInfo));

    assert(text);

    char *curr_pointer = buffer;

    for (size_t curr_num = 0; curr_num < text_size; curr_num++) {

        text[curr_num] = curr_pointer;

        curr_pointer = strchr(curr_pointer, '\n');
        curr_pointer++;

    }

    return text;
}

