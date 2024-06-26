#include "types.h"
#include <env.h>
#include <alloc.h>
#include <cstring.h>
#include <mem.h>

/*
 * get an environment variable from the global environment vector
 */
Result(zstr) getenv(const char* name) {

    // filter out invalid names
    if (name == NULL || *name == '\0') {
        return Err(zstr, 2);
    }

    zstr* env = environ;
    u64 len_name = strlen(name);

    // iterate over the vector
    while (*env != NULL) {
        // if the name matches and is terminated with a '=', we found it
        if (strncmp(name, *env, len_name) == 0 && (*env)[len_name]== '=') {
            // just return a pointer to the value
            return Ok(zstr, &((*env)[len_name + 1]));
        }
        env++;
    }
    return Err(zstr, 1);
}

/*
 * FIXME: memory
 * yes this leaks memory because our allocated fields are never freed
 * i cant really think of a way to fix that though, since I can't know where that memory is from(is it on the stack, heap etc)
 * the only long term solution would be to add some identification field to our malloc structs
 * GLIBC copies all of environ into the heap AFAIK, that's too complicated for me right now
 */
// static inline
// errno_t __chgenv(const char* name, u64 len_name, const char* value, u64 len_valu, char** target) {
//         u64 total_len = len_valu + len_name + 2*(sizeof(char));
//         char* buffer = malloc(total_len);
//         if (buffer == NULL) {
//             return 1;
//         }
//
//         memcpy(buffer, name, len_name);
//         buffer[len_name] = '=';
//         memcpy(buffer + len_name + sizeof(char), value, len_valu);
//         buffer[total_len] = '\0';
//
//         // replace the buffer in place, leaks ram if it was allocated on the heap
//         *target = buffer;
//         return 0;
//
// }

// DEPRECATED: for changing the environment use environ.h
// errno_t setenv(const char* name, const char* value, bool replace) {
//     if (name == NULL || *name == '\0') {
//         return 1;
//     }
//     zstr* env = environ;
//
//     u64 len_name = strlen(name);
//     u64 len_valu = strlen(value);
//
//     u64 total_len = len_name + len_valu + 2*(sizeof(char));
//     while (*env != NULL) {
//         if (strncmp(name, *env, len_name) == 0 && (*env)[len_name] == '=') {
//             if (!replace) {
//                 return 0;
//             }
//             return __chgenv(name, len_name, value, len_valu, env);
//         }
//         env++;
//     }
//     // we're appending, so we need to zero the next one
//     return __chgenv(name, len_name, value, len_valu, env);
//     *(env++) = NULL;
//
// }

// DEPRECATED: for changing the environment use environ.h
// errno_t unsetenv(const char* name) {
//     if (name == NULL || *name == '\0') {
//         return 1;
//     }
//     u64 len_name = strlen(name);
//
//     zstr* env = environ;
//
//     while (*env != NULL) {
//         if (strncmp(name, *env, len_name) == 0 && (*env)[len_name] == '=') {
//             char** ep = env;
//             /*
//              * shift the *entire* vector backwards
//              * sounds dumb, GLIBC does it the same way
//              */
//             do {
//                 ep[0] = ep[1];
//             } while (*ep++);
//         } else 
//         env++;
//
//     }
//     return 0;
//
//
// }
