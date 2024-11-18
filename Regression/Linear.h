#ifndef Linear
#define Linear()
#include <stdio.h>

typedef struct {
  float slope;
  float intercept;
} Beta;

Beta* Fit_Model(float[], float[], size_t, size_t);
float* Predict_Model(float[], size_t, Beta);
float Cost_Function(float[], float*,size_t,size_t);
void Stochastic_Gradient_Descent(float [], float [],Beta*,size_t,int,float);
void Free_Model(Beta*);

#endif
