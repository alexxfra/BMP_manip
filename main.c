#include "bmp.h"
#include "transformations.h"
#include "math.h"

int main (int argc, char *argv[]) {    
    
    struct bmp_image *img;
    
    //Read image
    FILE *testfile = fopen("assets/old.monk.bmp", "rb");
    img = read_bmp(testfile);
    if (img == NULL) {
        free_bmp_image(img);
        exit(1);
    }
    fclose(testfile);

    struct bmp_image *trans = flip_vertically(img);
    
    testfile = fopen("assets/testout.bmp", "wb");
    write_bmp(testfile, trans);
    fclose(testfile);
    free_bmp_image(img);
    free_bmp_image(trans);

    return 0;
}




