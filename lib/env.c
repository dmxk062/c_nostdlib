#include "env.h"

char* getenv(const char* name) {
    char** env = environ;

    u64 len_name = strlen(name);

    while (*env != NULL) {
        if (strncmp(name, *env, len_name) == 0 && (*env)[len_name]== '=') {
            return &((*env)[len_name + 1]);
        }
        env++;
    }
    return NULL;
}

i64 setenv(const char* name, const char* value, bool replace) {
    char** env = environ;

    u64 len_name = strlen(name);
    u64 len_valu = strlen(value);

    // + '=' + '\0'
    u64 total_len = len_name + len_valu + 2*(sizeof(char));
    while (*env != NULL) {
        if (strncmp(name, *env, len_name) == 0 && (*env)[len_name] == '=') {
            if (!replace) {
                return 0;
            }
            memcpy(*env + len_name + sizeof(char), value, len_valu);
            return 0;
        }
        env++;
    }
}
