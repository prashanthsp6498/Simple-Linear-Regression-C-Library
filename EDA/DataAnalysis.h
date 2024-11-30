#ifndef DataAnalysis
#define DataAnalysis
#include <stdio.h>
#define MAX_SIZE 1024

typedef struct {
  float *X;
  float *Y;
  size_t num_rows;
} getFile;

typedef struct {
  float *X_Train;
  float *Y_Train;
  float *X_Test;
  float *Y_Test;
  size_t train_size;
  size_t test_size;
} SplitData;

typedef struct {
  float *X;
  float *Y;
} NormVar;

getFile *Read_Dataset(const char *file_name, const int Data_feature,
                      const int target_feature);

NormVar *Normalize(float[], float[], size_t, float *, float *);

float *Denormalize(float[], float, float, size_t);

SplitData Split_Dataset(float[], float[], size_t, float);

#endif
