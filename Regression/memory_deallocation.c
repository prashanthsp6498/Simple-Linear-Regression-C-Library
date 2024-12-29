#include "Regression/Linear.h"
#include <stdio.h>
#include <stdlib.h>

void Free_Model(Beta *model) {
        if (model) {
                free(model);
        } else {
                fprintf(stderr,
                        "There is no memory blocks allocated for Model.\n");
        }
}

void Free_Data(getFile *data) {
        if (data) {
                free(data->X);
                free(data->Y);
                free(data);
        }
}

void Free_Normalize(NormVar *normalize) {
        if (normalize) {
                free(normalize->X);
                free(normalize->Y);
                free(normalize);
        }
}

void Free_Split(SplitData *split) {
        free(split->X_Train);
        free(split->X_Test);
        free(split->Y_Train);
        free(split->Y_Test);
        free(split);
}
