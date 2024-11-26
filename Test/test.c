#include "../EDA/DataAnalysis.h"
#include "../Regression/Linear.h"
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enter File Name
const char *FileName = "ADANIPORTS.csv";

// This is to Demonstrate how to use the Library.
int main() {
  // Read file
  printf("Now Opening the file func\n");

  // Read_Dataset(const char *file_name, const int independent_var,const int*
  // target_var) */
  getFile *data = Read_Dataset(FileName, 5, 8);
  int datasize = data->num_rows;

  float y_min, y_max;
  float x_min, x_max;
  x_min = FLT_MAX;
  x_max = -FLT_MAX;
  NormVar *normalize = Normalize(data->X, data->Y, datasize, &y_min, &y_max);

  size_t size_x, size_y;
  size_x = size_y = datasize;

  float train_ratio = 0.8;
  SplitData split_data =
      Split_Dataset(normalize->X, normalize->Y, size_x, train_ratio);

  Beta *model =
      Fit_Model(split_data.X_Train, split_data.Y_Train, size_x, size_y);
  float lr = 0.05;
  int epochs = 10000;
  printf("%.2f %.2f\n", model->slope, model->intercept);

  Stochastic_Gradient_Descent(normalize->X, normalize->Y, model, size_x, epochs,
                              lr);
  printf("After stochastic gradient %.2f %.2f \t\n", model->slope,
         model->intercept);

  float *prediction = Predict_Model(split_data.X_Test, datasize, *model);

  float *cost = (float *)malloc(sizeof(float));

  // convert Normalized X_Test to Denormalized X_Test
  float *x_Test = DeNormalize(split_data.X_Test, x_min, x_max, size_x);
  *cost = Cost_Function(x_Test, prediction, size_x, size_y);
  float *denormVar = DeNormalize(prediction, y_min, y_max, size_x);

  for (int i = 0; i < datasize; i++) {
    printf("Actual element: %.2f\t Normalized element X: %.2f  Y: %.2f\t "
           "Denormalize "
           "element: %.2f\t Predicted element : %.2f\t Cost_Function: %.2f\n",
           split_data.Y_Test[i], normalize->X[i], normalize->Y[i], denormVar[i],
           prediction[i], cost[i]);
  }

  // float *deNormalize = DeNormalize(split_data.X_Test, x_min, x_max, size_x);

  metricResult rmse = RMSE(split_data.Y_Test, prediction, size_x);
  if (rmse.is_valid) {
    printf("RMSE: Is_Valid(0: No 1: Yes) ? %d Value: %f", rmse.is_valid,
           rmse.value);
  } else {
    fprintf(stderr, "Failed to calculate RMSE\n");
    return 0;
  }

  metricResult mse = MSE(split_data.Y_Test, prediction, size_x);
  if (mse.value >= 0) {

    printf("\tMSE: Is_Valid(0: No 1: Yes) ? %d\t Value: %f\n", mse.is_valid,
           mse.value);
  } else {
    printf("Failed to calculate MSE\n");
    return 0;
  }

  if (model) {
    Free_Model(model);
    model = NULL;
  }
  if (data) {
    free(data);
    data = NULL;
  }
  if (normalize) {
    free(normalize);
    normalize = NULL;
  }
  if (prediction) {
    free(prediction);
    prediction = NULL;
  }

  return 0;
}
