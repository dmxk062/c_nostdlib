#pragma once
#ifndef _ENVIRON_H
#define _ENVIRON_H
#include "types.h"

#define ENV_PAGE_SIZE 4096

struct Environment {
    zstr*   env;
    u64     env_count;
    u64     env_size;
};

/* Create a new high level environment */
errno_t Environment_init(struct Environment* environ, zstr* envvec);
/* Cleanup on program exit */
errno_t Environment_free(struct Environment* environ);

RESULT(u64) Environment_find(struct Environment* env, const zstr name);
RESULT(zstr) Environment_get(struct Environment* env, const zstr name);
errno_t Environment_set(struct Environment* env, const zstr name, const zstr value, bool replace);
errno_t Environment_unset(struct Environment* env, const zstr name);




#endif
