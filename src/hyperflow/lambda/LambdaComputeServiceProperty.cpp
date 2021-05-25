#include <hyperflow/lambda/LambdaComputeServiceProperty.h>

namespace wrench {
    namespace hyperflow {
        SET_PROPERTY_NAME(LambdaComputeServiceProperty, LAMBDA_STARTUP_OVERHEAD);
        SET_PROPERTY_NAME(LambdaComputeServiceProperty, TASK_STARTUP_OVERHEAD);
        SET_PROPERTY_NAME(LambdaComputeServiceProperty, TASK_SELECTION_ALGORITHM);
        SET_PROPERTY_NAME(LambdaComputeServiceProperty, SIMULATE_COMPUTATION_AS_SLEEP);
        SET_PROPERTY_NAME(LambdaComputeServiceProperty, LAMBDA_RAM);
    }
}