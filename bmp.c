#include "bmp.h"

struct bmp_header* read_bmp_header(FILE* stream) {
    // Go to start
    fseek(stream, 0, SEEK_SET);

    // New header calloc
    struct bmp_header *newH = (struct bmp_header*) calloc(1,sizeof(struct bmp_header));

    // Read and check file field
    size_t ret = fread(&newH->type, 2, 1, stream);

    // Check corr
    if (ret < 1) {
         printf("Error: This is not a BMP file.");
         free(newH);
         return NULL;
    }

    // Check val
    if (newH->type != 0x4d42) {
        printf("Error: Corrupted BMP file.");
        free(newH);
        return NULL;
    }

    // Reading file size
    fread(&newH->size, 4, 1, stream);

    // Reading h/w
    fseek(stream,18,SEEK_SET);
    fread(&newH->width, 4, 1, stream);
    fread(&newH->height, 4, 1, stream);

    // Writing constant data
    newH->offset = 0x36;
    newH->dib_size = 0x28;
    newH->planes = 0x01;
    newH->bpp = 0x18;
    newH->image_size = newH->size - 0x36;   // OR 0 NOT SURE IF ITS NEEDED YET

    return newH;
}

struct pixel* read_data(FILE* stream, const struct bmp_header* header) {
    // Check header
    if (header == NULL) {
        return NULL;
    }

    // Create pixel structure
    size_t pxcount = header->width * header->height;
    struct pixel *pxarr = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

    // Read pixel data
    fseek(stream, 54, SEEK_SET);
    for (size_t i = 0; i < pxcount; i++) {
        fread(&pxarr[i].blue, 1, 1, stream);
        fread(&pxarr[i].red, 1, 1, stream);
        fread(&pxarr[i].green, 1, 1, stream);
    }

    return pxarr;
}

struct bmp_image* read_bmp(FILE* stream) {
    
    // Create image
    struct bmp_image *newImage = (struct bmp_image*) calloc(1, sizeof(struct bmp_image));

    // Load header
    newImage->header = read_bmp_header(stream);
    if (newImage->header == NULL) {
        free(newImage);
        return NULL;
    }

    // Load data
    newImage->data = read_data(stream, newImage->header);
    if (newImage->data == NULL) {
        free(newImage);
        return NULL;
    }
    
    return newImage;
}
