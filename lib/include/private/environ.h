#pragma once
#ifndef _PRIVATE_ENVIRON_H
#define _PRIVATE_ENVIRON_H
#include <environ.h>
#define ENV_PAGE_SIZE (4 * 1024 * 1024) // allocate 4mb just for the environment

struct Environment {
    zstr*   env;
    u64     env_count;
    u64     env_size;
};

struct Environment* Environment_new(u64 size);
/* Create a new high level environment */
errno_t Environment_init(struct Environment* environ, zstr* envvec);
/* Cleanup on program exit or after fork in the parent process */
errno_t Environment_free(struct Environment* environ);

/* Find the *whole* entry of an environment variable */
Result(u64) Environment_find(struct Environment* env, const char* name, u64 name_len);
Result(u64) Environment_zfind(struct Environment* env, const zstr name);
/* get the value associated with **name** */
Result(zstr) Environment_get(struct Environment* env, const char* name, u64 name_len);
Result(zstr) Environment_zget(struct Environment* env, const zstr name);
/* create or modify a variable */
errno_t Environment_set(struct Environment* env, const char* name, u64 name_len, const char* value, u64 value_len, bool replace);
errno_t Environment_zset(struct Environment* env, const zstr name, const zstr value, bool replace);
/* delete one */
errno_t Environment_unset(struct Environment* env, const char* name, u64 name_len);
errno_t Environment_zunset(struct Environment* env, const zstr name);



#endif
