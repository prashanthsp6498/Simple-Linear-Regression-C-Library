#include "../EDA/DataAnalysis.h"
#include "../Regression/Linear.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enter File Name
const char *FileName = "Datasets/winequality-white.csv";

// This is to Demonstrate how to use the Library.
int main() {
  // Read file from the Dataset
  printf("Now Opening the file func\n");

  // Read_Dataset(const char *file_name, const int independent_var,const int*
  // target_var) */
  getFile *data = Read_Dataset(FileName, 10, 11);

  if (sizeof(data->X) != sizeof(data->Y)) {
    fprintf(stderr, "Test() Error: Size of Independent and Dependent variable "
                    "is not equal\n");
    // If failed to allocate memory, then free the *data
    free(data->X);
    free(data->Y);
    return 0;
  }
  int datasize = data->num_rows;

  float y_min, y_max;
  // Pass the data points to Normalization function.
  NormVar *normalize = Normalize(data->X, data->Y, datasize, &y_min, &y_max);

  size_t size_x, size_y;
  size_x = size_y = datasize;

  float train_ratio = 0.8;
  SplitData split_data =
      Split_Dataset(normalize->X, normalize->Y, size_x, train_ratio);

  Beta *model =
      Fit_Model(split_data.X_Train, split_data.Y_Train, size_x, size_y);
  float lr = 0.5;
  int epochs = 10000;
  printf("%.2f %.2f\n", model->slope, model->intercept);

  Stochastic_Gradient_Descent(normalize->X, normalize->Y, model, size_x, epochs,
                              lr);
  printf(
      "\n\t\tAfter stochastic gradient\t\n Slope: %.2f\t Intercept: %.2f \t\n",
      model->slope, model->intercept);

  float *prediction = Predict_Model(split_data.X_Test, datasize, *model);

  float *cost = (float *)malloc(sizeof(float));

  // convert Normalized X_Test to Denormalized X_Test
  // float *x_Test = DeNormalize(split_data.X_Test, x_min, x_max, size_x);
  *cost = Cost_Function(split_data.X_Test, prediction, size_x, size_y);
  float *denormVar = Denormalize(prediction, y_min, y_max, size_x);

  for (int i = 0; i < datasize; i++) {
    printf("Independent Var: %.2f\t Dependent var: %.2f\t | Actual element: "
           "%.2f |\t "
           "Normalized element X: %.2f  Y: %.2f Denormalize element: %f\t "
           "| Predicted element : %.2f |\t Cost_Function: %.2f\n",
           data->X[i], data->Y[i], split_data.Y_Test[i], normalize->X[i],
           normalize->Y[i], denormVar[i], prediction[i], cost[i]);
  }

  // float *deNormalize = DeNormalize(split_data.X_Test, x_min, x_max, size_x);

  metricResult rmse = RMSE(split_data.Y_Test, prediction, size_x);
  if (rmse.is_valid) {
    printf("RMSE: Is_Valid(0: No 1: Yes) ? %d Accuracy: %f", rmse.is_valid,
           rmse.Accuracy);
  } else {
    fprintf(stderr, "Failed to calculate RMSE\n");
    return 0;
  }

  metricResult mse = MSE(split_data.Y_Test, prediction, size_x);
  if (mse.Accuracy >= 0) {

    printf("\tMSE: Is_Valid(0: No 1: Yes) ? %d\t Accuracy: %f\n", mse.is_valid,
           mse.Accuracy);
  } else {
    printf("Failed to calculate MSE\n");
    return 0;
  }

  Free_Model(model);
  free(prediction);
  prediction = NULL;
  free(data);
  data = NULL;
  free(normalize);
  normalize = NULL;

  return 0;
}
