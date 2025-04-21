#include "bmp24.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// === Allocation / Libération ===
t_pixel **bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (!pixels) return NULL;
    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            for (int j = 0; j < i; j++) free(pixels[j]);
            free(pixels);
            return NULL;
        }
    }
    return pixels;
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    for (int i = 0; i < height; i++) free(pixels[i]);
    free(pixels);
}

void bmp24_free(t_bmp24 *img) {
    if (img) {
        bmp24_freeDataPixels(img->data, img->height);
        free(img);
    }
}

// === Chargement BMP 24 bits (lecture manuelle des en-têtes) ===
t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("Erreur ouverture fichier %s\n", filename);
        return NULL;
    }

    // Lecture des champs critiques
    uint16_t type;
    int32_t width, height;
    uint16_t bits;
    uint32_t compression, offset;

    fseek(f, 0, SEEK_SET);
    fread(&type, sizeof(uint16_t), 1, f);

    fseek(f, 18, SEEK_SET);
    fread(&width, sizeof(int32_t), 1, f);
    fread(&height, sizeof(int32_t), 1, f);

    fseek(f, 28, SEEK_SET);
    fread(&bits, sizeof(uint16_t), 1, f);

    fseek(f, 30, SEEK_SET);
    fread(&compression, sizeof(uint32_t), 1, f);

    fseek(f, 10, SEEK_SET);
    fread(&offset, sizeof(uint32_t), 1, f);

    if (type != 0x4D42 || bits != 24 || compression != 0) {
        printf("Fichier incompatible. BMP 24 bits non compressé requis.\n");
        fclose(f);
        return NULL;
    }

    // Allocation
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    img->width = width;
    img->height = height;
    img->colorDepth = bits;
    img->data = bmp24_allocateDataPixels(width, height);
    if (!img->data) {
        fclose(f);
        free(img);
        return NULL;
    }

    // Lecture des pixels
    fseek(f, offset, SEEK_SET);
    int padding = (4 - (width * 3) % 4) % 4;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char bgr[3];
            fread(bgr, 1, 3, f);
            img->data[height - 1 - y][x].blue = bgr[0];
            img->data[height - 1 - y][x].green = bgr[1];
            img->data[height - 1 - y][x].red = bgr[2];
        }
        fseek(f, padding, SEEK_CUR);
    }

    fclose(f);
    printf("Image loaded : %dx%d\n", width, height);
    return img;
}

// === Sauvegarde d'une image BMP 24 bits ===
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Erreur écriture fichier %s\n", filename);
        return;
    }

    // Écriture header (14 octets)
    uint16_t type = 0x4D42;
    uint32_t offset = 54;
    uint32_t size = offset + (img->width * 3 + (4 - (img->width * 3 % 4)) % 4) * img->height;
    uint16_t reserved = 0;

    fwrite(&type, sizeof(uint16_t), 1, f);
    fwrite(&size, sizeof(uint32_t), 1, f);
    fwrite(&reserved, sizeof(uint16_t), 1, f);
    fwrite(&reserved, sizeof(uint16_t), 1, f);
    fwrite(&offset, sizeof(uint32_t), 1, f);

    // Écriture info header (40 octets)
    uint32_t headerSize = 40;
    uint16_t planes = 1;
    uint16_t bits = 24;
    uint32_t compression = 0;
    uint32_t imageSize = size - offset;
    int32_t resolution = 2835;

    fwrite(&headerSize, sizeof(uint32_t), 1, f);
    fwrite(&img->width, sizeof(int32_t), 1, f);
    fwrite(&img->height, sizeof(int32_t), 1, f);
    fwrite(&planes, sizeof(uint16_t), 1, f);
    fwrite(&bits, sizeof(uint16_t), 1, f);
    fwrite(&compression, sizeof(uint32_t), 1, f);
    fwrite(&imageSize, sizeof(uint32_t), 1, f);
    fwrite(&resolution, sizeof(int32_t), 1, f);
    fwrite(&resolution, sizeof(int32_t), 1, f);
    fwrite(&compression, sizeof(uint32_t), 1, f); // ncolors = 0
    fwrite(&compression, sizeof(uint32_t), 1, f); // important colors = 0

    // Écriture pixels
    int padding = (4 - (img->width * 3) % 4) % 4;
    unsigned char pad[3] = {0, 0, 0};

    for (int y = img->height - 1; y >= 0; y--) {
        for (int x = 0; x < img->width; x++) {
            unsigned char bgr[3] = {
                img->data[y][x].blue,
                img->data[y][x].green,
                img->data[y][x].red
            };
            fwrite(bgr, 1, 3, f);
        }
        fwrite(pad, 1, padding, f);
    }

    fclose(f);
    printf("Image enregistrée dans %s\n", filename);
}

