#ifndef memory_deallocation
#define memory_deallocation()
#include "../EDA/DataAnalysis.h"
#include "Regression/Linear.h"

void Free_Model(Beta *);
void Free_Data(getFile *);
void Free_Normalize(NormVar *);
void Free_Split(SplitData *);

#endif
