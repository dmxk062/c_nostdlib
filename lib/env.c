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

static inline
i64 __chgenv(const char* name, u64 len_name, const char* value, u64 len_valu, char** target) {
        u64 total_len = len_valu + len_name + 2*(sizeof(char));
        char* buffer = malloc(total_len);
        if (buffer == NULL) {
            return -1;
        }
        memcpy(buffer, name, len_name);
        buffer[len_name] = '=';
        memcpy(buffer + len_name + sizeof(char), value, len_valu);
        buffer[total_len] = '\0';
        *target++ = buffer;
        *target = NULL;
        return 0;

}

i64 setenv(const char* name, const char* value, bool replace) {
    char** env = environ;

    u64 len_name = strlen(name);
    u64 len_valu = strlen(value);

    u64 total_len = len_name + len_valu + 2*(sizeof(char));
    while (*env != NULL) {
        if (strncmp(name, *env, len_name) == 0 && (*env)[len_name] == '=') {
            if (!replace) {
                return 0;
            }
            return __chgenv(name, len_name, value, len_valu, env);
        }
        env++;
    }
    return __chgenv(name, len_name, value, len_valu, env);
}
