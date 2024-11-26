#include "Linear.h"
#include "EDA/DataAnalysis.h"
#include <complex.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Beta *Initialize_Model() {
  Beta *model = (Beta *)malloc(sizeof(Beta));
  if (!model) {
    fprintf(stderr, " Memory allocation failed. Please check the "
                    "'Initialize_Model()' func\n");
    return NULL;
  }

  model->slope = ((float)rand() / RAND_MAX) * 0.01;
  model->intercept = ((float)rand() / RAND_MAX) * 0.01;

  return model;
}

Beta *Fit_Model(float X[], float y[], size_t n, size_t m) {

  Beta *model = Initialize_Model();
  float sumXY = 0.0;
  float sumX = 0.0;
  float sumY = 0.0;
  float sumX2 = 0.0;

  if (n != m || n == 0 || m == 0) {
    fprintf(
        stderr,
        "Size of Independent and Dependent variable are not same. check the "
        "Dataset or length of the variables\n");
    Free_Model(model);
    return NULL;
  }

  printf("Size of both Dependent and Independent variable are equal\n");
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
  // calculate slope and intercept
  model->slope = (n * sumXY - sumX * sumY) / denominator;

  model->intercept = (sumY - model->slope * sumX) / n;

  printf("SumX: %f\t  sumXY: %f\t   sumY: %f\t    sumX2: %f slope: %.2f "
         "intercept: "
         "%.2f\n",
         sumX, sumXY, sumY, sumX2, model->slope, model->intercept);

  return model;
}

// Predict the Output
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
                                 int epochs, float lr) {
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
      float prediction = model->slope * X[i] + model->intercept;
      float error = Y[i] - prediction;

      if (isnan(error) || isnan(model->slope) || isnan(model->intercept)) {
        printf("Nan detected %.3f %.3f\n", model->slope, model->intercept);
        return;
      }

      model->slope += lr * error * X[i];
      model->intercept += lr * error;
      /*
      printf("Epoch: %d Data point: %zu, Error: %.4f, Updated Slope: %.4f, "
             "Updated Intercept: %.4f\n",
             epoch, i, error, model->slope, model->intercept);
      */
    }
  }
}

float Cost_Function(float actualValue[], float *predictedValue, size_t sizeX,
                    size_t sizeY) {
  float costFunction = 0.0;

  if (sizeX != sizeY) {
    fprintf(stderr,
            "Length of Dependent and Independent variable are not same ");
  }

  for (size_t i = 0; i < sizeX; i++) {
    if (actualValue[i] != 0 && predictedValue[i] != 0) {
      float error = predictedValue[i] - actualValue[i];
      costFunction += error * error;
    }
  }

  costFunction /= (2 * sizeX);

  return costFunction;
}

metricResult RMSE(float *ActualData, float *PredictedData, size_t n) {
  // Formula: sqrt((PredictedData - ActualData)^2)/n
  metricResult result = {0.0, false};

  if (n == 0) {
    fprintf(stderr, "Error: Number of data points must be greater than zero\n");
    return result;
  }

  float error_sum = 0.0;
  size_t valid_count = 0;
  for (size_t i = 0; i < n; i++) {
    if (isnan(PredictedData[i]) || isnan(ActualData[i]) ||
        isinf(ActualData[i]) || isinf(PredictedData[i])) {
      fprintf(stderr, "Skipping Invalid data point\n");
      continue;
    }
    float error = PredictedData[i] - ActualData[i];
    error_sum += error * error;
    valid_count = valid_count+1;
  }

  if (valid_count == 0) {
    fprintf(stderr, "Error: No valid data points for RMSE Calculations\n");
    result.is_valid = false;
    result.value = NAN;
    return result;
  }

  result.value = sqrt(error_sum / valid_count);
  result.is_valid = true;

  return result;
}

metricResult MSE(float *ActualData, float *PredictedData, size_t n) {
  metricResult result = {0.0, false};

  float error_sum = 0.0;
  size_t validCount = 0;
  for (size_t i = 0; i < n; i++) {
    if (isnan(ActualData[i]) || isnan(PredictedData[i]) ||
        isinf(PredictedData[i]) || isinf(ActualData[i])) {
      fprintf(stderr, "Invalid Data at index: %zu\n", i);
      continue;
    }

    float error = ActualData[i] - PredictedData[i];
    error_sum += error * error;
    validCount++;
  }

  if (validCount == 0) {
    fprintf(stderr, "Error: No valid Data Points\n");
    result.is_valid = false;
    result.value = NAN;
    return result;
  }

  result.value = error_sum / validCount;
  result.is_valid = true;

  return result;
}

void Free_Model(Beta *model) {
  if (model) {
    free(model);
  }
}

