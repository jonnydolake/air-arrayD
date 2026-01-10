#ifndef _AIR_LIB_
#define _AIR_LIB_
#define AIR_ARRAYD_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

/////////////////////////
// typedef struct Str String;
// typedef char* string;

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} String;

typedef struct {
    char **data;
    size_t size;
    size_t capacity;
} strVec;

typedef struct {
    f32 *data;
    size_t size;
    size_t capacity;
} f32Vec;

typedef struct {
    i32 *data;
    size_t size;
    size_t capacity;
} i32Vec;

typedef struct {
    i64 *data;
    size_t size;
    size_t capacity;
} i64Vec;

#define String_New(String_var, ...) initD(String_var, __VA_ARGS__)
#define String_Print(String_var) printD(String_var)

#define get_castD(T, ...) ((T[]){__VA_ARGS__})

#define SAFE_FREE(p) do { if (p) { free(p); (p) = NULL; } } while(0)
#define free_D(da) SAFE_FREE(da.data)


#ifdef AIR_ARRAYD_IMPLEMENTATION

static inline f32Vec init_floatD(const size_t size, const f32 data[]);
static inline i32Vec init_intD(const size_t size, const int data[]);
static inline String init_stringD(const size_t size, const char s[]);
static inline void print_stringD(String da);
static inline void print_floatD(f32Vec da);
static inline void print_intD(i32Vec da);

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
        for (u32 i = 0; i < (n).size; i++) { \
            appendD((da), (n).data[i]); \
        } \
    } while(0)

#define popD(da, i) \
    do { \
        i32 index = i; \
        if (index < 0) index = (da).size + index; \
        if (index < 0 || index >= (i32)(da).size) { \
            printf(">>ACTION STOP: Pop failed: Index is out of bounds.\n"); \
            break; \
        } \
        if (index == (i32)(da).size - 1) { \
            (da).size--; \
            (da).data[(da).size] = 0; \
            break; \
        } \
        for (i32 j = index; j < (i32)(da).size - 1; j++) { \
            (da).data[j] = (da).data[j + 1]; \
            (da).size--; \
            (da).data[(da).size] = 0; \
        } \
    } while(0)

#define initD(da, ...) \
    _Generic((da), \
        f32Vec: init_floatD, \
        String: init_stringD, \
        i32Vec: init_intD \
        /* Add more types here */ \
    )( \
        sizeof(get_castD(typeof((da).data[0]), __VA_ARGS__))/sizeof(typeof((da).data[0])), \
        get_castD(typeof((da).data[0]), __VA_ARGS__) \
    )

#define printD(da) \
    _Generic((da), \
        String: print_stringD, \
        f32Vec: print_floatD, \
        i32Vec: print_intD \
    )(da)

static inline f32Vec init_floatD(const size_t size, const f32 data[]) {
    f32Vec da = {0};
    for (size_t i = 0; i < size; i++) {
        appendD(da, data[i]);
    }
    return da;
}
static inline i32Vec init_intD(const size_t size, const int data[]) {
    i32Vec da = {0};
    for (size_t i = 0; i < size; i++) {
        appendD(da, data[i]);
    }
    return da;
}

static inline String init_stringD(const size_t size, const char s[])
{
    String da = {0};
    for (size_t i = 0; i < (size - 1) ; i++) {
        appendD(da, s[i]);
    }
    return da;
}

static inline void print_stringD(String da) {
    (da).data[(da).size] = 0;
    printf("%s\n", (da).data);
}
static inline void print_floatD(f32Vec da)
{
    printf("[");
    for (u32 i = 0; i < (da).size; i++) {
        printf("%f", (da).data[i]);
        if (i != (da).size - 1) printf(", ");
    }
    printf("]\n");
}
static inline void print_intD(i32Vec da)
{
    printf("[");
    for (u32 i = 0; i < (da).size; i++) {
        printf("%d", (da).data[i]);
        if (i != (da).size - 1) printf(", ");
    }
    printf("]\n");
}


#endif //AIR_ARRAYD_IMPLEMENTATION

#endif //_AIR_LIB_
