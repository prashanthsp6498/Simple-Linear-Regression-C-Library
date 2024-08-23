#include "DataAnalysis.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024

getFile *Read_Dataset(const char file_name[100], const int independent_var,
                      const int dependent_var) {
  // Create an Instances
  FILE *file = fopen(file_name, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file %s\n", file_name);
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

  printf("Opening the File");
  fseek(file, 0, SEEK_SET);

  int maxEnt = get_file_size / (2 * sizeof(float));
  getFile *dependencies = malloc(maxEnt * sizeof(getFile));

  if (dependencies == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(file);
    free(dependencies);
  }
  char buffer[MAX_LINE_LENGTH];

  int row = 0;
  int column = 0;

  while (fgets(buffer, maxEnt, file) != NULL) {
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
        // Assign it to Dependent and Independent Variables
        dependencies->X[row - 1] = atof(value);
      }
      if (column == dependent_var) {
        dependencies->Y[row - 1] = atof(value);
      }
      value = strtok(NULL, ",");
      column++;
    }
    printf("\n");
  }
  dependencies->num_rows = row;
  Normalize(dependencies->X, dependencies->Y, dependencies->num_rows);

  fclose(file);

  return (dependencies);
}

void Normalize(float X[], float Y[], size_t n) {

  float x_min = FLT_MAX;
  float x_max = -FLT_MAX;
  float y_min = FLT_MAX;
  float y_max = -FLT_MAX;

  for (size_t i = 0; i < n; i++) {
    if (X[i] < x_min)
      x_min = X[i];

    if (X[i] > x_max)
      x_max = X[i];
    if (Y[i] > y_max)
      y_max = Y[i];
    if (Y[i] < y_min)
      y_min = Y[i];
  }

  for (size_t i = 0; i < n; i++) {
    X[i] = (X[i] - x_min) / (x_max - x_min);
    Y[i] = (Y[i] - y_min) / (y_max - y_min);
  }
}

float deNormalize(float Y) {
  float y_min = FLT_MAX;
  float y_max = -FLT_MAX;
  if (Y < y_min)
    y_min = Y;
  if (Y > y_max)
    y_max = Y;
  float pred = (Y * (y_max - y_min) + y_min);

  return pred;
  ;
}
