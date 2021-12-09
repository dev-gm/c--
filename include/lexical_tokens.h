#include <stdint.h>
#include <string.h>

struct LexicalToken {
    union {
        struct {
            struct Type type;
            char *name;
        } variable_declare; // returns value
        struct {
            char *name; // must already have been declared. int a = 0 becomes variable_declare { type: int, name: "a" }; variable_assign { name: "a", value:  }
            union {
                struct LexicalToken value;
                struct {
                    struct LexicalToken *instructions;
                    size_t instructions_len;
                } instructions;
            } value;
        } variable_assign; // returns value
        struct {
            struct Function function;
            void **args;
        } function_call; // ((arg1, arg2) => {arg1 + arg2})(arg1, arg2)
        char *assembly_call;
        char *variable;
        char char;
        int8_t int8;
        int16_t int16;
        int32_t int32;
        int64_t int64;
        uint8_t uint8;
        uint16_t uint16;
        uint32_t uint32;
        uint64_t uint64;
        float float;
        double double;
    } token;
    enum {
        VARIABLE_DECLARE; // global
        VARIABLE_ASSIGN; // global
        FUNCTION_CALL;
        ASSEMBLY_CALL;
        VARIABLE;
        CHAR;
        INT8;
        INT16;
        INT32;
        INT64;
        UINT8;
        UINT16;
        UINT32;
        UINT64;
        FLOAT;
        DOUBLE;
    } type;
};

LexicalToken *lextoken_get_from_str(const char *input);

struct Function {
    struct Type output;
    struct {
        char *name;
        struct Type type;
    } *args;
    size_t args_len;
};

struct Type {
    union {
        struct {
            struct {
                struct Type type;
                char *name;
            } *members;
            size_t members_len;
        } struct;
        struct {
            struct {
                int value;
                char *name;
            } *constants;
            size_t constants_len;
        } enum;
        struct {
            struct {
                struct Type type;
                char *name;
            } *variants;
            size_t constants_len;
        } union;
        Function function;
    } value;
    enum {
        STRUCT;
        TRAIT;
        ENUM;
        UNION;
        FUNCTION;
    } type;
};
