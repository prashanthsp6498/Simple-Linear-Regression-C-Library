#include "Linear.h"
#ifndef model_performance_with_regularization
#define model_performance_with_regularization()

#include <stdbool.h>
#include <stddef.h>

typedef struct {
        float accuracy;
        bool is_valid;
} metricResult;

metricResult Mean_Absolute_Error(float *, float *, size_t);
metricResult Mean_Squared_Error(float *, float *, size_t);
metricResult Root_Mean_Squared_Error(float *, float *, size_t);
metricResult R_Square(float *, float *, size_t);
float Lasso_Regularization(Beta *, float);
float Ridge_Regularization(Beta *, float);
float ElasticNet_Regularization(Beta *, float, float);

#endif
