#ifndef _ANIME_IS_REAL__LIB_
#define _ANIME_IS_REAL__LIB_
#include <stdarg.h>
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

//macro with the dynamic array struct definition (since all of them have the same format)
#define __DA_STRUCT_DEFINITION(Type) \
    {Type *data; size_t size; size_t capacity;}

typedef struct __DA_STRUCT_DEFINITION(char) String;
typedef struct __DA_STRUCT_DEFINITION(String) String_Vec;

typedef struct __DA_STRUCT_DEFINITION(i8)  Vec_i8;
typedef struct __DA_STRUCT_DEFINITION(i16) Vec_i16;
typedef struct __DA_STRUCT_DEFINITION(i32) Vec_i32;
typedef struct __DA_STRUCT_DEFINITION(i64) Vec_i64;

typedef struct __DA_STRUCT_DEFINITION(u8)  Vec_u8;
typedef struct __DA_STRUCT_DEFINITION(u16) Vec_u16;
typedef struct __DA_STRUCT_DEFINITION(u32) Vec_u32;
typedef struct __DA_STRUCT_DEFINITION(u64) Vec_u64;

typedef struct __DA_STRUCT_DEFINITION(f32) Vec_f32;
typedef struct __DA_STRUCT_DEFINITION(f64) Vec_f64;

#define Vec(type) Vec_##type
#define _New(self, ...) __NEW_UNCONVENTIONAL(self, __VA_ARGS__)

#define New_Vec(type, ...) \
    ({Vec(type) __self = {0}; __NEW(__self, __VA_ARGS__); __self;})

//note: we decrease the size of the string by 1 because the '\0'
//gets counted as a character when initializing with __NEW()
#define New_String(...) \
    ({String __self = {0}; __NEW(__self, __VA_ARGS__); __self.size--; __self;})

#define Print_String(self) __STRING_PRINT(self)
#define Print_Vec(self) __VEC_PRINT(self)


#define __Is_String_T(T) _Generic(T, String: 1, default: 0)

//VEC TYPES
#define __Vec_T(Vec_var)      \
    _Generic(Vec_var,         \
        Vec_i8:     "%d",     \
        Vec_i16:    "%d",     \
        Vec_i32:    "%d",     \
        Vec_i64:    "%ld",    \
                              \
        Vec_u8 :    "%u",     \
        Vec_u16:    "%u",     \
        Vec_u32:    "%u",     \
        Vec_u64:    "%lu",    \
                              \
        Vec_f32:    "%f",     \
        Vec_f64:    "%lf",    \
        String_Vec: "s",      \
        default:    "0"       \
    )

#define __STRING_PRINT(String_var) \
    do { \
        if (__Is_String_T(String_var) == 0) { \
            perror("[ERROR] Failed to __STRING_PRINT! Value passed is not a String type"); \
            break; \
        } \
        if((String_var).size == 0) { \
            perror("[WARNING] Nothing to print, the size of the String value is 0!"); \
            break; \
        } \
        (String_var).data[(String_var).size] = '\0'; \
        printf("%s\n", (String_var).data); \
    } while(0) //might be better to have this just be a function


#define __VEC_PRINT(Vec_var) \
    do { \
        if (__Vec_T(Vec_var)[0] == '0') { \
            perror("[ERROR] Failed to __VEC_PRINT! Value passed is not a Vec_<T> type"); \
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
#define free_D(self) SAFE_FREE(self.data)


#ifdef AIR_ARRAYD_IMPLEMENTATION

// TODO: add checks for NULL
#define appendD(self, n) \
    do { \
        if ((self).size >= (self).capacity) { \
            if ((self).capacity == 0) (self).capacity = 64; \
            else (self).capacity *= 2; \
            (self).data = realloc((self).data, (self).capacity * sizeof(*(self).data)); \
        } \
        (self).data[(self).size++] = (n); \
    } while(0)


#define concatD(self, other) \
    do { \
        for (size_t i = 0; i < (other).size; i++) { \
            appendD((self), (other).data[i]); \
        } \
    } while(0)


#define popD(self, i) \
    do { \
        size_t index = i; \
        if (index < 0) index = (self).size + index; \
        if (index < 0 || index >= (self).size) { \
            printf(">>ACTION STOP: Pop failed: Index is out of bounds.\n"); \
            break; \
        } \
        if (index == (self).size - 1) { \
            (self).size--; \
            (self).data[(self).size] = 0; \
            break; \
        } \
        for (size_t j = index; j < (self).size - 1; j++) { \
            (self).data[j] = (self).data[j + 1]; \
            (self).size--; \
            (self).data[(self).size] = 0; \
        } \
    } while(0)


#define __NEW_UNCONVENTIONAL(self, ...) self = {0}; \
    do { \
        size_t __nsize = \
            sizeof(get_castD(__typeof__((self).data[0]), __VA_ARGS__)) / \
            sizeof(__typeof__((self).data[0])); \
        __typeof__((self).data[0]) __ndata[] = {__VA_ARGS__}; \
        for (size_t i = 0; i < __nsize; i++) { \
            appendD((self), __ndata[i]); \
        } \
    } while(0)

#define __NEW(self, ...) \
    do { \
        size_t __nsize = \
            sizeof(get_castD(__typeof__((self).data[0]), __VA_ARGS__)) / \
            sizeof(__typeof__((self).data[0])); \
        __typeof__((self).data[0]) __ndata[] = {__VA_ARGS__}; \
        for (size_t i = 0; i < __nsize; i++) { \
            appendD((self), __ndata[i]); \
        } \
    } while(0)

// #define initD(self, ...) \
//     _Generic((self), \
//         Vec_f32: init_floatD, \
//         String: init_stringD, \
//         Vec_i32: init_intD \
//         /* Add more types here */ \
//     )( \
//         sizeof(get_castD(typeof((self).data[0]), __VA_ARGS__))/sizeof(typeof((self).data[0])), \
//         get_castD(typeof((self).data[0]), __VA_ARGS__) \
//     )


#endif //AIR_ARRAYD_IMPLEMENTATION

int main(void)
{
    String test = New_String("test");
    appendD(test, 75);
    appendD(test, 65);
    Print_String(test);
    concatD(test, New_String(" another test"));
    Print_String(test);

    Vec_f32 test_f32 = New_Vec(f32, 2, 4, 8);
    appendD(test_f32, 10);
    Print_Vec(test_f32);
    Vec_i32 test_i32 = New_Vec(i32, 4, 6, 7);

    Print_Vec(test_i32);
}

#endif //_ANIME_IS_REAL__LIB_
