#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "string_buffer.h"

const int DEFAULT_SIZE = 1024;

string_buffer_t* new_sbuf(int capacity)
{
    string_buffer_t* s = calloc(1, sizeof(*s));
    if (0 == capacity) {
        s->buffer = calloc(DEFAULT_SIZE, 1);
        s->capacity = DEFAULT_SIZE;
    } else {
        s->buffer = calloc(capacity, 1);
        s->capacity = capacity;
    }
    s->size = 0;
    return s;
}

void free_sbuf(string_buffer_t** s)
{
    string_buffer_t* sp = *s;
    free(sp->buffer);
    free(sp);
    s = NULL;
}

int copy_sbuf(string_buffer_t* sbuf, const char* s, size_t size)
{
    if (ensure_sbuf(sbuf, sbuf->size + size + 1))
        return -1;
    strncat(sbuf->buffer, s, size);
    sbuf->size += size + 1;
    return 0;
}

int extend_sbuf(string_buffer_t* s, int delta)
{
    char* tmp = realloc(s->buffer, s->capacity + delta);
    if (tmp) {
        s->buffer = tmp;
        return 0;
    } else {
        return -1;
    }
}

int shrink_sbuf(string_buffer_t* s)
{
    char* tmp = realloc(s->buffer, s->size + 1);
    if (tmp) {
        s->buffer = tmp;
        s->buffer[s->size] = 0;
        return 0;
    } else {
        return -1;
    }
}

int ensure_sbuf(string_buffer_t* s, int size)
{
    if (s->capacity < size) {
        int new_size = s->capacity + (size - s->capacity);
        return extend_sbuf(s, new_size);
    } else {
        return 0;
    }
}

void test_sbuf_file()
{
    const char* file = "/usr/share/dict/cracklib-small";
    char buf[256];
    
    int fd = open(file, O_RDONLY);

    int n;
    int total = 0;
    string_buffer_t* s = new_sbuf(0);
    while ((n = read(fd, buf, sizeof buf)) != 0) {
        if (n < 0) {
            if (errno == EINTR) continue;
            else break;
        } else {
            copy_sbuf(s, buf, n);
            total += n;
        }
    }

    close(fd);
    printf("%s", s->buffer);
    free_sbuf(&s);
}

int main()
{
    test_sbuf_file();
}
