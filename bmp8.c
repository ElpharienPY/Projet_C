#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

// Predefined 3x3 convolution kernels used for different filters
// Float constants used to avoid narrowing conversion warnings
float box_blur_kernel[3][3] = {
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
    {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f}
};

float gaussian_kernel[3][3] = {
    {1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f},
    {2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f},
    {1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f}
};

float outline_kernel[3][3] = {
    {-1.0f, -1.0f, -1.0f},
    {-1.0f,  8.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f}
};

float emboss_kernel[3][3] = {
    {-2.0f, -1.0f,  0.0f},
    {-1.0f,  1.0f,  1.0f},
    { 0.0f,  1.0f,  2.0f}
};

float sharpen_kernel[3][3] = {
    { 0.0f, -1.0f,  0.0f},
    {-1.0f,  5.0f, -1.0f},
    { 0.0f, -1.0f,  0.0f}
};

// Converts a static 3x3 kernel into a float**
// Allocates an array of pointers that point to each row of the static kernel
float** toFloatPointer(float kernel[3][3]) {
    float** ptr = malloc(3 * sizeof(float*));
    if (!ptr) {
        printf("Memory allocation error in toFloatPointer.\n");
        return NULL;
    }
    for (int i = 0; i < 3; i++) {
        ptr[i] = kernel[i]; // Just point to each row; no deep copy
    }
    return ptr;
}

int main(void) {
    t_bmp8 *img = NULL;           // Holds the currently loaded image
    char filepath[100];           // Stores the user-provided file path
    int choix = 0, choixFiltre = 0;  // Menu selections

    while (1) {
        // Display the main menu options
        printf("\nPlease choose an option:\n");
        printf("1. Open an image\n");
        printf("2. Save an image\n");
        printf("3. Apply a filter\n");
        printf("4. Display image information\n");
        printf("5. Quit\n");
        printf("\n>>> Your choice: ");

        // Safely read user input and validate it
        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error.\n");
            break;
        }
        if (sscanf(input, "%d", &choix) != 1 || choix < 1 || choix > 5) {
            printf("Please enter a valid option (1 to 5).\n");
            continue;
        }

        switch (choix) {
            case 1: // Open an image
                printf("File path: ");
                scanf("%s", filepath);
                getchar(); // Clear the newline character from the buffer

                if (img) bmp8_free(img); // Free previously loaded image
                img = bmp8_loadImage(filepath);

                if (img)
                    printf("Image loaded successfully!\n");
                else
                    printf("Error loading image.\n");
                break;

            case 2: // Save the currently loaded image
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

                // Display the list of available filters
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

                // Validate filter input
                char filtreInput[10];
                if (fgets(filtreInput, sizeof(filtreInput), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                if (sscanf(filtreInput, "%d", &choixFiltre) != 1 || choixFiltre < 1 || choixFiltre > 9) {
                    printf("Please enter a valid filter option (1 to 9).\n");
                    break;
                }

                // Apply the selected filter
                switch (choixFiltre) {
                    case 1:
                        bmp8_negative(img);
                        printf("Negative filter applied.\n");
                        break;
                    case 2: {
                        int lum = 0;
                        printf("Brightness value (-255 to 255): ");
                        scanf("%d", &lum);
                        getchar();
                        bmp8_brightness(img, lum);
                        printf("Brightness adjusted.\n");
                        break;
                    }
                    case 3: {
                        int threshold = 0;
                        printf("Threshold value (0 to 255): ");
                        scanf("%d", &threshold);
                        getchar();
                        bmp8_threshold(img, threshold);
                        printf("Threshold filter applied.\n");
                        break;
                    }
                    case 4: {
                        float** kernel = toFloatPointer(box_blur_kernel);
                        if (kernel != NULL) {
                            bmp8_applyFilter(img, kernel, 3);
                            free(kernel);
                        }
                        printf("Box blur applied.\n");
                        break;
                    }
                    case 5: {
                        float** kernel = toFloatPointer(gaussian_kernel);
                        if (kernel != NULL) {
                            bmp8_applyFilter(img, kernel, 3);
                            free(kernel);
                        }
                        printf("Gaussian blur applied.\n");
                        break;
                    }
                    case 6: {
                        float** kernel = toFloatPointer(outline_kernel);
                        if (kernel != NULL) {
                            bmp8_applyFilter(img, kernel, 3);
                            free(kernel);
                        }
                        printf("Outline filter applied.\n");
                        break;
                    }
                    case 7: {
                        float** kernel = toFloatPointer(emboss_kernel);
                        if (kernel != NULL) {
                            bmp8_applyFilter(img, kernel, 3);
                            free(kernel);
                        }
                        printf("Emboss filter applied.\n");
                        break;
                    }
                    case 8: {
                        float** kernel = toFloatPointer(sharpen_kernel);
                        if (kernel != NULL) {
                            bmp8_applyFilter(img, kernel, 3);
                            free(kernel);
                        }
                        printf("Sharpen filter applied.\n");
                        break;
                    }
                    case 9:
                        // Return to main menu
                        break;
                }
                break;

            case 4: // Display image properties (width, height, etc.)
                if (!img) {
                    printf("Please load an image first.\n");
                    break;
                }
                bmp8_printInfo(img);
                break;

            case 5: // Exit the program cleanly
                if (img) bmp8_free(img);
                printf("Program exited. Goodbye!\n");
                return 0;

            default:
                // Should never be reached due to validation
                break;
        }
    }

    return 0;
}
