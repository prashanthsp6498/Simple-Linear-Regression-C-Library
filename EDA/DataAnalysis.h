#ifndef DataAnalysis
#define DataAnalysis
#include <stdio.h>
#define MAX_SIZE 1024

typedef struct {
  float X[MAX_SIZE];
  float Y[MAX_SIZE];
  size_t num_rows;
} getFile;

getFile *Read_Dataset(const char file_name[100], const int Data_feature,
                      const int target_feature);

void Normalize(float[],float [], size_t);
float deNormalize(float);

#endif
