#include "EDA/DataAnalysis.h"
#include <stddef.h>
#ifndef Linear
#define Linear()
#include <stdbool.h>
#include <stdio.h>

/* Declare your Struct section here.. */
typedef struct {
        float slope;
        float intercept;
} Beta;

// Here Declare you function with arguments
Beta *Fit_Model(float[], float[], size_t, size_t, int, float, float, float);
float *Predict_Model(float[], size_t, Beta);
void Stochastic_Gradient_Descent(float[], float[], Beta *, size_t, int, float,
                                 float, float);
void Free_Model(Beta *);
void Free_Data(getFile *);
void Free_Normalize(NormVar *);
void Free_Split(SplitData *);

#endif
