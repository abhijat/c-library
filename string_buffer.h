#ifndef STRING_BUFFER_
#define STRING_BUFFER_

typedef struct
{
    char* buffer;
    size_t size;
    size_t capacity;
} string_buffer_t;

extern const int DEFAULT_SIZE;

string_buffer_t* new_sbuf(int capacity);
void free_sbuf(string_buffer_t** s);
int copy_sbuf(string_buffer_t* sbuf, const char* s, size_t size);
int extend_sbuf(string_buffer_t* s, int delta);
int shrink_sbuf(string_buffer_t* s);
int ensure_sbuf(string_buffer_t* s, int size);

#endif
