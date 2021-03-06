#include "bmp.h"

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
         free(newH);
         return NULL;
    }

    // Check val
    if (newH->type != 0x4d42) {
        free(newH);
        return NULL;
    }

    // Reading file size
    fread(&newH->size, 4, 1, stream);

    // Reading h/w
    fseek(stream,18,SEEK_SET);
    fread(&newH->width, 4, 1, stream);
    fread(&newH->height, 4, 1, stream);

    // Read X/Ypm
    fseek(stream,38,SEEK_SET);
    fread(&newH->x_ppm, 4, 1, stream);
    fread(&newH->y_ppm, 4, 1, stream);

    // Writing constant data
    newH->offset = 0x36;
    newH->dib_size = 0x28;
    newH->planes = 0x01;
    newH->bpp = 0x18;
    newH->image_size = newH->size - 54;

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

    // Check data size
    fseek(stream, 0, SEEK_END);
    size_t dataEnd = ftell(stream);

    fseek(stream, 54, SEEK_SET);
    size_t dataStart = ftell(stream);

    if (dataEnd - dataStart != header->size - 54) {
        return NULL;
    }

    // Create pixel structure
    size_t pxcount = header->width * header->height;
    struct pixel *pxarr = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

    // Calculate padding
    short paddingAmount = ((header->width * 24 + 31) / 32) * 4 - header->width * 3;

    // Read pixel data
    long index;
    
    for (size_t h = 0; h < header->height; h++) {
        for (size_t w = 0; w < header->width; w++) {
            index = (h * header->width) + w;
            fread(&pxarr[index].blue, 1, 1, stream);
            fread(&pxarr[index].green, 1, 1, stream);
            fread(&pxarr[index].red, 1, 1, stream);
        }
        fseek(stream, paddingAmount, SEEK_CUR);
    }

    return pxarr;
}

struct bmp_image* read_bmp(FILE* stream) {
    
    // Create image
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));

    // Load header
    newImage->header = read_bmp_header(stream);
    if (newImage->header == NULL) {
        fprintf(stderr, "Error: This is not a BMP file.\n");
        free_bmp_image(newImage);
        return NULL;
    }

    // Load data
    newImage->data = read_data(stream, newImage->header);
    if (newImage->data == NULL) {
        fprintf(stderr, "Error: Corrupted BMP file.\n");
        free_bmp_image(newImage);
        return NULL;
    }
    
    return newImage;
}

bool write_bmp(FILE* stream, const struct bmp_image* image) {
    
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


    // Calculate padding
    short paddingAmount = ((image->header->width * 24 + 31) / 32) * 4 - image->header->width * 3;
    
    // Write data
    long index;
    for (size_t h = 0; h < image->header->height; h++) {
        for (size_t w = 0; w < image->header->width; w++) {
            index = (h * image->header->width) + w;
            fwrite(&image->data[index].blue, 1, 1, stream);
            fwrite(&image->data[index].green, 1, 1, stream);
            fwrite(&image->data[index].red, 1, 1, stream);
        }
        fwrite(PADDING_CHAR, 1, paddingAmount, stream);
    }
    
    return true;
}

void free_bmp_image(struct bmp_image* image) {
    if (image != NULL) {
        if (image->header != NULL) {
            free(image->header);
        }

        if (image->data != NULL) {
            free(image->data);
        }
        free(image);
    }
}
