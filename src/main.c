#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//setup stolen from Casey
#define internal static
#define local_persist static
#define global_variable static

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
/////////////////////////
// typedef struct Str stringD;
// typedef char* string;

typedef enum {
    DA_STRING,
} DA_type;

typedef struct {
    char *data;
    uint32 size;
    uint32 capacity;
} stringD;

typedef struct {
    char **data;
    uint32 size;
    uint32 capacity;
} string_listD;

typedef struct {
    float *data;
    uint32 size;
    uint32 capacity;
} floatD;

typedef struct {
    int *data;
    uint32 size;
    uint32 capacity;
} intD;


static inline void print_stringD(stringD da) {
    (da).data[(da).size] = 0;
    printf("%s\n", (da).data);
}
static inline void print_floatD(floatD da) {
    printf("[");
    for (uint32 i = 0; i < (da).size; i++) {
        printf("%f", (da).data[i]);
        if (i != (da).size - 1) printf(", ");
    }
    printf("]\n");
}
static inline void print_intD(intD da) {
    printf("[");
    for (uint32 i = 0; i < (da).size; i++) {
        printf("%d", (da).data[i]);
        if (i != (da).size - 1) printf(", ");
    }
    printf("]\n");
}


#define appendD(da, n) \
do { \
    if ((da).size >= (da).capacity) { \
        if ((da).capacity == 0) (da).capacity = 64; \
        else (da).capacity *= 2; \
        (da).data = realloc((da).data, (da).capacity * sizeof(*(da).data)); \
    } \
    da.data[(da).size++] = (n); \
} while(0)

#define concatD(da, n) \
    do { \
        for (uint32 i = 0; i < (n).size; i++) { \
            appendD((da), (n).data[i]); \
        } \
    } while(0)

#define popD(da, i) \
    do { \
        int32 index = i; \
        if (index < 0) index = (da).size + index; \
        if (index < 0 || index >= (int32)(da).size) { \
            printf(">>ACTION STOP: Pop failed: Index is out of bounds.\n"); \
            break; \
        } \
        if (index == (int32)(da).size - 1) { \
            (da).size--; \
            (da).data[(da).size] = 0; \
            break; \
        } \
        for (int32 j = index; j < (int32)(da).size - 1; j++) { \
            (da).data[j] = (da).data[j + 1]; \
            (da).size--; \
            (da).data[(da).size] = 0; \
        } \
    } while(0)


floatD init_floatD(const size_t size, const float data[]) {
    floatD da = {0};
    for (size_t i = 0; i < size; i++) {
        appendD(da, data[i]);
    }
    return da;
}
intD init_intD(const size_t size, const int data[]) {
    intD da = {0};
    for (size_t i = 0; i < size; i++) {
        appendD(da, data[i]);
    }
    return da;
}
stringD init_stringD(const size_t size, const char s[])
{
    stringD da = {0};
    for (size_t i = 0; i < (size - 1) ; i++) {
        appendD(da, s[i]);
    }
    return da;
}

#define get_castD(T, ...) ((T[]){__VA_ARGS__})

#define initD(da, ...) \
    _Generic((da), \
        floatD: init_floatD, \
        stringD: init_stringD, \
        intD: init_intD \
        /* Add more types here */ \
    )( \
        sizeof(get_castD(typeof((da).data[0]), __VA_ARGS__))/sizeof(typeof((da).data[0])), \
        get_castD(typeof((da).data[0]), __VA_ARGS__) \
    )

#define printD(da) \
    _Generic((da), \
        stringD: print_stringD, \
        floatD: print_floatD, \
        intD: print_intD \
    )(da)


int main(void)
{
    stringD test = initD(test, "tes");
    stringD test2 = initD(test2, "ting");
    // float arr1[] = {2,4,5,67,8};
    stringD test_float = initD(test_float, 65,66,67,68,99,0);
    stringD test_float2 = initD(test_float, 76,88,71);
    // makeD(test_float, 4,5,6,7);
    appendD(test, 't');
    printD(test);
    popD(test, -1);
    // s_append(&test, "test2\n");
    printD(test);
    concatD(test, test2);

    char bar[] = {"hello"};
    printf("%s\n", bar);
    printD(test);
    printD(test_float);
    popD(test_float, -1);
    printD(test_float);
    concatD(test_float, test_float2);
    printD(test_float);

    return 0;
}

stringD init_String(const char *s)
{
    stringD da = {0};
    for (int i = 0; s[i] != '\0'; i++) {
        appendD(da, s[i]);
    }
    return da;
}

void s_append(stringD *s, char c)
{
    if (s->size >= s->capacity) {
        if (s->capacity == 0) s->capacity = 64;
        else s->capacity *= 2;
        s->data = realloc(s->data, s->capacity*sizeof(*s->data));
    }
    s->data[s->size++] = c;
}
