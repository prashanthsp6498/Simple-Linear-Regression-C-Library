#include "../EDA/DataAnalysis.h"
#include "Regression/Linear.h"
#include <stdlib.h>

void Free_Model(Beta *model) {
        free(model);
        model = NULL;
}

void Free_Data(getFile *data) {
        if (data) {
                free(data->X);
                free(data->Y);
                free(data);
                data = NULL;
        }
}

void Free_Normalize(NormVar *normalize) {
        if (normalize) {
                free(normalize->X);
                free(normalize->Y);
                free(normalize);
                normalize = NULL;
        }
}

void Free_Split(SplitData *split) {
        free(split->X_Train);
        free(split->X_Test);
        free(split->Y_Train);
        free(split->Y_Test);
        free(split);
        split = NULL;
}
