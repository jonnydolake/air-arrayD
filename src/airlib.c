#ifndef _ANIME_IS_REAL__LIB_
#define _ANIME_IS_REAL__LIB_
#include <stddef.h>
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

#define __STRUCT_T_DEFINITION(T) { \
    T *data; \
    size_t size; \
    size_t capacity; \
}

typedef struct __STRUCT_T_DEFINITION(char) String;
typedef struct __STRUCT_T_DEFINITION(String) String_Vec;

typedef struct __STRUCT_T_DEFINITION(i8) Vec_i8;
typedef struct __STRUCT_T_DEFINITION(i16) Vec_i16;
typedef struct __STRUCT_T_DEFINITION(i32) Vec_i32;
typedef struct __STRUCT_T_DEFINITION(i64) Vec_i64;

typedef struct __STRUCT_T_DEFINITION(u8) Vec_u8;
typedef struct __STRUCT_T_DEFINITION(u16) Vec_u16;
typedef struct __STRUCT_T_DEFINITION(u32) Vec_u32;
typedef struct __STRUCT_T_DEFINITION(u64) Vec_u64;

typedef struct __STRUCT_T_DEFINITION(f32) Vec_f32;
typedef struct __STRUCT_T_DEFINITION(f64) Vec_f64;


#define __String_T(T) \
    _Generic(T, \
        String: 1, \
        default: 0 \
    )

#define String_Print(String_var) \
do { \
    if (__String_T(String_var) == 0) { \
        perror("[ERROR] Failed to String_Print! Value passed is not a String type"); \
        break; \
    } \
    if((String_var).size == 0) { \
        perror("[WARNING] Nothing to print, the size of the String value is 0!"); \
        break; \
    } \
    (String_var).data[(String_var).size] = 0; \
    printf("%s\n", (String_var).data); \
} while(0) //might be better to have this just be a function


//VEC TYPES
#define __Vec_T(Vec_var)        \
    _Generic(Vec_var,       \
        Vec_i8 :    "%d",     \
        Vec_i16 :   "%d",     \
        Vec_i32 :   "%d",     \
        Vec_i64 :   "%ld",    \
        Vec_u8 :    "%u",     \
        Vec_u16 :   "%u",     \
        Vec_u32 :   "%u",     \
        Vec_u64 :   "%lu",    \
        Vec_f32 :   "%f",     \
        Vec_f64 :   "%lf",    \
        String_Vec: "s",      \
        default :   "0"       \
    )

#define Vec_Print(Vec_var) \
do { \
    if (__Vec_T(Vec_var)[0] == '0') { \
        perror("[ERROR] Failed to Vec_Print! Value passed is not a Vec_<T> type"); \
        break;} \
    printf("["); \
    for (size_t i = 0; i < (Vec_var).size; i++) { \
        printf(__Vec_T(Vec_var), (Vec_var).data[i]); \
        if (i != (Vec_var).size - 1) {printf(", ");} \
    } \
    printf("]\n"); \
} while(0)


// #define cast_Vec(T) _Generic((T), \
//         char* : , \
//         int : "Integer", \
//         long : "Long Integer", \
//         default : "Others")

#define get_castD(T, ...) ((T[]){__VA_ARGS__})

#define SAFE_FREE(p) do { if (p) { free(p); (p) = NULL; } } while(0)
#define free_D(da) SAFE_FREE(da.data)


#ifdef AIR_ARRAYD_IMPLEMENTATION

static inline Vec_f32 init_floatD(const size_t size, const f32 data[]);
static inline Vec_i32 init_intD(const size_t size, const i32 data[]);
static inline String init_stringD(const size_t size, const char s[]);

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
        Vec_f32: init_floatD, \
        String: init_stringD, \
        Vec_i32: init_intD \
        /* Add more types here */ \
    )( \
        sizeof(get_castD(typeof((da).data[0]), __VA_ARGS__))/sizeof(typeof((da).data[0])), \
        get_castD(typeof((da).data[0]), __VA_ARGS__) \
    )

#define printD(da) \
    _Generic((da), \
        String: print_stringD, \
        Vec_f32: print_floatD, \
        Vec_i32: print_intD \
    )(da)

static inline Vec_f32 init_floatD(const size_t size, const f32 data[]) {
    Vec_f32 da = {0};
    for (size_t i = 0; i < size; i++) {
        appendD(da, data[i]);
    }
    return da;
}
static inline Vec_i32 init_intD(const size_t size, const i32 data[]) {
    Vec_i32 da = {0};
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

// static inline void String_Print(String str) {
//     str.data[str.size] = 0;
//     printf("%s\n", str.data);
// }

#endif //AIR_ARRAYD_IMPLEMENTATION

int main(void)
{
    String test = initD(test, "test");
    String_Print(test);

    Vec_f32 test_f32 = initD(test_f32, 4, 6, 7);
    Vec_Print(test_f32);
    Vec_i32 test_i32 = initD(test_i32, 4, 6, 7);
    Vec_Print(test_i32);
}

#endif //_ANIME_IS_REAL__LIB_
