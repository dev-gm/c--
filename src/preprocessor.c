#include <linked_list.h>
#include <preprocessor.h>
#include <limits.h>

// reads "filename" return contents. NULL if file doesn't exist
char *read_file(const char *filename) {
    char *out;
    long length;
    FILE *file = fopen(filename, "r");
    if (!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    out = malloc(length);
    if (out)
        fread(out, 1, length, file);
    fclose(file);
    return out;
}

struct Indices {
    bool *(indices: 1);
};

struct Indices indices_new(size_t len) {
    struct Indices out = {
        .indices = calloc((len / CHAR_BIT) + (len % CHAR_BIT ? 1 : 0), 1),
        .len = len,
    };
    return out;
}

// $+-$-+#$1$+$2$ // '#' is separator. cannot be used in a macro alias.
struct Macro *macro_from_raw(char *raw) {
    if (!raw)
        return NULL;
    struct Macro *macro = malloc(sizeof(struct Macro));
    char *separator, *prev, *next, out_buf;
    struct Indices var_indices;
    size_t index = 0, last = 0, i;
    bool recording_index;
    separator = strchr(raw, '#');
    if (!separator)
        return NULL;
    // counts how many items need to be in list of aliases
    macro->alias_len = 0;
    macro->vars_len = 0;
    var_indices = indices_new(separator - raw);
    for (next = strchr(raw, '$'); next && next < separator; next = strchr(next, '$')) {
        var_indices.indices[next - raw] = true;
        if (next == raw)
            macro->var_at_start = true;
        else
            ++macro->alias_len;
        ++macro->vars_len;
        next = strchr(next, '$');
    }
    // puts items into list of aliases
    macro->alias = calloc(macro->alias_len, sizeof(char *));
    for (i = 0; i < var_indices.len; ++i) {
        if (var_indices.indices[i]) {
            macro->alias[index] = memset(malloc(i - last), '\0');
            strncpy(macro->alias[index], raw + last, i - last - 1);
            last = i;
            ++index;
        }
    }
    // counts how many items need to be in list of outputs
    macro->output_len = 0;
    var_indices = indices_new(strlen(separator + 1));
    for (next = strchr(separator + 1, '$'); next; next = strchr(next, '$')) {
        var_indices.indices[next - (separator + 1)];
        prev = next;
        ++macro->output_len;
    }
    // puts items into list of outputs
    macro->output = calloc(macro->output_len, sizeof(struct MacroOutput));
    recording_index = false;
    for (i = 0, last = 0, index = 0; i < var_indices.len; ++i) {
        if (var_indices.indices[i]) {
            if (recording_index) {
                // set index - convert string in between '$' to int
                (separator + 1)[i] = '\0';
                macro->output[index].index = (separator + 1)[last] - "0";
                (separator + 1)[i] = '$';
                // set value
                macro->output[index].value = strcpy(memset(malloc(strlen(out_buf) + 1), '\0'), out_buf);
            } else {
                // copy value from string to macro output
                out_buf = strncpy(memset(malloc(i - last + 1), '\0'), (separator + 1)[last], i - last);
            }
            recording_index = !recording_index;
            last = i + 1;
            ++index;
        }
    }
    return macro;
}

bool macro_is_defined(struct Macro *macro, char *alias) {
    char *out, *current;
    size_t len = macro->vars_len, i;
    for (i = 0; i < macro->alias_len; ++i)
        len += strlen(macro->alias[i]);
    out = memset(malloc(len + 1), '\0');
    current = out;
    if (macro->var_at_start) {
        out[0] = '$';
        ++current;
    }
    for (i = 0; i < macro->alias_len; ++i) {
        strcpy(current, macro->alias[i]);
        current += strlen(macro->alias[i]);
        current++[0] = '$';
    }
    return strcmp(out, alias) == 0;
}

char *macro_matches(struct Macro *macro, const char *input) {
    size_t i, var_len, out_len;
    char (*vars)[macro->vars_len];
    char *start, *current = input, *instance, *out;
    // checks if the part before first variable matches macro
    if (!macro->var_at_start) {
        instance = strstr(current, macro->alias[0]);
        if (!instance) {
            return NULL;
        } else if (instance > current) {
            var_len = instance - current;
            start = memset(malloc(var_len + 1), '\0')
            strncpy(start, current, var_len);
        }
        current = instance + strlen(macro->alias[i]);
    }
    // goes through all parts of alias and stores strings in between them
    for (i = !macro->var_at_start; i < macro->alias_len; ++i) {
        instance = strstr(current, macro->alias[i]);
        if (!instance)
            return NULL;
        var_len = instance - current;
        vars[i] = memset(malloc(var_len + 1), '\0', var_len + 1);
        strncpy(var[i], current, var_len);
        current = instance + strlen(macro->alias[i]);
    }
    // counts how much space output buffer needs to be
    for (i = 0; i < macro->output_len; ++i) {
        if (macro->output[i].value)
            out_len += strlen(macro->output[i].value);
        if (macro->output[i].index >= 1)
            out_len += strlen(vars[macro->output[i].index-1]);
    }
    // goes through outputs and puts vars in correct places
    out = memset(malloc(out_len + 1), '\0', out_len + 1);
    current = out;
    for (i = 0; i < macro->output_len; ++i) {
        if (macro->output[i].value) {
            strcpy(current, macro->output[i].value);
            current += strlen(macro->output[i].value);
        }
        if (macro->output[i].index >= 1) {
            strcpy(current, vars[macro->output[i].index-1]);
            current += strlen(vars[macro->output[i].index-1]);
        }
    }
    return out;
}

char *preprocessor_run(struct List *macros, char *raw) {
    char *i1, *i2, *a1, *a2, *b1, *b2, *current = raw;
    bool keep;
    while (true) {
        i1 = strchr(current, '#');
        if (!i1)
            return raw;
        i2 = strchr(i1, ' ');
        i2 = '\0';
        switch (i1 + 1) {
            case "define":
                memmove(i1, i2 + 1, strlen(i2 + 1));
                a1 = i1;
                do {
                    a2 = strchr(a1, ';');
                } while (*(a2 - 1) == '\\');
                *a2 = '\0';
                list_insert(macros, macro_from_raw(a1));
                memmove(a1, a2 + 1, strlen(a2 + 1));
                current = a1;
                break;
            case "ifdef":
                memmove(i1, i2 + 1, strlen(i2 + 1));
                a1 = i1;
                do {
                    a2 = strchr(a1, ';');
                } while (*(a2 - 1) == '\\');
                *a2 = '\0';
                keep = list_search(macros, a1, &macro_is_defined);
                memmove(a1, a2 + 1, strlen(a2 + 1));
                b1 = a1;
                b2 = strstr(b1, "#endif;");
                if (!b2) {
                    if (!keep) {
                        *b2 = '\0';
                        return raw;
                    }
                    break;
                }
                memmove(b2, b2 + 7, strlen(b2 + 7));
                if (!keep) {
                    memmove(b1, b2, strlen(b2));
                    b2 = b1;
                }
                current = b2;
                break;
            case "ifndef":
                memmove(i1, i2 + 1, strlen(i2 + 1));
                a1 = i1;
                do {
                    a2 = strchr(a1, ';');
                } while (*(a2 - 1) == '\\');
                *a2 = '\0';
                keep = !list_search(macros, a1, &macro_is_defined);
                memmove(a1, a2 + 1, strlen(a2 + 1));
                b1 = a1;
                b2 = strstr(b1, "#endif;");
                if (!b2) {
                    if (!keep) {
                        *b2 = '\0';
                        return raw;
                    }
                    break;
                }
                memmove(b2, b2 + 7, strlen(b2 + 7));
                if (!keep) {
                    memmove(b1, b2, strlen(b2));
                    b2 = b1;
                }
                current = b2;
                break;
        }
    }
    return raw;
}
