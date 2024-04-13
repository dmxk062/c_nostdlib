#include "env.h"

char* getenv(const char* name) {
    if (name == NULL || *name == '\0') {
        return NULL;
    }
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

// yes this leaks memory because our allocated fields are never freed
// i cant really think of a way to fix that though
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
    if (name == NULL || *name == '\0') {
        return -1;
    }
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

i64 unsetenv(const char* name) {
    if (name == NULL || *name == '\0') {
        return -1;
    }
    u64 len_name = strlen(name);

    char** env = environ;

    while (*env != NULL) {
        if (strncmp(name, *env, len_name) == 0 && (*env)[len_name] == '=') {
            char** ep = env;
            do {
                ep[0] = ep[1];
            } while (*ep++);
        } else 
        env++;

    }
    return 0;


}
