/* DataAnalysis.c */
#include "DataAnalysis.h"
#include "../Regression/memory_deallocation.h"
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LINE_LENGTH 1024

/* Returns the Independent and Dependent variable with the Dataset size */
getFile *Read_Dataset(const char *file_name, const int independent_var,
                      const int target_var) {
        // Create an Instances
        FILE *file = fopen(file_name, "r");
        if (!file) {
                fprintf(stderr, "Failed to open file %s\n", file_name);
                return 0;
        }

        if (fseek(file, 0, SEEK_END) != 0) {
                perror("Error seeking to end");
                fclose(file);
        }

        // get file size
        long get_file_size = ftell(file);

        if (get_file_size == -1L) {
                perror("Error getting file size");
        }

        printf("Wait Opening the file ...\n");
        printf("The size of the Dataset: %ld\n", get_file_size);
        fseek(file, 0, SEEK_SET);

        int maxEnt = get_file_size / (2 * sizeof(getFile));
        getFile *dependencies = (getFile *)malloc(sizeof(getFile));

        if (!dependencies) {
                fprintf(stderr, "DataAnalysis().Read_Dataset() Error: Memory "
                                "allocation failed\n");
                fclose(file);
                free(dependencies);
                return NULL;
        }

        dependencies->X = (float *)malloc(maxEnt * sizeof(float));
        dependencies->Y = (float *)malloc(maxEnt * sizeof(float));

        if (!dependencies->X || !dependencies->Y) {
                fprintf(stderr, "DataAnalysis().Read_Dataset() Error: Memory "
                                "not allocated for "
                                "Dependencies.X and Dependencies.Y\n");
                free(dependencies->X);
                free(dependencies->Y);
                free(dependencies);
                return NULL;
        }

        char buffer[MAX_LINE_LENGTH];

        int row = 0;
        int column = 0;

        while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
                column = 0;
                row++;

                // Skip the Headers
                if (row == 0)
                        continue;

                // Store each row with comma separaed
                char *value = strtok(buffer, ",");

                while (value) {
                        // Select particular Column for Feature extraction
                        if (column == independent_var) {
                                // Assign it to Dependent and Independent
                                // Variables
                                dependencies->X[row - 1] = atof(value);
                        }
                        if (column == target_var) {
                                dependencies->Y[row - 1] = atof(value);
                        }
                        value = strtok(NULL, ",");
                        column++;
                }
                printf("\n");
        }
        dependencies->num_rows = row;

        fclose(file);

        return dependencies;
}

// Split dataset into Train and Test based on the relevant ratio
SplitData *Split_Dataset(float *X, float *Y, size_t n, float train_ratio) {
        SplitData *split_data = (SplitData *)malloc(sizeof(SplitData));

        // If memory is not allocated
        if (!split_data) {
                fprintf(stderr,
                        "Split_Dataset() Error: Memory is not allocated\n");
                exit(1);
        }

        if (X == NULL || Y == NULL) {
                fprintf(stderr, "Error X and Y both are null");
        }

        split_data->train_size = (size_t)(n * train_ratio);
        split_data->test_size = n - split_data->train_size;

        split_data->X_Train =
            (float *)malloc(split_data->train_size * sizeof(float));
        split_data->Y_Train =
            (float *)malloc(split_data->train_size * sizeof(float));
        split_data->X_Test =
            (float *)malloc(split_data->test_size * sizeof(float));
        split_data->Y_Test =
            (float *)malloc(split_data->test_size * sizeof(float));

        if (!split_data->X_Train || !split_data->Y_Train ||
            !split_data->X_Test || !split_data->Y_Test) {
                fprintf(stderr, "Memory allocation is failed for Split Data\n");
                Free_Split(split_data);
                exit(1);
        }

        size_t *indicies = (size_t *)malloc(n * sizeof(size_t));
        for (size_t i = 0; i < n; i++) {
                indicies[i] = i;
        }

        srand(time(NULL));
        for (size_t i = n - 1; i > 0; i--) {
                size_t j = rand() % (i + 1);
                size_t temp = indicies[i];
                indicies[i] = indicies[j];
                indicies[j] = temp;
        }

        for (int i = 0; i < (int)split_data->train_size; i++) {
                split_data->X_Train[i] = X[indicies[i]];
                split_data->Y_Train[i] = Y[indicies[i]];
        }
        for (int i = 0; i < (int)split_data->test_size; i++) {
                split_data->X_Test[i] = X[indicies[split_data->train_size + i]];
                split_data->Y_Test[i] = Y[indicies[split_data->train_size + i]];
        }

        free(indicies);
        return split_data;
}

