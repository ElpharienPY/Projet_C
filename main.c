#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

// 3x3 filter kernels
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

// Helper to convert static 2D array to float**
float** toFloatPointer(float kernel[3][3]) {
    float** ptr = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        ptr[i] = kernel[i];
    }
    return ptr;
}

int main() {
    t_bmp8 *img = NULL;         // Pointer to the loaded image
    char filepath[100];         // To store image path input
    int choix, choixFiltre;     // Main menu and filter menu choices

    while (1) {
        // Display the main menu
        printf("\nPlease choose an option:\n");
        printf("1. Open an image\n");
        printf("2. Save an image\n");
        printf("3. Apply a filter\n");
        printf("4. Display image information\n");
        printf("5. Quit\n");
        printf("\n>>> Your choice: ");

        // Read user input safely (avoid infinite loop) ---
        char input[10];
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &choix) != 1 || choix < 1 || choix > 5) {
            printf("Please enter a valid option (1 to 5).\n");
            continue;
        }

        switch (choix) {
            case 1: // Open an image
                printf("File path: ");
                scanf("%s", filepath);
                getchar(); // Clear newline
                if (img) bmp8_free(img);
                img = bmp8_loadImage(filepath);
                if (img)
                    printf("Image loaded successfully!\n");
                else
                    printf("Error loading image.\n");
                break;

            case 2: // Save an image
                if (!img) {
                    printf("Please load an image first.\n");
                    break;
                }
                printf("File path: ");
                scanf("%s", filepath);
                getchar(); // Clear newline
                bmp8_saveImage(filepath, img);
                printf("Image saved successfully!\n");
                break;

            case 3: // Apply a filter
                if (!img) {
                    printf("Please load an image first.\n");
                    break;
                }

                // Display the filter menu
                printf("\nChoose a filter to apply:\n");
                printf("1. Negative\n");
                printf("2. Brightness\n");
                printf("3. Black and white\n");
                printf("4. Box Blur\n");
                printf("5. Gaussian Blur\n");
                printf("6. Outline\n");
                printf("7. Emboss\n");
                printf("8. Sharpen\n");
                printf("9. Return to main menu\n");
                printf(">>> Your choice: ");

                char filtreInput[10];
                fgets(filtreInput, sizeof(filtreInput), stdin);
                if (sscanf(filtreInput, "%d", &choixFiltre) != 1 || choixFiltre < 1 || choixFiltre > 9) {
                    printf("Please enter a valid filter option (1 to 9).\n");
                    break;
                }

                switch (choixFiltre) {
                    case 1:
                        bmp8_negative(img);
                        printf("Negative filter applied.\n");
                        break;
                    case 2: {
                        int lum;
                        printf("Brightness value (-255 to 255): ");
                        scanf("%d", &lum);
                        getchar();
                        bmp8_brightness(img, lum);
                        printf("Brightness adjusted.\n");
                        break;
                    }
                    case 3: {
                        int threshold;
                        printf("Threshold value (0 to 255): ");
                        scanf("%d", &threshold);
                        getchar();
                        bmp8_threshold(img, threshold);
                        printf("Threshold filter applied.\n");
                        break;
                    }
                    case 4: {
                        float** kernel = toFloatPointer(box_blur_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Box blur applied.\n");
                        break;
                    }
                    case 5: {
                        float** kernel = toFloatPointer(gaussian_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Gaussian blur applied.\n");
                        break;
                    }
                    case 6: {
                        float** kernel = toFloatPointer(outline_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Outline filter applied.\n");
                        break;
                    }
                    case 7: {
                        float** kernel = toFloatPointer(emboss_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Emboss filter applied.\n");
                        break;
                    }
                    case 8: {
                        float** kernel = toFloatPointer(sharpen_kernel);
                        bmp8_applyFilter(img, kernel, 3);
                        free(kernel);
                        printf("Sharpen filter applied.\n");
                        break;
                    }
                    case 9:
                        // Go back to main menu
                        break;
                }
                break;

            case 4: // Display image info
                if (!img) {
                    printf("Please load an image first.\n");
                    break;
                }
                bmp8_printInfo(img);
                break;

            case 5: // Exit
                if (img) bmp8_free(img);
                printf("Program exited. Goodbye!\n");
                return 0;
        }
    }
}
