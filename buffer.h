#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

struct StringsInfo {

    char *string_ptr;
    int string_size;
};

size_t GetFileSize(const char *file_name);

struct StringsInfo *GetStruct_PtrStingsArray(char *buffer, size_t text_size);
char *GetBuffer(const char *file_name, size_t *file_size);

int StringsCount(char *buffer);
char **GetStringPtrArray(char *buffer, size_t text_size);

#endif // BUFFER_H_INCLUDED
