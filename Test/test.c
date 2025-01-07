/* Test.c */
#include "../EDA/DataAnalysis.h"
#include "../Regression/Linear.h"
#include "../Regression/memory_deallocation.h"
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
            "Hi, Welcome to LinRegC Library : A simple and efficient tool for "
            "simple linear regression in C\n");

        /*FileName, IndependentVar, DependentVar */
        getFile *data = Read_Dataset(FileName, 10, 11);

        /* returns the size of the dataset */
        int datasize = data->num_rows;

        // Pass the data points to Normalization function.
        NormVar *normalize = Normalize(data->X, data->Y, datasize);

        size_t size_x, size_y;
        size_x = size_y = datasize;

        float train_ratio = 0.8;
        int epochs = 2000;
        // need to set a particular value
        float lr = 0.01;
        float lambda1 = 0.5;
        float lambda2 = 0.5;

        SplitData *split_data =
            Split_Dataset(normalize->X, normalize->Y, size_x, train_ratio);

        Beta *model = Fit_Model(split_data->X_Train, split_data->Y_Train,
                                split_data->train_size, split_data->train_size,
                                epochs, lr, lambda1, lambda2);

        /*
            Stochastic_Gradient_Descent(normalize->X, normalize->Y, model,
           size_x, epochs, lr, lambda1, lambda2);
        */

        float *prediction =
            Predict_Model(split_data->X_Test, split_data->test_size, *model);

        // convert Normalized X_Test to Denormalized X_Test
        float *denormalize_y_test =
            Denormalize(split_data->Y_Test, normalize->y_min, normalize->y_max,
                        split_data->test_size);
        if (!denormalize_y_test) {
                fprintf(stderr,
                        "Test().Y_Test Error: Memory allocation failed\n");
                return EXIT_FAILURE;
        }
        // Facing issue: the normalize function is not implementing to
        // split_data->y_test and split_data->x_test
        float *independent =
            Denormalize(split_data->X_Test, normalize->x_min, normalize->x_max,
                        split_data->test_size);
        float *Denormalize_prediction =
            Denormalize(prediction, normalize->y_min, normalize->y_max,
                        split_data->test_size);
        printf("\n");
        printf("\t "
               "+++++++++++++++++++++++++++++++++++++++++++++++++++++++"
               "+++++++++++++++++| \tOutPut\t"
               "\t|++++++++++++++++++++++++++++++++++++++++++++++++++++"
               "++++++++++++++++++++\n");

        printf(
            "\t ---------------------------------------------------------------"
            "---------------------------------------------------------------"
            "--------------------------------------------------\n");
        printf("\t|var_x \t| var_y \t| Normalize_var_x \t|  Denormalize_var_x "
               "\t| Normalize_var_y \t| Denormalize_var_y \t| "
               "Normalize_Prediction: \t| Denormalize_Prediction  |\n");
        printf("\t| "
               "---------------------------------------------------------------"
               "---------------------------------------------------------------"
               "--------------------------------------------------|\n");
        for (int i = 0; i < (int)split_data->test_size; i++) {
                printf("\t| %.2f\t| var_y: %.2f\t| "
                       "var_x(Norm): %.2f \t| var_x(Denorm): %.2f\t|"
                       "var_y(Norm): %.2f\t| var_y(Denorm): "
                       "%.2f\t| Prediction(Norm): %.2f \t| Prediction(Denorm): "
                       "%.2f|\n",
                       data->X[i], data->Y[i], split_data->X_Test[i],
                       independent[i], split_data->Y_Test[i],
                       denormalize_y_test[i], prediction[i],
                       Denormalize_prediction[i]);
        }
        printf(
            "\t ---------------------------------------------------------------"
            "------------------------------------------------------------------"
            "-"
            "-----------------------------------------------\n");

        // Model Performance: i.e MSE RMSE and MAE
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
        printf("\t \tSlope: %.2f\t Intercept: %.2f\n", model->slope,
               model->intercept);

        printf(
            "\t ---------------------------------------------------------------"
            "---------------------------------------------------------------"
            "------------------------------------------\n");
        printf(
            "\t| \t\t\t\t\t\t\t\t\tAccuracy Metricis\t\t\t\t\t\t\t\t\t\t |\n");
        printf("\n");
        printf("\t|\t\t\t\t\t\t\tRMSE: %.2f%%\t MSE: %.2f%%\t MAE: %.2f%% "
               "\t\t\t\t\t\t\t\t\t |\n",
               rmse.accuracy, mse.accuracy, mae.accuracy);
        printf(
            "\t ---------------------------------------------------------------"
            "---------------------------------------------------------------"
            "------------------------------------------\n");

        // Free the memory allocations
        // Use Valgrind to trace the memory allocations
        // Free in reverse order of allocation
        free(denormalize_y_test);
        free(Denormalize_prediction);
        free(prediction);
        Free_Model(model);
        Free_Split(split_data);
        Free_Normalize(normalize);
        Free_Data(data);
        return 0;
}
