
#include "model_performance_with_regularization.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

metricResult Mean_Squared_Error(float *actualValue, float *predictValue,
                                size_t x) {
        metricResult result = {0.0, false};
        float sum = 0.0;
        int validCount = 0;

        for (size_t i = 0; i < x; i++) {
                if (!actualValue || !predictValue) {
                        fprintf(stderr,
                                "model_performance_with_regularization().MSE() "
                                "Error Line.14: 'Value is null.\n'");
                        result.is_valid = false;
                        return result;
                }
                float error = actualValue[i] - predictValue[i];
                sum += error * error;
                validCount++;
        }

        if (validCount == 0) {
                fprintf(stderr, "model_performance_with_regularization().MSE() "
                                "Error Line.25: 'Error in Valid Count.'\n");
                result.is_valid = false;
                return result;
        }

        result.accuracy = sum / validCount;
        result.is_valid = true;

        return result;
}

metricResult Mean_Absolute_Error(float *actualValue, float *predictValue,
                                 size_t x) {
        metricResult result = {0.0, false};

        float sum = 0.0;
        int validCount = 0;

        for (size_t i = 0; i < x; i++) {
                if (!actualValue || !predictValue) {
                        fprintf(stderr,
                                "model_performance_with_regularization().MAE() "
                                "Error Line.49: 'Value is null.\n'");
                        result.is_valid = false;
                        return result;
                }
                float error = actualValue[i] - predictValue[i];
                sum += error * error;
                validCount++;
        }

        if (validCount == 0) {
                fprintf(stderr, "model_performance_with_regularization().MAE() "
                                "Error Line.60: 'Error in Valid Count.'\n");
                result.is_valid = false;
                return result;
        }

        result.accuracy = sum / validCount;
        result.is_valid = true;
        return result;
}

metricResult Root_Mean_Squared_Error(float *actualValue, float *predictValue,
                                     size_t x) {
        metricResult result = {0.0, false};
        float sum = 0.0;
        int validCount = 0;

        for (size_t i = 0; i < x; i++) {
                if (!actualValue || !predictValue) {
                        fprintf(
                            stderr,
                            "model_performance_with_regularization().RMSE() "
                            "Error Line.79: 'Value is null.\n'");
                        result.is_valid = false;
                        return result;
                }
                float error = actualValue[i] - predictValue[i];
                sum += error * error;
                validCount++;
        }
        if (validCount == 0) {
                fprintf(stderr,
                        "model_performance_with_regularization().RMSE() "
                        "Error Line.91: 'Error in Valid Count.'\n");
                result.is_valid = false;
                return result;
        }

        result.accuracy = sqrt(sum / validCount);
        result.is_valid = true;
        return result;
}

// Regularization Methods: L1 and L2;
float Lasso_Regularization(Beta *model, float lambda) {
        if (!model) {
                fprintf(stderr, "Lasso Regularization Error: Model is Null.\n");
                return 0.0;
        }
        float lasso_penalty = fabs(model->slope) + fabs(model->intercept);
        return lambda * lasso_penalty;
}

float Ridge_Regularization(Beta *model, float lambda) {
        if (!model) {
                fprintf(stderr, "Ridge Regularization Error: Model is Null.\n");
                return 0.0;
        }

        float ridge_penalty = (model->slope * model->slope) +
                              (model->intercept * model->intercept);
        return lambda * ridge_penalty;
}
