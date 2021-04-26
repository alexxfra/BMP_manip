#include "transformations.h"

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
    struct bmp_image *ret = flip_vertically(newImage);
    free(newImage);

    return ret;
}

struct bmp_image* scale(const struct bmp_image* image, float factor) {

    // Get source size data
    size_t source_height = image->header->height;
    size_t source_width = image->header->width;
    size_t source_pxcount = source_width * source_height;

    size_t new_height;
    size_t new_width;
    size_t new_pxcount;

    // Check factor
    if (factor == 1) {
        new_width = source_width;
        new_height = source_height;
        new_pxcount = source_pxcount;
    }

    else if (factor != 1) {
        new_width = round(source_width * factor);
        new_height = round(source_height * factor);
        new_pxcount = new_width * new_height;
    }
    printf("OW: %d\nOH: %d\nOP: %d\n", source_width, source_height, source_pxcount);
    printf("NW: %d\nNH: %d\nNP: %d\n", new_width, new_height, new_pxcount);


    // Alloc
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(new_pxcount, sizeof(struct pixel));

    // Copy header
    newImage->header->size = new_pxcount * 3 + 57;
    newImage->header->width = new_width;
    newImage->header->height = new_height;
    newImage->header->type = TYPE;
    newImage->header->offset = OFFSET;
    newImage->header->dib_size = DIB_SIZE;
    newImage->header->bpp = BPP;
    newImage->header->planes = PLANES;
    newImage->header->image_size = newImage->header->size - OFFSET;

    int newx, newy;

    for (size_t h = 0; h < new_height; h++) {
        for (size_t w = 0; w < new_height; w++) {
            newx = (w * source_width) / new_width;
            newy = (h * source_height) / new_height;
            printf("new x: %d\tnew y: %d\nold x: %d\told y: %d\n\n", w, h, newx, newy);

            newImage->data[(h * new_width) + w].blue = image->data[((h * source_height)/new_height) * source_height + ((w * source_width) / new_width)].blue;
            newImage->data[(h * new_width) + w].red = image->data[((h * source_height)/new_height) * source_height + ((w * source_width) / new_width)].red;
            newImage->data[(h * new_width) + w].green = image->data[((h * source_height)/new_height) * source_height + ((w * source_width) / new_width)].green;
        }
    }
    return newImage;
}

struct bmp_image* extract(const struct bmp_image* image, const char* colors_to_keep) {

    uint8_t blue = 0x00;
    uint8_t red = 0x00;
    uint8_t green = 0x00;

    //Check pointers
    if (image == NULL || colors_to_keep == NULL)
        return NULL;
    
    // Check string content
    short index = 0;
    while (colors_to_keep[index] != '\0') {
        printf("Index: %d\n", index);
        // check if red
        if (colors_to_keep[index] == R) 
            red = 0xFF;

        // check if green
        else if (colors_to_keep[index] == G)
            green = 0xFF;

        // check if blue
        else if (colors_to_keep[index] == B)
            blue = 0xFF;
        
        else
            return NULL;

        index++;
    }
    
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
            newImage->data[(h * width) + w].blue = image->data[(h * width) + w].blue & blue;
            newImage->data[(h * width) + w].red = image->data[(h * width) + w].red & red;
            newImage->data[(h * width) + w].green = image->data[(h * width) + w].green & green;
        }
    }

    return newImage;
}