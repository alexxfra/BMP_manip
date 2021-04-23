#include "bmp.h"
#include "transformations.h"

// CONSTS
#define OFFSET 0x36
#define DIB_SIZE 0x28
#define PLANES 0x01
#define BPP 0x18
#define TYPE 0x4d42


// BMP
struct bmp_header* read_bmp_header(FILE* stream) {
    
    // Check stream
    if (stream == NULL){
        return NULL;
    }

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
    if (newH->type != TYPE) {
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
    newH->offset = OFFSET;
    newH->dib_size = DIB_SIZE;
    newH->planes = PLANES;
    newH->bpp = BPP;
    newH->image_size = newH->size - OFFSET;   // OR 0 NOT SURE IF ITS NEEDED YET

    return newH;
}

struct pixel* read_data(FILE* stream, const struct bmp_header* header) {
    
    // Check header
    if (header == NULL) {
        return NULL;
    }

    // Check stream
    if (stream == NULL){
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

bool write_bmp(FILE* stream, const struct bmp_image* image){
    
    // Check stream
    if (stream == NULL || image == NULL){
        return false;
    }
    
    // Write header
    fwrite(&image->header->type, 2, 1, stream);
    fwrite(&image->header->size, 4, 1, stream);
    fwrite(&image->header->reserved1, 2, 1, stream);
    fwrite(&image->header->reserved2, 2, 1, stream);
    fwrite(&image->header->offset, 4, 1, stream);
    fwrite(&image->header->dib_size, 4, 1, stream);
    fwrite(&image->header->width, 4, 1, stream);
    fwrite(&image->header->height, 4, 1, stream);
    fwrite(&image->header->planes, 2, 1, stream);
    fwrite(&image->header->bpp, 2, 1, stream);
    fwrite(&image->header->compression, 4, 1, stream);
    fwrite(&image->header->image_size, 4, 1, stream);
    fwrite(&image->header->x_ppm, 4, 1, stream);
    fwrite(&image->header->y_ppm, 4, 1, stream);
    fwrite(&image->header->num_colors, 4, 1, stream);
    fwrite(&image->header->important_colors, 4, 1, stream);

    // Write data
    size_t pxcount = image->header->width * image->header->height;

    for (size_t i = 0; i < pxcount; i++) {
        fwrite(&image->data[i].blue, 1, 1, stream);
        fwrite(&image->data[i].red, 1, 1, stream);
        fwrite(&image->data[i].green, 1, 1, stream);
    }
    return true;
}

void free_bmp_image(struct bmp_image* image){

    free(image->header);
    free(image->data);
    free(image);
}

// TRANSFORM
struct bmp_image* flip_horizontally(const struct bmp_image* image){
    
    // Create image
    struct bmp_image *newImage = (struct bmp_image*) calloc(1, sizeof(struct bmp_image));

    // Copy header
    newImage->header->size = image->header->size;
    newImage->header->width = image->header->width;
    newImage->header->height = image->header->height;
    newImage->header->type = TYPE;
    newImage->header->offset = OFFSET;
    newImage->header->dib_size = DIB_SIZE;
    newImage->header->bpp = BPP;
    newImage->header->planes = PLANES;
    newImage->header->image_size = newImage->header->size - OFFSET;

    size_t height = image->header->height;
    size_t width = image->header->width;

    // Flip data horizontally
    for (size_t h = 0; h < height; h++) {
        for (size_t w = 0; h < width; w++) {
            newImage->data[h * height + w] = image->data[(h) * height + (width - 1 - w)];
        }
    }
    return newImage;
}

int main (void) {
    
    struct bmp_image *testimg;
    FILE *testfile = fopen("assets/umbrella.bmp", "rb");
    testimg = read_bmp(testfile);
    fclose(testfile);

    struct bmp_image *hor = flip_horizontally(testimg);
    testfile = fopen("assets/testout.bmp", "wb");
    write_bmp(testfile, hor);
    fclose(testfile);

    free_bmp_image(testimg);
    free_bmp_image(hor);

    
    
    return 0;
}




