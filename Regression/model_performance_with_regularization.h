#ifndef CostFunction
#define CostFunction

#include <stdbool.h>
#include <stddef.h>

typedef struct {
        float accuracy;
        bool is_valid;
} metricResult;

metricResult Mean_Absolute_Error(float *, float *, size_t);
metricResult Mean_Squared_Error(float *, float *, size_t);
metricResult Root_Mean_Squared_Error(float *, float *, size_t);
float Lasso_Regularization(float *weights, size_t num_weights, float lambda);
float Ridge_Regularization(float *weights, size_t num_weights, float lambda);

#endif
