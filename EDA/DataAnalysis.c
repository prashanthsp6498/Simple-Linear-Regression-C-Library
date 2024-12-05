#include "DataAnalysis.h"
#include "../Regression/Linear.h"
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_LINE_LENGTH 1024

// Read the Dataset Files
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

        for (size_t i = 0; i < split_data->train_size; i++) {
                split_data->X_Train[i] = X[indicies[i]];
                split_data->Y_Train[i] = Y[indicies[i]];
        }
        for (size_t i = 0; i < split_data->test_size; i++) {
                *split_data->X_Test = X[indicies[split_data->train_size + i]];
                *split_data->Y_Test = Y[indicies[split_data->train_size + i]];
        }

        free(indicies);
        return split_data;
}

// Normalize the data-point into 0-1 for efficient result
NormVar *Normalize(float X[], float Y[], size_t n, float *y_min, float *y_max) {

        // Scaling is little difficult to implement for me ?
        float x_min = FLT_MAX;
        float x_max = -FLT_MAX;

        if (X == NULL || Y == NULL) {
                fprintf(stderr, "X or Y is Null");
                return NULL;
        }

        NormVar *norm = (NormVar *)malloc(sizeof(NormVar));

        if (!norm) {
                fprintf(stderr, "Error Normalize().X().Y() returned null\n");
                free(norm);
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

        *y_min = FLT_MAX;
        *y_max = -FLT_MAX;

        for (size_t i = 0; i < n; i++) {
                if (X[i] < x_min)
                        x_min = X[i];

                if (X[i] > x_max)
                        x_max = X[i];
                if (Y[i] > *y_max)
                        *y_max = Y[i];
                if (Y[i] < *y_min)
                        *y_min = Y[i];
        }

        if (x_max == x_min) {
                fprintf(stderr,
                        "Error all x values are the same. Cannot Normalize\n");
                return NULL;
        }
        if (*y_max == *y_min) {
                fprintf(stderr,
                        "Error all y values are the same. Cannot Normalize\n");
                return NULL;
        }

        for (size_t i = 0; i < n; i++) {
                // make sure you are creating and initializing the pointer
                // correctly
                norm->X[i] = (X[i] - x_min) / (x_max - x_min);
                norm->Y[i] = (Y[i] - *y_min) / (*y_max - *y_min);
        }

        if (norm->X == 0 || norm->Y == 0) {
                printf("X and Y values are %f %f", *norm->X, *norm->Y);
        }

        return norm;
}

// Denormalize is used to convert back to original value, i.e Example 0.80 ->
// 989.00
float *Denormalize(float normalize_Value[], float y_max, float y_min,
                   size_t n) {
        float *original_value = (float *)malloc(n * sizeof(float));
        for (size_t i = 0; i < n; i++) {
                original_value[i] =
                    normalize_Value[i] * (y_max - y_min) + y_min;
        }
        return original_value;
}
