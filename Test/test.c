#include "../EDA/DataAnalysis.h"
#include "../Regression/Linear.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Enter File Name
const char *FileName = "ADANIPORTS.csv";

int main() {

  // Read file
  printf("Now Opening the file func\n");
  /**Read_Dataset(const char *file_name, const int independent_var,const int
   * target_var) */
  getFile *data = Read_Dataset(FileName, 5, 8);
  int datasize = data->num_rows;

  float y_min, y_max;
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

  float* cost = (float *)malloc(sizeof(float));

  *cost = Cost_Function(split_data.X_Test, prediction, size_x, size_y);
  float *denormVar = DeNormalize(prediction, y_min, y_max, size_x);

  for (int i = 0; i < datasize; i++) {
    printf("Actual element: %.2f\t Normalized element X: %.2f  Y: %.2f\t "
           "Denormalize "
           "element: %.2f\t Predicted element : %.2f\t Cost_Function: %.2f\n",
           split_data.Y_Test[i], normalize->X[i], normalize->Y[i], denormVar[i],
           prediction[i], cost[i]);
  }

 float rmse = RMSE(split_data.X_Test, prediction , size_x);
  printf("Accuracy: %f\n", rmse);

  // printf("Size of X and Y: %ld %ld", size_x, size_y);

  Free_Model(model);
  free(normalize);
  free(data);
  free(prediction);

  return 0;
}
