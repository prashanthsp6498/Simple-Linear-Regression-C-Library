/* Linear.h */
#ifndef Linear
#define Linear()
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/* Declare your Struct section here.. */
typedef struct {
        float slope;
        float intercept;
} Beta;

/* Function Declaration */
Beta *Fit_Model(float[], float[], size_t, size_t, int, float, float, float);
float *Predict_Model(float[], size_t, Beta);
void Stochastic_Gradient_Descent(float[], float[], Beta *, size_t, int, float,
                                 float, float);

#endif
