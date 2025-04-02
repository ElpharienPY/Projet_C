#ifndef BMP8_H
#define BMP8_H

// Structure pour image BMP 8 bits grayscale
typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char *data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;

// Prototypes des fonctions
t_bmp8 *bmp8_loadImage(const char *filename);
void bmp8_saveImage(const char *filename, t_bmp8 *img);
void bmp8_free(t_bmp8 *img);
void bmp8_printInfo(t_bmp8 *img);

// Fonctions de traitement d'image
void bmp8_negative(t_bmp8 *img);
void bmp8_brightness(t_bmp8 *img, int value);
void bmp8_threshold(t_bmp8 *img, int threshold);

// Fonction de filtrage
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize);

#endif // BMP8_H
