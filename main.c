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
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));

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

    // Write data
    size_t pxcount = image->header->width * image->header->height;

    for (size_t i = 0; i < pxcount; i++) {
        fwrite(&image->data[i].blue, 1, 1, stream);
        fwrite(&image->data[i].red, 1, 1, stream);
        fwrite(&image->data[i].green, 1, 1, stream);
    }
    return true;
}

void free_bmp_image(struct bmp_image* image) {

    free(image->header);
    free(image->data);
    free(image);
}

// TRANSFORM
struct bmp_image* flip_horizontally(const struct bmp_image* image) {

    // Get size data
    size_t height = image->header->height;
    size_t width = image->header->width;
    size_t pxcount = width * height;
    
    // Alloc
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

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

    // Flip data horizontally
    for (size_t h = 0; h < height; h++) {
        for (size_t w = 0; w < width; w++) {
            newImage->data[(h * width) + w].blue = image->data[(h * width) + (width - 1 - w)].blue;
            newImage->data[(h * width) + w].red = image->data[(h * width) + (width - 1 - w)].red;
            newImage->data[(h * width) + w].green = image->data[(h * width) + (width - 1 - w)].green;
        }
    }
    return newImage;
}

struct bmp_image* flip_vertically(const struct bmp_image* image) {
    
    // Get size data
    size_t height = image->header->height;
    size_t width = image->header->width;
    size_t pxcount = width * height;
    
    // Alloc
    printf("Allocating memory for new image");
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

    // Copy header
    printf("Copying header\n");
    newImage->header->size = image->header->size;
    newImage->header->width = image->header->width;
    newImage->header->height = image->header->height;
    newImage->header->type = TYPE;
    newImage->header->offset = OFFSET;
    newImage->header->dib_size = DIB_SIZE;
    newImage->header->bpp = BPP;
    newImage->header->planes = PLANES;
    newImage->header->image_size = newImage->header->size - OFFSET;

    // Flip data vertically
    for (size_t h = 0; h < height; h++) {
        for (size_t w = 0; w < width; w++) {
            newImage->data[(h * width) + w].blue = image->data[((height - 1 - h) * width) + w].blue;
            newImage->data[(h * width) + w].red = image->data[((height - 1 - h) * width) + w].red;
            newImage->data[(h * width) + w].green = image->data[((height - 1 - h) * width) + w].green;
        }
    }
    return newImage;

}

struct bmp_image* rotate_right(const struct bmp_image* image) {
    
    // Get size data
    size_t height = image->header->height;
    size_t width = image->header->width;
    size_t pxcount = width * height;
    
    // Alloc
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

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

    // Turn right
    for (size_t w = 0; w < width; w++) {
        for (size_t h = 0; h < height; h++) {
            newImage->data[(h * width) + w].blue = image->data[((height - 1 - h) * width) + w].blue;
            newImage->data[(h * width) + w].red = image->data[((height - 1 - h) * width) + w].red;
            newImage->data[(h * width) + w].green = image->data[((height - 1 - h) * width) + w].green;
        }
    }
    return newImage;
}

struct bmp_image* rotate_left(const struct bmp_image* image) {

    // Get size data
    size_t height = image->header->height;
    size_t width = image->header->width;
    size_t pxcount = width * height;
    
    // Alloc
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

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

    // Turn left 
    for (size_t w = 0; w < width; w++) {
        for (size_t h = 0; h < height; h++) {
            newImage->data[(h * width) + w].blue = image->data[(h * width) + (width - 1 - w)].blue;
            newImage->data[(h * width) + w].red = image->data[(h * width) + (width - 1 - w)].red;
            newImage->data[(h * width) + w].green = image->data[(h * width) + (width - 1 - w)].green;
        }
    }
    return newImage;
}

struct bmp_image* crop(const struct bmp_image* image, const uint32_t start_y, const uint32_t start_x, const uint32_t height, const uint32_t width) {
    
    // Get image stats
    size_t source_width = image->header->width;
    size_t source_height = image->header->height;

    // Check staring point
    if (start_y < 0 || start_y >= source_height || start_x < 0 || start_x >= source_width)
        return NULL;

    // Check crop size
    if (height < 1 || width < 1)
        return NULL;

    // Check bounds of crop
    if (start_y + height > source_height || start_x + width > source_width)
        return NULL;


    // Get size data
    size_t pxcount = width * height;
    
    //Debug
    printf("Source H: %d\nSource W: %d\n", source_height, source_width);
    printf("New H: %d\nNew W: %d\n", height, width);
    printf("start x: %d\nstart y: %d\n", start_x, start_y);

    // Alloc new image
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

    // Create header
    newImage->header->size = OFFSET + pxcount * 3;
    newImage->header->width = width;
    newImage->header->height = height;
    newImage->header->type = TYPE;
    newImage->header->offset = OFFSET;
    newImage->header->dib_size = DIB_SIZE;
    newImage->header->bpp = BPP;
    newImage->header->planes = PLANES;
    newImage->header->image_size = pxcount * 3;

    for (size_t h = 0; h < height; h++) {
        for (size_t w = 0; w < width; w++) {
            newImage->data[(h * width) + w].blue = image->data[((source_height - start_y - h) * source_width) - (source_width - start_x - w)].blue;
            newImage->data[(h * width) + w].red = image->data[((source_height - start_y - h) * source_width) - (source_width - start_x - w)].red;
            newImage->data[(h * width) + w].green = image->data[((source_height - start_y - h) * source_width) - (source_width - start_x - w)].green;
        }  
    }
    return flip_vertically(newImage);
}

struct bmp_image* extract(const struct bmp_image* image, const char* colors_to_keep) {
    
    // Get size data
    size_t height = image->header->height;
    size_t width = image->header->width;
    size_t pxcount = width * height;
    
    // Alloc
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

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

    for (size_t w = 0; w < width; w++) {
        for (size_t h = 0; h < height; h++) {
            newImage->data[(h * width) + w].blue = image->data[((height - 1 - h) * width) + w].blue;
            newImage->data[(h * width) + w].red = image->data[((height - 1 - h) * width) + w].red;
            newImage->data[(h * width) + w].green = image->data[((height - 1 - h) * width) + w].green;
        }
    }

}


int main (void) {

    struct bmp_image *img;
    
    //Read image
    FILE *testfile = fopen("assets/lenna.bmp", "rb");
    img = read_bmp(testfile);
    fclose(testfile);

    struct bmp_image *flip;
    flip = crop(img, 100, 100, 100, 100);

    testfile = fopen("assets/testout.bmp", "wb");
    write_bmp(testfile, flip);
    fclose(testfile);



    
    
    return 0;
}




