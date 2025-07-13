/* Linear.c */
#include "Linear.h"
#include "memory_deallocation.h"
#include "model_performance_with_regularization.h"
#include <complex.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* returns the model with memory allocation pointing to 0 */
Beta *Initialize_Model() {
        Beta *model = (Beta *)calloc(1, sizeof(Beta));
        if (!model) {
                fprintf(stderr, " Memory allocation failed. Please check the "
                                "'Initialize_Model()' func\n");
                return NULL;
        }

        model->slope = (float)(rand() / RAND_MAX) * 0.01;
        model->intercept = (float)(rand() / RAND_MAX) * 0.01;

        return model;
}

/* returns the slope and intercept */
Beta *Fit_Model(float X[], float y[], size_t n, size_t m, int epochs, float lr,
                float lambda1, float lambda2) {

        Beta *model = Initialize_Model();
        if (X == NULL || y == NULL) {
                fprintf(
                    stderr,
                    "Linear().Fit_Model Error: Input arrays cannot be NULL.\n");
                Free_Model(model);
                return NULL;
        }
        float sumXY = 0.0;
        float sumX = 0.0;
        float sumY = 0.0;
        float sumX2 = 0.0;

        if (n != m || n == 0 || m == 0) {
                fprintf(stderr, "Size of Independent and Dependent variable "
                                "are not same. check the "
                                "Dataset or length of the variables\n");
                Free_Model(model);
                return NULL;
        }

        printf(
            "\t ---------------------------------------------------------------"
            "----------------------------------------------------------------"
            "----------------------------------------\n");
        printf("\t| \t\t\t\t\t\t\tSize of both Dependent and Independent "
               "variable are equal\t\t\t\t\t\t\t|\n");
        printf(
            "\t|---------------------------------------------------------------"
            "---------------------------------------------------------------"
            "-----------------------------------------|\n");
        for (size_t i = 0; i < n; i++) {
                sumXY += X[i] * y[i];
                sumX += X[i];
                sumY += y[i];
                sumX2 += X[i] * X[i];
        }

        float denominator = n * sumX2 - sumX * sumX;
        if (fabs(denominator) < 1e-6) {
                fprintf(stderr, "Denominator is zero");
                Free_Model(model);
                return NULL;
        }

        Stochastic_Gradient_Descent(X, y, model, n, epochs, lr, lambda1,
                                    lambda2);
        model->slope = (n * sumXY - sumX * sumY) / denominator;

        model->intercept = (sumY - model->slope * sumX) / n;
        printf("\t|\tSumX: %f |  \tsumXY: %f\t|   \tsumY: %f\t|   \tsumX2: "
               "%f\t| \tslope: %.2f\t| "
               "\tintercept: "
               "%.2f |\n",
               sumX, sumXY, sumY, sumX2, model->slope, model->intercept);
        printf(
            "\t ---------------------------------------------------------------"
            "--------------------------------------------------------------"
            "------------------------------------------\n");

        return model;
}

/* returns the y = mx + c */
float *Predict_Model(float X[], size_t size, Beta model) {

        float *prediction = (float *)malloc(size * sizeof(float));

        if (prediction == NULL) {
                fprintf(stderr, "Memory allocation error");
                return NULL;
        }

        for (size_t i = 0; i < size; i++) {
                prediction[i] = model.slope * X[i] + model.intercept;
        }

        return prediction;
}

void Stochastic_Gradient_Descent(float X[], float Y[], Beta *model, size_t n,
                                 int epochs, float lr, float lambda1,
                                 float lambda2) {
        // suppose assume epochs is 10,000 then iterates over 10000 times...
        for (int epoch = 0; epoch < epochs; epoch++) {
                for (size_t i = 0; i < n; i++) {
                        size_t rand_index = rand() % n;
                        float tempX = X[i];
                        float tempY = Y[i];
                        X[i] = X[rand_index];
                        Y[i] = Y[rand_index];
                        X[rand_index] = tempX;
                        Y[rand_index] = tempY;
                }

                for (size_t i = 0; i < n; i++) {
                        float prediction =
                            model->slope * X[i] + model->intercept;
                        float error = Y[i] - prediction;

                        float slope_gradient = -error * X[i];
                        float intercept_gradient = -error;

                        float l3_penalty =
                            ElasticNet_Regularization(model, lambda1, lambda2);
                        /*

                        model->slope += lr * (error * X[i] -
                        lasso_penality); model->intercept += lr * (error -
                        lasso_penality);
                        */

                        model->slope -= lr * (slope_gradient + l3_penalty);
                        model->intercept -=
                            lr * (intercept_gradient + l3_penalty);

                        if (!model->slope || !model->intercept) {
                                fprintf(stderr,
                                        "Model is not calculated correctly: "
                                        "error %f\t %f\n",
                                        model->slope, model->intercept);
                                return;
                        }
                }
        }
}
