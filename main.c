#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

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
                if (img) {
                    printf("Image loaded successfully!\n");
                } else {
                    printf("Erreur lors du chargement de l'image.\n");
                }
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
                printf("9. Return to the previous menu\n");
                printf("\n>>> Votre choix : ");
                scanf("%d", &choixFiltre);

                switch (choixFiltre) {
                    case 1:
                        bmp8_negative(img);
                        printf("Filter applied successfully!\n");
                        break;
                    case 2: {
                        int lum;
                        printf("Brightness value (-255 to 255): ");
                        scanf("%d", &lum);
                        bmp8_brightness(img, lum);
                        printf("Brightness adjusted successfully!\n");
                        break;
                    }
                    case 3: {
                        int seuil;
                        printf("Threshold (0 to 255): ");
                        scanf("%d", &seuil);
                        bmp8_threshold(img, seuil);
                        printf("Threshold filter applied successfully!\n");
                        break;
                    }
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                        printf("Filtre a implementer plus tard (convolution).\n");
                        break;
                    case 9:
                        printf("Retour au menu principal.\n");
                        break;
                    default:
                        printf("Choix invalide.\n");
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
                printf("Fin du programme. Au revoir!\n");
                exit(0);

            default:
                printf("Choix invalide, veuillez reessayer.\n");
        }
    }
}
