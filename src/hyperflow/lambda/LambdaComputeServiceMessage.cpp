
#include <hyperflow/lambda/LambdaComputeServiceMessage.h>

namespace wrench {
    namespace hyperflow {
        LambdaComputeServiceMessage::LambdaComputeServiceMessage(std::string name, double payload) :
                ComputeServiceMessage("LambdaComputeServiceMessage::" + name, payload) {
        }


        LambdaComputeServiceJobRequestMessage::LambdaComputeServiceJobRequestMessage(std::string answer_mailbox,
                                                                                     std::shared_ptr<StandardJob> job,
                                                                                     double payload)
                : LambdaComputeServiceMessage("LambdaComputeServiceJobRequestMessage", payload) {
            if (job == nullptr) {
                throw std::invalid_argument(
                        "LambdaComputeServiceJobRequestMessage::LambdaComputeServiceJobRequestMessage(): Invalid arguments");
            }
            if (answer_mailbox.empty()) {
                throw std::invalid_argument(
                        "LambdaComputeServiceJobRequestMessage::LambdaComputeServiceJobRequestMessage(): Empty answer mailbox");
            }
            this->job = job;
            this->answer_mailbox = answer_mailbox;

        }
    }
}

