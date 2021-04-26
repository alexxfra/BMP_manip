#include "bmp.h"
#include "transformations.h"

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

    struct bmp_image *vert = flip_vertically(img);
    struct bmp_image *hor = flip_horizontally(img);
    struct bmp_image *cr = crop(img, 0, 0, 30, 30);
    struct bmp_image *lt = rotate_left(img);
    struct bmp_image *rt = rotate_right(img);
    struct bmp_image *sc = scale(img, 2);
    struct bmp_image *et = extract(img, "rgb");
    
    testfile = fopen("assets/testout.bmp", "wb");
    write_bmp(testfile, sc);
    fclose(testfile);
    free_bmp_image(img);
    free_bmp_image(vert);
    free_bmp_image(hor);
    free_bmp_image(cr);
    free_bmp_image(lt);
    free_bmp_image(rt);
    free_bmp_image(sc);
    free_bmp_image(et);


    return 0;
}




