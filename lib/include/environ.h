#pragma once
#ifndef _ENVIRON_H
#define _ENVIRON_H
#include "types.h"

/*
 * Environment handlign
 * This is a safer, heap based alternative to the env.h header
 * Instead of modifying the stack directly, this copies the environment to the heap and modifies it there
 * This allos this to free allocated environment variables
 */

#define ENV_PAGE_SIZE 4096

struct Environment {
    zstr*   env;
    u64     env_count;
    u64     env_size;
};

/* Create a new high level environment */
errno_t Environment_init(struct Environment* environ, zstr* envvec);
/* Cleanup on program exit or after fork in the parent process */
errno_t Environment_free(struct Environment* environ);

/* Find the *whole* entry of an environment variable */
RESULT(u64) Environment_find(struct Environment* env, const zstr name);
/* get the value associated with **name** */
RESULT(zstr) Environment_get(struct Environment* env, const zstr name);
/* create or modify a variable */
errno_t Environment_set(struct Environment* env, const zstr name, const zstr value, bool replace);
/* delete one */
errno_t Environment_unset(struct Environment* env, const zstr name);




#endif
