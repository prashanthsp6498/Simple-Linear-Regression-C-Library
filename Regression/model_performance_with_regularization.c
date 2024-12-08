
#include "model_performance_with_regularization.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

metricResult Mean_Squared_Error(float *actualValue, float *predictValue,
                                size_t x) {
        metricResult result = {0.0, false};
        float sum = 0.0;
        if (!actualValue || !predictValue) {
                fprintf(stderr, "model_performance_with_regularization().MSE() "
                                "Error Line.14: 'Value is null.\n'");
                result.is_valid = false;
                return result;
        }

        for (size_t i = 0; i < x; i++) {
                float error = actualValue[i] - predictValue[i];
                sum += error * error;
        }

        result.accuracy = (sum / x) * 100;
        result.is_valid = true;

        return result;
}

metricResult Mean_Absolute_Error(float *actualValue, float *predictValue,
                                 size_t x) {
        metricResult result = {0.0, false};

        float sum = 0.0;

        if (!actualValue || !predictValue) {
                fprintf(stderr, "model_performance_with_regularization().MAE() "
                                "Error Line.49: 'Value is null.\n'");
                result.is_valid = false;
                return result;
        }

        for (size_t i = 0; i < x; i++) {
                float error = actualValue[i] - predictValue[i];
                sum += error * error;
        }

        result.accuracy = (sum / x) * 100;
        result.is_valid = true;
        return result;
}

metricResult Root_Mean_Squared_Error(float *actualValue, float *predictValue,
                                     size_t x) {
        metricResult result = {0.0, false};
        float sum = 0.0;

        if (!actualValue || !predictValue) {
                fprintf(stderr,
                        "model_performance_with_regularization().RMSE() "
                        "Error Line.79: 'Value is null.\n'");
                result.is_valid = false;
                return result;
        }
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
        }

        result.accuracy = (sqrt(sum / x)) * 100;
        result.is_valid = true;
        return result;
}

// Regularization Methods: L1 and L2;
float Lasso_Regularization(Beta *model, float lambda) {
        if (!model)
                return 0.0;

        return lambda * (model->slope >= 0 ? 1 : -1);
}

float Ridge_Regularization(Beta *model, float lambda) {
        if (!model)
                return 0.0;

        return lambda * (model->slope * model->slope);
}

float ElasticNet_Regularization(Beta *model, float lambda1, float lambda2) {

        float l1_penalty =
            lambda1 * (fabs(model->slope) + fabs(model->intercept));
        float l2_penalty =
            lambda2 * (pow(model->slope, 2) + pow(model->intercept, 2));

        return l1_penalty + l2_penalty;
}
