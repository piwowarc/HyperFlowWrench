
#ifndef S_LAMBDACOMPUTESERVICEPROPERTY_H
#define S_LAMBDACOMPUTESERVICEPROPERTY_H


#include "wrench/services/compute/ComputeServiceProperty.h"

namespace wrench {
    namespace hyperflow {
        class LambdaComputeServiceProperty : public ComputeServiceProperty {
        public:
            DECLARE_PROPERTY_NAME(LAMBDA_STARTUP_OVERHEAD);
            DECLARE_PROPERTY_NAME(TASK_STARTUP_OVERHEAD);
            DECLARE_PROPERTY_NAME(TASK_SELECTION_ALGORITHM);
            DECLARE_PROPERTY_NAME(SIMULATE_COMPUTATION_AS_SLEEP);
            DECLARE_PROPERTY_NAME(LAMBDA_RAM);
        };
    }
}
#endif //S_LAMBDACOMPUTESERVICEPROPERTY_H
