#include "../EDA/DataAnalysis.h"
#include "../Regression/Linear.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

const char FileName[100] = "ADANIPORTS.csv";

int main() {

  // Read file
  getFile *data = Read_Dataset(FileName, 5, 4);
  int datasize = data->num_rows;


  for (int i = 0; i < datasize; i++) {
    printf("Independent_Var: %.2f\t Dependent_Var: %.2f\n", *&data->X[i], *&data->Y[i]);
  }

  size_t size_x, size_y;
  size_x = size_y = datasize;

  Beta *model = Fit_Model(data->X, data->Y, size_x, size_y);
  float lr = 0.01;
  int epochs = 1000;
  printf("%.2f %.2f\n", model->slope, model->intercept);

  Stochastic_Gradient_Descent(data->X, data->Y ,model , size_x, epochs, lr);
  printf("After stochastic gradient %.2f %.2f\n", model->slope, model->intercept);


  float *prediction = Predict_Model(data->X, datasize, *model);

  for (int i = 0; i < datasize; i++) {
    float y_pred  = deNormalize(prediction[i]);
    printf("Actual element: %.2f  Predicted element : %.2f\n", data->Y[i],
           y_pred);
  }


  free(prediction);
  free(data);
  Free_Model(model);

  return 0;
}
