#include "lib/string.h"
#include "lib/stat.h"
#include "alloc/alloc.h"
#include "lib/io.h"

int main(int argc, char* argv[]) {
    
    const i64 text_buffer_size = 4096;
    char* text_buffer = malloc(text_buffer_size);
    if (text_buffer == NULL) {
        puts("Failed to allocate memory\n");
    }

    i64 ret = fmt("%.f\n", text_buffer, text_buffer_size, 1, (f64)332.55567566745674756L);

    if (ret > 0 ) {puts(text_buffer);} else {return ret;}
    free(text_buffer);

    return 0;
}
