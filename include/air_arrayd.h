// #define AIR_ARRAYD_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>

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

#define get_castD(T, ...) ((T[]){__VA_ARGS__})

#define SAFE_FREE(p) do { if (p) { free(p); (p) = NULL; } } while(0)
#define free_D(da) SAFE_FREE(da.data)

#ifdef AIR_ARRAYD_IMPLEMENTATION

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
    void *data;
    uint32 size;
    uint32 capacity;
} genericD;

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

floatD init_floatD(const size_t size, const float data[]);
intD init_intD(const size_t size, const int data[]);
stringD init_stringD(const size_t size, const char s[]);
void print_stringD(stringD da);
void print_floatD(floatD da);
void print_intD(intD da);

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

void print_stringD(stringD da) {
    (da).data[(da).size] = 0;
    printf("%s\n", (da).data);
}
void print_floatD(floatD da) {
    printf("[");
    for (uint32 i = 0; i < (da).size; i++) {
        printf("%f", (da).data[i]);
        if (i != (da).size - 1) printf(", ");
    }
    printf("]\n");
}
void print_intD(intD da) {
    printf("[");
    for (uint32 i = 0; i < (da).size; i++) {
        printf("%d", (da).data[i]);
        if (i != (da).size - 1) printf(", ");
    }
    printf("]\n");
}

#endif //AIR_ARRAYD_IMPLEMENTATION
