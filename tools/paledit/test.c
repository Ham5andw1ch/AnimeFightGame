#include <stdio.h>
#include <stdlib.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "nuklear_backend.h"

void editor(struct nk_context *con)
{
    printf("FOO\n");
}

int main()
{
    printf("Compiled\n");
    run_nuklear(editor);
}