// === Filtres simples ===
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];
            p->red = 255 - p->red;
            p->green = 255 - p->green;
            p->blue = 255 - p->blue;
        }
    }
}

void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];
            uint8_t g = (p->red + p->green + p->blue) / 3;
            p->red = p->green = p->blue = g;
        }
    }
}

void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *p = &img->data[y][x];
            p->red = fminf(fmaxf(p->red + value, 0), 255);
            p->green = fminf(fmaxf(p->green + value, 0), 255);
            p->blue = fminf(fmaxf(p->blue + value, 0), 255);
        }
    }
}

// === Convolution générique ===
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    int n = kernelSize / 2;
    float r = 0, g = 0, b = 0;

    for (int ky = -n; ky <= n; ky++) {
        for (int kx = -n; kx <= n; kx++) {
            int px = x + kx;
            int py = y + ky;
            if (px >= 0 && px < img->width && py >= 0 && py < img->height) {
                t_pixel p = img->data[py][px];
                float coeff = kernel[ky + n][kx + n];
                r += p.red * coeff;
                g += p.green * coeff;
                b += p.blue * coeff;
            }
        }
    }

    t_pixel result;
    result.red = (uint8_t)fminf(fmaxf(r, 0), 255);
    result.green = (uint8_t)fminf(fmaxf(g, 0), 255);
    result.blue = (uint8_t)fminf(fmaxf(b, 0), 255);
    return result;
}

void bmp24_applyFilter(t_bmp24 *img, float **kernel, int kernelSize) {
    t_pixel **newData = bmp24_allocateDataPixels(img->width, img->height);
    if (!newData) return;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            newData[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    bmp24_freeDataPixels(img->data, img->height);
    img->data = newData;
}

// === Filtres prédéfinis ===
void bmp24_boxBlur(t_bmp24 *img) {
    float box[3][3] = {
        {1/9.f, 1/9.f, 1/9.f},
        {1/9.f, 1/9.f, 1/9.f},
        {1/9.f, 1/9.f, 1/9.f}
    };
    float* kernel[3] = { box[0], box[1], box[2] };
    bmp24_applyFilter(img, kernel, 3);
}

void bmp24_gaussianBlur(t_bmp24 *img) {
    float gauss[3][3] = {
        {1/16.f, 2/16.f, 1/16.f},
        {2/16.f, 4/16.f, 2/16.f},
        {1/16.f, 2/16.f, 1/16.f}
    };
    float* kernel[3] = { gauss[0], gauss[1], gauss[2] };
    bmp24_applyFilter(img, kernel, 3);
}

void bmp24_outline(t_bmp24 *img) {
    float outline[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    float* kernel[3] = { outline[0], outline[1], outline[2] };
    bmp24_applyFilter(img, kernel, 3);
}

void bmp24_emboss(t_bmp24 *img) {
    float emboss[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };
    float* kernel[3] = { emboss[0], emboss[1], emboss[2] };
    bmp24_applyFilter(img, kernel, 3);
}

void bmp24_sharpen(t_bmp24 *img) {
    float sharpen[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };
    float* kernel[3] = { sharpen[0], sharpen[1], sharpen[2] };
    bmp24_applyFilter(img, kernel, 3);
}
