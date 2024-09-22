#include <private/environ.h>
#include <alloc.h>
#include <mem.h>
#include <cstring.h>
#include <errno.h>
#include <types.h>
#include <private/libc_start.h>

struct Environment* Environment_new(u64 size) {
    struct Environment* environ = malloc(sizeof(struct Environment));
    zstr* envvec = malloc(size * sizeof(zstr));
    if (!environ||!envvec) {
        return NULL;
    }
    environ->env_size = size;
    environ->env_count = 0;
    environ->env = envvec;
    return environ;
}

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
        newstr[len] = '\0';
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

Result(u64) Environment_find(struct Environment* env, const char* name, u64 name_len) {
    for (u64 i = 0; i < env->env_count; i++) {
        u64 ent_len = strlen(env->env[i]);
        if (ent_len < name_len) 
            continue;

        if (memeq(name, env->env[i], name_len) && *(env->env[i] + name_len) == '=') 
            return Ok(u64, i);
    }

    return Err(u64, 0);

}

Result(u64) Environment_zfind(struct Environment* env, const zstr name) {
    u64 name_len = strlen(name);
    return Environment_find(env, name, name_len);

}

Result(zstr) Environment_get(struct Environment* env, const char* name, u64 name_len) {
    Result(u64) index = Environment_find(env, name, name_len);
    if (!index.ok)
        return Err(zstr, NULL);

    return Ok(zstr, env->env[index.value] + name_len + 1);

}
Result(zstr) Environment_zget(struct Environment* env, const zstr name) {
    return Environment_get(env, name, strlen(name));
}

errno_t Environment_set(struct Environment* env, const char* name, u64 name_len, const char* value, u64 value_len, bool replace) {

    Result(u64) index = Environment_find(env, name, name_len);
    zstr* target = NULL;
    bool append = false;
    // variable already exists, create it
    if (index.ok) {
        if (!replace)
            return 1;
        target = env->env + index.value;
        free(env->env[index.value]);
    // append one
    } else {
        if (env->env_count >= env->env_size)
            return ENOMEM;

        target = env->env + env->env_count;
        append = true;
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

errno_t Environment_zset(struct Environment* env, const zstr name, const zstr value, bool replace) {
    return Environment_set(env, name, strlen(name), value, strlen(value), replace);
}

errno_t Environment_unset(struct Environment* env, const char* name, u64 name_len) {
    Result(u64) index = Environment_find(env, name, name_len);
    if (!index.ok)
        return 1;

    free(env->env[index.value]);

    for (u64 i = index.value; i < env->env_count; i++) {
        env->env[i] = env->env[i+1];
    }
    env->env_count--;
}

errno_t Environment_zunset(struct Environment* env, const zstr name) {
    return Environment_unset(env, name, strlen(name));
}

Result(zstr) getenv(const zstr name) {
    return Environment_zget(g_nolibc_global_STATE.environ, name);
}
errno_t setenv(const zstr name, const zstr value, bool replace) {
    return Environment_zset(g_nolibc_global_STATE.environ, name, value, replace);
}
errno_t unsetenv(const zstr name) {
    return Environment_zunset(g_nolibc_global_STATE.environ, name);
}
