#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>

// Chargement image BMP
t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    t_bmp8 *img = malloc(sizeof(t_bmp8));
    if (!img) {
        fclose(f);
        printf("Erreur allocation mémoire.\n");
        return NULL;
    }

    fread(img->header, sizeof(unsigned char), 54, f);
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];

    if (img->colorDepth != 8) {
        printf("L'image n'est pas en niveaux de gris 8 bits.\n");
        free(img);
        fclose(f);
        return NULL;
    }

    fread(img->colorTable, sizeof(unsigned char), 1024, f);
    img->data = malloc(img->dataSize);
    fread(img->data, sizeof(unsigned char), img->dataSize, f);

    fclose(f);
    return img;
}

// Sauvegarde image BMP
void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Impossible de creer le fichier %s\n", filename);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, f);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, f);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, f);

    fclose(f);
}

// Libération mémoire
void bmp8_free(t_bmp8 *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

// Affichage infos
void bmp8_printInfo(t_bmp8 *img) {
    printf("\nInformations de l'image :\n");
    printf("Largeur      : %u pixels\n", img->width);
    printf("Hauteur      : %u pixels\n", img->height);
    printf("Profondeur   : %u bits\n", img->colorDepth);
    printf("Taille image : %u octets\n", img->dataSize);
}

// Filtre négatif
void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; i++)
        img->data[i] = 255 - img->data[i];
}

// Ajustement de la luminosité
void bmp8_brightness(t_bmp8 *img, int value) {
    int temp;
    for (unsigned int i = 0; i < img->dataSize; i++) {
        temp = img->data[i] + value;
        if (temp > 255) temp = 255;
        else if (temp < 0) temp = 0;
        img->data[i] = (unsigned char)temp;
    }
}

// Conversion noir et blanc
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; i++)
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
}

