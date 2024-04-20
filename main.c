#include "include/types.h"
#include "lib/string.h"
#include "lib/parsers.h"
#include "lib/format.h"
#include "lib/io.h"
#include "lib/stat.h"
#include "alloc/alloc.h"

const char rwx[] = "rwx-";
const char bits[] = "ugs-";

void index_perms(char* out, bool r, bool w, bool x, const char* fmt) {
    if (r) out[0]=fmt[0];
    else out[0]=fmt[3];
    if (w) out[1]=fmt[1];
    else out[1]=fmt[3];
    if (x) out[2]=fmt[2];
    else out[2]=fmt[3];
}

i32 main(i32 argc, zstr argv[]) {

    if (argc != 2) {
        return 2;
    }

    struct stat st;

    stat(argv[1], &st);

    char buffer[1024];

    index_perms(buffer, st.mode.setuid, st.mode.setgid, st.mode.sticky, bits);
    index_perms(buffer + 3, st.mode.uread, st.mode.uwrite, st.mode.uexec, rwx);
    index_perms(buffer + 6, st.mode.gread, st.mode.gwrite, st.mode.gexec, rwx);
    index_perms(buffer + 9, st.mode.oread, st.mode.owrite, st.mode.oexec, rwx);
    buffer[12] = '\0';

    fprint("%s %o\n", (fmts){
            {buffer}, {.i = st.mode.type}
            });



    

    return 0;
}

