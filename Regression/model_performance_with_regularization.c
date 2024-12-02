
#include "model_performance_with_regularization.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

metricResult MSE(float *actualValue, float *predictValue, size_t x) {
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
float Lasso_Regularization(float *weights, size_t num_weights, float lambda) {
        float lasso_penalty = 0.0;
        for (size_t i = 0; i < num_weights; i++) {
                lasso_penalty += fabs(weights[i]);
        }
        return lambda * lasso_penalty;
}

float Ridge_Regularization(float *weights, size_t num_weights, float lambda) {
        float ridge_penalty = 0.0;
        for (size_t i = 0; i < num_weights; i++) {
                ridge_penalty += weights[i] * weights[i];
        }
        return lambda * ridge_penalty;
}
