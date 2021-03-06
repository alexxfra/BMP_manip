#include "transformations.h"
#include "math.h"

struct bmp_image* flip_horizontally(const struct bmp_image* image) {

    if (image == NULL)
        return NULL;

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
    newImage->header->type = 0x4d42;
    newImage->header->offset = 0x36;
    newImage->header->dib_size = 0x28;
    newImage->header->bpp = 0x18;
    newImage->header->planes = 0x01;
    newImage->header->image_size = image->header->size - 0x36;

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
    
    if (image == NULL)
        return NULL;

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
    newImage->header->type = 0x4d42;
    newImage->header->offset = 0x36;
    newImage->header->dib_size = 0x28;
    newImage->header->bpp = 0x18;
    newImage->header->planes = 0x01;
    newImage->header->image_size = image->header->size - 0x36;

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
    
    if (image == NULL)
        return NULL;

    // Get size data
    size_t height = image->header->height;
    size_t width = image->header->width;
    size_t pxcount = width * height;
    
    // Alloc
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

    // Make header
    // Change  w/h
    newImage->header->width = image->header->height;
    newImage->header->height = image->header->width;

    // Calculate data size
    newImage->header->image_size = ((newImage->header->width * 24 + 31) / 32) * 4 * newImage->header->height;
    newImage->header->size = newImage->header->image_size + 0x36;

    // Add constants
    newImage->header->type = 0x4d42;
    newImage->header->offset = 0x36;
    newImage->header->dib_size = 0x28;
    newImage->header->bpp = 0x18;
    newImage->header->planes = 0x01;


    // Turn right
    for (size_t h = 0; h < newImage->header->height; h++) {
        for (size_t w = 0; w < newImage->header->width; w++) {
            newImage->data[((newImage->header->height - 1 - h) * newImage->header->width) + w].blue = image->data[(w * width) + h].blue;
            newImage->data[((newImage->header->height - 1 - h) * newImage->header->width) + w].red = image->data[(w * width) + h].red;
            newImage->data[((newImage->header->height - 1 - h) * newImage->header->width) + w].green = image->data[(w * width) + h].green;
        }
    }

    return newImage;
}

struct bmp_image* rotate_left(const struct bmp_image* image) {

    if (image == NULL)
        return NULL;

    // Get size data
    size_t height = image->header->height;
    size_t width = image->header->width;
    size_t pxcount = width * height;
    
    // Alloc
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(pxcount, sizeof(struct pixel));

    // Make header
    // Change  w/h
    newImage->header->width = image->header->height;
    newImage->header->height = image->header->width;

    // Calculate data size
    newImage->header->image_size = ((newImage->header->width * 24 + 31) / 32) * 4 * newImage->header->height;
    newImage->header->size = newImage->header->image_size + 0x36;

    // Add constants
    newImage->header->type = 0x4d42;
    newImage->header->offset = 0x36;
    newImage->header->dib_size = 0x28;
    newImage->header->bpp = 0x18;
    newImage->header->planes = 0x01;

    // Turn left 1
    for (size_t h = 0; h < newImage->header->height; h++) {
        for (size_t w = 0; w < newImage->header->width; w++) {
            newImage->data[(h * newImage->header->width) + (newImage->header->width - 1 - w)].blue = image->data[(w * width) + h].blue;
            newImage->data[(h * newImage->header->width) + (newImage->header->width - 1 - w)].red = image->data[(w * width) + h].red;
            newImage->data[(h * newImage->header->width) + (newImage->header->width - 1 - w)].green = image->data[(w * width) + h].green;
        }
    }

    return newImage;
}

struct bmp_image* crop(const struct bmp_image* image, const uint32_t start_y, const uint32_t start_x, const uint32_t height, const uint32_t width) {
    
    if (image == NULL)
        return NULL;

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
    // Change w/h
    newImage->header->width = width;
    newImage->header->height = height;

    //Calculate size
    newImage->header->image_size = ((width * 24 + 31) / 32) * 4 * height;
    newImage->header->size = newImage->header->image_size + 0x36;

    // Add constants
    newImage->header->type = 0x4d42;
    newImage->header->offset = 0x36;
    newImage->header->dib_size = 0x28;
    newImage->header->bpp = 0x18;
    newImage->header->planes = 0x01;

    for (size_t h = 0; h < height; h++) {
        for (size_t w = 0; w < width; w++) {
            newImage->data[(h * width) + w].blue = image->data[((source_height - start_y - h) * source_width) - (source_width - start_x - w)].blue;
            newImage->data[(h * width) + w].red = image->data[((source_height - start_y - h) * source_width) - (source_width - start_x - w)].red;
            newImage->data[(h * width) + w].green = image->data[((source_height - start_y - h) * source_width) - (source_width - start_x - w)].green;
        }  
    }
    struct bmp_image *ret = flip_vertically(newImage);
    free_bmp_image(newImage);

    return ret;
}

struct bmp_image* scale(const struct bmp_image* image, float factor) {

    if (image == NULL)
        return NULL;

    if (factor <= 0)
        return NULL;

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

    // Alloc
    struct bmp_image *newImage = (struct bmp_image*) malloc(sizeof(struct bmp_image));
    newImage->header = (struct bmp_header*) calloc(1,sizeof(struct bmp_header)); 
    newImage->data = (struct pixel*) calloc(new_pxcount, sizeof(struct pixel));

    // Copy header
    // Change h/w
    newImage->header->width = new_width;
    newImage->header->height = new_height;

    // Calculate size
    newImage->header->image_size = ((new_width * 24 + 31) / 32) * 4 * new_height;
    newImage->header->size = newImage->header->image_size + 0x36;
    newImage->header->type = 0x4d42;
    newImage->header->offset = 0x36;
    newImage->header->dib_size = 0x28;
    newImage->header->bpp = 0x18;
    newImage->header->planes = 0x01;

    for (size_t hIndex = 0; hIndex < new_height; hIndex++) {
        for (size_t wIndex = 0; wIndex < new_width; wIndex++) {
            newImage->data[(hIndex * new_width) + wIndex].blue = image->data[((hIndex*source_height)/new_height)*source_width + (wIndex * source_width)/new_width].blue;
            newImage->data[(hIndex * new_width) + wIndex].green = image->data[((hIndex*source_height)/new_height)*source_width + (wIndex * source_width)/new_width].green;
            newImage->data[(hIndex * new_width) + wIndex].red = image->data[((hIndex*source_height)/new_height)*source_width + (wIndex * source_width)/new_width].red;
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
        // check if red
        if (colors_to_keep[index] == 0x72) 
            red = 0xFF;

        // check if green
        else if (colors_to_keep[index] == 0x67)
            green = 0xFF;

        // check if blue
        else if (colors_to_keep[index] == 0x62)
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
    newImage->header->type = 0x4d42;
    newImage->header->offset = 0x36;
    newImage->header->dib_size = 0x28;
    newImage->header->bpp = 0x18;
    newImage->header->planes = 0x01;
    newImage->header->image_size = image->header->size - 0x36;

    for (size_t w = 0; w < width; w++) {
        for (size_t h = 0; h < height; h++) {
            newImage->data[(h * width) + w].blue = image->data[(h * width) + w].blue & blue;
            newImage->data[(h * width) + w].red = image->data[(h * width) + w].red & red;
            newImage->data[(h * width) + w].green = image->data[(h * width) + w].green & green;
        }
    }

    return newImage;
}
