#include "lexical_tokens.h"
#include "linked_list.h"
#include <stdio.h>
// #define $+$ # asm {addl $1$ $2$};
// #define $+=$ # $1$=$1$+$2$; // can extend with curly brackets
// #define ${}$ # asm {mul $1$ $2$};
// #define var int

char *read_file(const char *filename);

struct MacroOutput {
    char *value; // NULL if no space between values
    size_t index; // index>=1. no index if index<1
}

struct Macro {
    char **alias; // for $+-$-+ you do alias={"+-", "-+"}, vars_len=2
    size_t alias_len;
    size_t vars_len;
    bool var_at_start: 1; // if vars_len > (alias_len + var_at_start), var is at end
    struct MacroOutput *output; // output = "value1""index1""value2""index2"
    size_t output_len;
}

struct Macro *macro_from_raw(char *raw);

bool macro_is_defined(struct Macro *macro, char *alias);

// return NULL if not found
char *macro_matches(struct Macro *macro, char *input);

struct PreProcessorToken {
    union {
        struct Macro *define;
        char *ifdef;
        char *ifndef;
        void endif;
    } token;
    enum {
        DEFINE;
        IFDEF;
        IFNDEF;
        ENDIF;
    } type;
};

char *preprocessor_run(struct List *macros, char *raw);
