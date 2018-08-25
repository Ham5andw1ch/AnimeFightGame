#include <stdio.h>
#include "farbfeld.h"

int main(int argc, char** argv)
{
    printf("Compiled\n");

    if(argc < 2)
    {
        fprintf(stderr, "Filename, please.");
        return 2;
    }

    struct farbfeld ff;
    int ret;
    if((ret = readFarbfeld(argv[1], &ff)))
        return ret;

    printf("Farbfeld Image:\n");
    printf("\tMagic: %c%c%c%c%c%c%c%c\n", ff.magic[0], ff.magic[1], ff.magic[2], ff.magic[3],
            ff.magic[4], ff.magic[5], ff.magic[6], ff.magic[7]);
    printf("\tWidth: %u\tHeight: %u\n", ff.width, ff.height);
    for(int r = 0; r < ff.height; ++r)
    {
        printf("\t");
        for(int c = 0; c < ff.width; ++c)
        {
            printf("[%4X %4X %4X %4X] ", ff.rgba[r * ff.width + c].red, ff.rgba[r * ff.width + c].green,
                    ff.rgba[r * ff.width + c].blue, ff.rgba[r * ff.width + c].alpha);
        }
        printf("\n");
    }
}
