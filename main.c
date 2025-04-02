#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

// Définition des kernels 3x3 pour les filtres
float box_blur_kernel[3][3] = {
    {1 / 9.0, 1 / 9.0, 1 / 9.0},
    {1 / 9.0, 1 / 9.0, 1 / 9.0},
    {1 / 9.0, 1 / 9.0, 1 / 9.0}
};

float gaussian_kernel[3][3] = {
    {1 / 16.0, 2 / 16.0, 1 / 16.0},
    {2 / 16.0, 4 / 16.0, 2 / 16.0},
    {1 / 16.0, 2 / 16.0, 1 / 16.0}
};

float outline_kernel[3][3] = {
    {-1, -1, -1},
    {-1,  8, -1},
    {-1, -1, -1}
};

float emboss_kernel[3][3] = {
    {-2, -1, 0},
    {-1,  1, 1},
    { 0,  1, 2}
};

float sharpen_kernel[3][3] = {
    { 0, -1,  0},
    {-1,  5, -1},
    { 0, -1,  0}
};

// Convertit un tableau statique [3][3] en float**
float** toFloatPointer(float kernel[3][3]) {
    float** ptr = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        ptr[i] = kernel[i];
    }
    return ptr;
}

int main() {
    t_bmp8 *img = NULL;
    char filepath[100];
    int choix, choixFiltre;

    while (1) {
        printf("\nVeuillez choisir une option :\n");
        printf("1. Ouvrir une image\n");
        printf("2. Sauvegarder une image\n");
        printf("3. Appliquer un filtre\n");
        printf("4. Afficher les informations de l'image\n");
        printf("5. Quitter\n");
        printf("\n>>> Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("File path: ");
                scanf("%s", filepath);
                if (img) bmp8_free(img);
                img = bmp8_loadImage(filepath);
                if (img)
                    printf("Image loaded successfully!\n");
                else
                    printf("Erreur lors du chargement de l'image.\n");
                break;

            case 2:
                if (!img) {
                    printf("Veuillez charger une image avant.\n");
                    break;
                }
                printf("File path: ");
                scanf("%s", filepath);
                bmp8_saveImage(filepath, img);
                printf("Image saved successfully!\n");
                break;

            case 3:
                if (!img) {
                    printf("Veuillez charger une image avant.\n");
                    break;
                }
                printf("\nVeuillez choisir un filtre :\n");
                printf("1. Negative\n");
                printf("2. Brightness\n");
                printf("3. Black and white\n");
                printf("4. Box Blur\n");
                printf("5. Gaussian blur\n");
                printf("6. Outline\n");
                printf("7. Emboss\n");
                printf("8. Sharpness\n");
                printf("9. Retour\n");
                printf(">>> Votre choix : ");
                scanf("%d", &choixFiltre);

                switch (choixFiltre) {
                    case 1:
                        bmp8_negative(img);
                        printf("Filtre negatif applique avec succes.\n");
                        break;
                    case 2: {
                        int lum;
                        printf("Valeur de luminosite (-255 à 255) : ");
                        scanf("%d", &lum);
                        bmp8_brightness(img, lum);
                        printf("Luminosite appliquee.\n");
                        break;
                    }
                    case 3: {
                        int threshold;
                        printf("Seuil (0 à 255) : ");
                        scanf("%d", &threshold);
                        bmp8_threshold(img, threshold);
                        printf("Filtre noir et blanc applique.\n");
                        break;
                    }
                    case 4: {
                        float** kernel = toFloatPointer(box_blur_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Filtre box blur applique.\n");
                        break;
                    }
                    case 5: {
                        float** kernel = toFloatPointer(gaussian_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Filtre gaussien applique.\n");
                        break;
                    }
                    case 6: {
                        float** kernel = toFloatPointer(outline_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Filtre contour applique.\n");
                        break;
                    }
                    case 7: {
                        float** kernel = toFloatPointer(emboss_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Filtre emboss applique.\n");
                        break;
                    }
                    case 8: {
                        float** kernel = toFloatPointer(sharpen_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Filtre sharpen applique.\n");
                        break;
                    }
                    case 9:
                        break;
                    default:
                        printf("Filtre invalide.\n");
                }
                break;

            case 4:
                if (!img) {
                    printf("Veuillez charger une image avant.\n");
                    break;
                }
                bmp8_printInfo(img);
                break;

            case 5:
                if (img) bmp8_free(img);
                printf("Fin du programme. Au revoir !\n");
                return 0;

            default:
                printf("Choix invalide.\n");
        }
    }
}
