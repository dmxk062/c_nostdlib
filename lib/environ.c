#include <environ.h>
#include <alloc.h>
#include <mem.h>
#include <cstring.h>
#include "errno.h"
#include "io.h"
#include "types.h"

errno_t Environment_init(struct Environment* environ, zstr* envvec) {
    u64 count = 0;
    zstr* envp = envvec;

    while (*envp++ != NULL) {
        count++;
    }
    count++; /* environ has to end in NULL */

    // get the next multiple of the size we want to allocate
    u64 size = (((count*sizeof(zstr)) + ENV_PAGE_SIZE -1) / ENV_PAGE_SIZE) * ENV_PAGE_SIZE;

    zstr* new_env = malloc(size);
    if (!new_env)
        return 1;

    environ->env_count = count - 1;
    environ->env_size  = size;
    environ->env       = new_env;

    memcpy(new_env, envvec, count*sizeof(zstr));

    for (u64 i = 0; i < count && new_env[i] != NULL; i++) {
        u64 len = strlen(new_env[i]);
        zstr newstr = malloc(len+1);
        if (!newstr)
            return ENOMEM;

        memcpy(newstr, new_env[i], len+1);
        new_env[i] = newstr;
    }
    return 0;
}

errno_t Environment_free(struct Environment* env) {
    for (u64 i = 0; i < env->env_count; i++) {
        free(env->env[i]);
    }
    free(env->env);


    return 0;
}

RESULT(u64) Environment_find(struct Environment* env, const zstr name) {
    u64 name_len = strlen(name);
    for (u64 i = 0; i < env->env_count; i++) {
        u64 ent_len = strlen(env->env[i]);
        if (ent_len < name_len) 
            continue;

        if (memeq(name, env->env[i], name_len) && *(env->env[i] + name_len) == '=') 
            return (RESULT(u64)){.success = TRUE, .value = i};
    }

    return (RESULT(u64)){.success = FALSE, .value = 0};

}

RESULT(zstr) Environment_get(struct Environment* env, const zstr name) {
    RESULT(u64) index = Environment_find(env, name);
    if (!index.success)
        return (RESULT(zstr)){.success = FALSE, .value = NULL};

    return (RESULT(zstr)){.success = TRUE, .value = env->env[index.value] + strlen(name) + 1};

}

errno_t Environment_set(struct Environment* env, const zstr name, const zstr value, bool replace) {
    u64 name_len  = strlen(name);
    u64 value_len = strlen(value);

    RESULT(u64) index = Environment_find(env, name);
    zstr* target = NULL;
    bool append = FALSE;
    // variable already exists, create it
    if (index.success) {
        if (!replace)
            return 1;


        target = env->env + index.value;
        free(env->env[index.value]);
    // append one
    } else {
        if (env->env_count >= env->env_size)
            return ENOMEM;

        target=env->env + env->env_count;
        append = TRUE;
    }

    zstr buffer = malloc(name_len + value_len + 2*sizeof(char)); /* + '=' + '\0' */
    if (!buffer)
        return ENOMEM;

    memcpy(buffer, name, name_len);
    buffer[name_len] = '=';
    memcpy(buffer + name_len + sizeof(char), value, value_len);
    buffer[name_len + value_len + sizeof(char)] = '\0';

    *target = buffer;

    if (append) {
        env->env_count++;;
        env->env[env->env_count] = NULL;
    }
    
    return 0;
}

errno_t Environment_unset(struct Environment* env, const zstr name) {
    RESULT(u64) index = Environment_find(env, name);
    if (!index.success)
        return 1;

    free(env->env[index.value]);

    for (u64 i = index.value; i < env->env_count; i++) {
        env->env[i] = env->env[i+1];
    }
    env->env_count--;

}
