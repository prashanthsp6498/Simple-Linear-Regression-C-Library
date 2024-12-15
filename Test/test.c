#include "../EDA/DataAnalysis.h"
#include "../Regression/Linear.h"
#include "../Regression/model_performance_with_regularization.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enter File Name
const char *FileName = "Datasets/winequality-white.csv";

int main() {
        // Read file from the Dataset
        printf(
            "Hi, Welcome to LinRegC Library: A simple and efficient tool for "
            "simple linear regression\n");

        /*FileName, IndependentVar, DependentVar */
        getFile *data = Read_Dataset(FileName, 10, 11);

        int datasize = data->num_rows;

        float y_min, y_max;
        // Pass the data points to Normalization function.
        NormVar *normalize =
            Normalize(data->X, data->Y, datasize, &y_min, &y_max);

        size_t size_x, size_y;
        size_x = size_y = datasize;

        float train_ratio = 0.8;
        int epochs = 1000;
        // need to set a particular value
        float lr = 0.05;
        float lambda1 = 0.2;
        float lambda2 = 0.2;
        // Facing problem  in split_data
        SplitData *split_data =
            Split_Dataset(normalize->X, normalize->Y, size_x, train_ratio);

        Beta *model = Fit_Model(split_data->X_Train, split_data->Y_Train,
                                split_data->train_size, split_data->train_size,
                                epochs, lr, lambda1, lambda2);

        // Learning rate
        printf("Slope: %.2f\t Intercept: %.2f\n", model->slope,
               model->intercept);

        /*
            Stochastic_Gradient_Descent(normalize->X, normalize->Y, model,
           size_x, epochs, lr, lambda1, lambda2);
        */
        printf("\n\t\tAfter stochastic gradient\t\n Slope: %.2f\t Intercept: "
               "%.2f \t\n",
               model->slope, model->intercept);

        float *prediction = Predict_Model(split_data->X_Test, datasize, *model);

        // convert Normalized X_Test to Denormalized X_Test
        // Fixed Bug in Cost_Function
        float *prediction_denorm_var =
            Denormalize(prediction, y_min, y_max, size_x);
        if (!prediction_denorm_var) {
                fprintf(stderr,
                        "Test().denormVar Error: Memory allocation failed\n");
                return EXIT_FAILURE;
        }
        float *Y_test = Denormalize(split_data->Y_Test, y_min, y_max, size_y);
        if (!Y_test) {
                fprintf(stderr,
                        "Test().Y_Test Error: Memory allocation failed\n");
                free(Y_test);
        }

        for (int i = 0; i < datasize; i++) {
                printf("Independent Variable: %.2f\t Dependent Variable: "
                       "%.2f\t Original Value: %.2f\t Predicted Value: %.2f\n",
                       data->X[i], data->Y[i], Y_test[i],
                       prediction_denorm_var[i]);
        }

        // Accuracy Section
        metricResult rmse =
            Root_Mean_Squared_Error(split_data->Y_Test, prediction, size_y);

        metricResult mse =
            Mean_Absolute_Error(split_data->Y_Test, prediction, size_y);

        metricResult mae =
            Mean_Squared_Error(split_data->Y_Test, prediction, size_y);

        if (!rmse.is_valid || !mse.is_valid || !mae.is_valid) {
                fprintf(stderr, "Error in Accuracy score\n");

                return -1;
        }

        if (rmse.accuracy >= 0.8 || mae.accuracy >= 0.8 ||
            mse.accuracy >= 0.8) {

                printf("RMSE: %.2f%% \t MSE: %.2f%%\t MAE: %.2f%%\n",
                       rmse.accuracy, mse.accuracy, mae.accuracy);
        }

        // Free the memory allocations
        Free_Model(model);
        Free_Data(data);
        Free_Normalize(normalize);
        Free_Split(split_data);
        free(prediction);
        free(prediction_denorm_var);
        free(Y_test);

        return 0;
}
