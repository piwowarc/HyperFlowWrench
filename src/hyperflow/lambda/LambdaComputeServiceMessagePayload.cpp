#include <hyperflow/lambda/LambdaComputeServiceMessagePayload.h>

namespace wrench {
    namespace hyperflow {
        SET_MESSAGEPAYLOAD_NAME(LambdaComputeServiceMessagePayload, DAEMON_STOPPED_MESSAGE_PAYLOAD);
        SET_MESSAGEPAYLOAD_NAME(LambdaComputeServiceMessagePayload, NOTIFY_EXECUTOR_STATUS_MESSAGE_PAYLOAD);
        SET_MESSAGEPAYLOAD_NAME(LambdaComputeServiceMessagePayload, SUBMIT_STANDARD_JOB_REQUEST_MESSAGE_PAYLOAD);
        SET_MESSAGEPAYLOAD_NAME(LambdaComputeServiceMessagePayload, STANDARD_JOB_DONE_MESSAGE_PAYLOAD);
    }
}