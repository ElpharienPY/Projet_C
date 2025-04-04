#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Load an 8-bit grayscale BMP image from disk
t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *f = fopen(filename, "rb"); // Open file in binary read mode
    if (!f) {
        printf("Unable to open file %s\n", filename);
        return NULL;
    }

    t_bmp8 *img = malloc(sizeof(t_bmp8)); // Allocate memory for image structure
    if (!img) {
        fclose(f);
        printf("Memory allocation error.\n");
        return NULL;
    }

    // Read the 54-byte BMP header
    fread(img->header, sizeof(unsigned char), 54, f);

    // Extract image metadata from header
    img->width      = *(unsigned int *)&img->header[18];  // Image width in pixels
    img->height     = *(unsigned int *)&img->header[22];  // Image height in pixels
    img->colorDepth = *(unsigned short *)&img->header[28]; // Bits per pixel (must be 8)
    img->dataSize   = *(unsigned int *)&img->header[34];  // Size of pixel data in bytes

    // Ensure the image is in 8-bit grayscale format
    if (img->colorDepth != 8) {
        printf("Image is not in 8-bit grayscale format.\n");
        free(img);
        fclose(f);
        return NULL;
    }

    // Read the color palette (1024 bytes = 256 colors × 4 bytes per entry)
    fread(img->colorTable, sizeof(unsigned char), 1024, f);

    // Allocate memory for pixel data
    img->data = malloc(img->dataSize);
    fread(img->data, sizeof(unsigned char), img->dataSize, f); // Read pixel data

    fclose(f); // Close the file
    return img;
}

// Save an 8-bit grayscale BMP image to disk
void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *f = fopen(filename, "wb"); // Open file in binary write mode
    if (!f) {
        printf("Unable to create file %s\n", filename);
        return;
    }

    // Write the BMP header, color palette, and pixel data
    fwrite(img->header, sizeof(unsigned char), 54, f);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, f);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, f);

    fclose(f); // Close the file
}

// Free the memory allocated for an image
void bmp8_free(t_bmp8 *img) {
    if (img) {
        free(img->data); // Free the pixel data
        free(img);       // Free the structure itself
    }
}

// Print image information (dimensions, depth, size)
void bmp8_printInfo(t_bmp8 *img) {
    printf("\nImage information:\n");
    printf("Width       : %u pixels\n", img->width);
    printf("Height      : %u pixels\n", img->height);
    printf("Color depth : %u bits\n", img->colorDepth);
    printf("Data size   : %u bytes\n", img->dataSize);
}

// Apply a negative filter to the image (invert pixel values)
void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

// Adjust the image brightness by adding a value to each pixel
void bmp8_brightness(t_bmp8 *img, int value) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int temp = img->data[i] + value; // Add brightness value
        if (temp > 255) temp = 255;      // Clamp to upper bound
        else if (temp < 0) temp = 0;     // Clamp to lower bound
        img->data[i] = (unsigned char)temp;
    }
}

// Convert the image to black and white using a given threshold
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

// Apply a convolution filter (e.g. blur, sharpen, etc.)
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    int n = kernelSize / 2; // Distance from center of the kernel
    unsigned char *newData = malloc(img->dataSize); // Output pixel data

    if (!newData) {
        printf("Memory allocation error for filtering.\n");
        return;
    }

    // Copy original image into newData to preserve border pixels
    for (unsigned int i = 0; i < img->dataSize; i++) {
        newData[i] = img->data[i];
    }

    // Apply convolution (excluding border pixels)
    for (unsigned int y = n; y < img->height - n; y++) {
        for (unsigned int x = n; x < img->width - n; x++) {
            float pixel = 0.0f;

            // Loop over kernel
            for (int ky = -n; ky <= n; ky++) {
                for (int kx = -n; kx <= n; kx++) {
                    int ix = x + kx; // x position of neighbor
                    int iy = y + ky; // y position of neighbor
                    pixel += img->data[iy * img->width + ix] * kernel[ky + n][kx + n];
                }
            }

            // Clamp and assign result
            if (pixel < 0.0f) pixel = 0.0f;
            if (pixel > 255.0f) pixel = 255.0f;
            newData[y * img->width + x] = (unsigned char)roundf(pixel);
        }
    }

    // Copy result back to original image
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = newData[i];
    }

    free(newData); // Free the temporary buffer
}
