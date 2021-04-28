#include "bmp.h"
#include "transformations.h"
#include "math.h"


int main (int argc, char *argv[]) {    
    
    FILE* stream = fopen("assets/square.4x2.bmp", "rb");
    struct bmp_image* image = read_bmp(stream);
    fclose(stream);

    struct bmp_image* fright = rotate_right(image);
    //struct bmp_image* fleft = extract(fright, "r");

    FILE *out = fopen("assets/testout.bmp", "wb");
    write_bmp(out, fright);
    fclose(out);
    // int w = 1;
    // int h = 1;

    // printf("%d",((w * 24 + 31) / 32) * 4 * h);

    return 0;
}