// returns the data-point into 0-1 for efficient result
NormVar *Normalize(float X[], float Y[], size_t n) {
        if (X == NULL || Y == NULL || n == 0) {
                fprintf(stderr,
                        "Invalid input parameters in Normalize function");
                return NULL;
        }

        // Scaling is little difficult to implement for me ?
        NormVar normalize;
        normalize.x_max = X[0];
        normalize.x_min = X[0];
        normalize.y_max = Y[0];
        normalize.y_min = Y[0];

        for (int i = 0; i < (int)n; i++) {
                // Update Y min/max
                if (Y[i] < normalize.y_min)
                        normalize.y_min = Y[i];
                if (Y[i] > normalize.y_max)
                        normalize.y_max = Y[i];
                // Update X min/max
                if (X[i] < normalize.x_min)
                        normalize.x_min = X[i];
                if (X[i] > normalize.x_max)
                        normalize.x_max = X[i];
        }

        if (normalize.y_max <= normalize.y_min ||
            normalize.x_max <= normalize.x_min) {
                fprintf(stderr, "Error: Invalid min/max values found:\n");
                fprintf(stderr, "Y range: [%.2f, %.2f]\n", normalize.y_min,
                        normalize.y_max);
                fprintf(stderr, "X range: [%.2f, %.2f]\n", normalize.x_min,
                        normalize.x_max);
                return NULL;
        }

        // Allocate memory for normalize function
        NormVar *norm = (NormVar *)malloc(sizeof(NormVar));

        if (!norm) {
                fprintf(stderr, "Error Normalize().X().Y() returned null\n");
                return NULL;
        }

        norm->X = (float *)malloc(n * sizeof(float));
        norm->Y = (float *)malloc(n * sizeof(float));

        if (!norm->X || !norm->Y) {
                fprintf(stderr, "Memory Allocation failed for X and Y\n");
                free(norm->X);
                free(norm->Y);
                free(norm);
                return NULL;
        }

        for (size_t i = 0; i < n; i++) {
                // make sure you are creating and initializing the pointer
                // correctly
                norm->X[i] = (normalize.x_max == normalize.x_min)
                                 ? 0.5
                                 : (X[i] - normalize.x_min) /
                                       (normalize.x_max - normalize.x_min);
                norm->Y[i] = (normalize.y_max == normalize.y_min)
                                 ? 0.5
                                 : (Y[i] - normalize.y_min) /
                                       (normalize.y_max - normalize.y_min);
                if (norm->X[i] < 0 || norm->X[i] > 1 || norm->Y[i] < 0 ||
                    norm->Y[i] > 1) {
                        fprintf(stderr,
                                "Warning: Normalized value out of range at "
                                "index %zu\n",
                                i);
                        fprintf(stderr, "X: %.2f, Y: %.2f\n", norm->X[i],
                                norm->Y[i]);
                }
        }

        printf("Normlaize Function: \n");
        printf("X: [%.2f, %.2f] -> [0, 1]\n", normalize.x_min, normalize.x_max);
        printf("Y: [%.2f, %.2f] -> [0, 1]\n", normalize.y_min, normalize.y_max);

        return norm;
}

// Denormalize is used to convert back to original value, i.e Example 0.80
// ->989.00
float *Denormalize(float normalize_Value[], float y_max, float y_min,
                   size_t n) {
        if (normalize_Value == NULL || n == 0) {
                fprintf(stderr,
                        "Invalid input parameters in Denormalize function\n");
                return NULL;
        }

        if (y_min > y_max) {
                float temp = y_max;
                y_max = y_min;
                y_min = temp;
                fprintf(stderr,
                        "Warning: y_min and y_max were swapped. Correct "
                        "values: y_min = %.2f, y_max = %.2f\n",
                        y_min, y_max);
        }

        if (y_max == y_min) {
                fprintf(
                    stderr,
                    "Error: y_max equals y_min (%.2f), cannot denormalize\n",
                    y_max);
                return NULL;
        }

        float *original_value = (float *)malloc(n * sizeof(float));

        if (!original_value) {
                fprintf(stderr, "Memory allocation failed in Denormalize\n");
                return NULL;
        }
        printf("Denormalization parameters:\n");
        printf("y_min: %.2f, y_max: %.2f, range: %.2f\n", y_min, y_max,
               y_max - y_min);

        for (size_t i = 0; i < n; i++) {
                if (normalize_Value[i] < 0 || normalize_Value[i] > 1) {
                        fprintf(stderr,
                                "Warning: Normalized value at index %zu is "
                                "outside [0,1] range: %.2f\n",
                                i, normalize_Value[i]);
                        float clamped =
                            normalize_Value[i] < 0
                                ? 0
                                : (normalize_Value[i] > 1 ? 1
                                                          : normalize_Value[i]);
                        original_value[i] = clamped * (y_max - y_min) + y_min;
                } else {

                        original_value[i] =
                            normalize_Value[i] * (y_max - y_min) + y_min;
                }

                // Print first few values for debugging
                if (i < 5) {
                        printf(
                            "Index %zu: Normalized=%.2f, Denormalized=%.2f\n",
                            i, normalize_Value[i], original_value[i]);
                }
        }

        return original_value;
}
