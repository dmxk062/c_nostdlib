#include "types.h"
#include <linux/auxv.h>
#include <env.h>

Elf64Auxval* Auxval_find_auxv() {
    zstr* envp = environ;
    while(*envp++ != NULL){}

    return (Elf64Auxval*)envp;

}
Elf64Auxval* Auxval_get_value(Elf64Auxval* auxv, enum AuxvalType type) {
    while (auxv->type != AuxvalType_NULL) {
        if (auxv->type == type){
            return auxv;
        }
        auxv++;
    }

}
