

#ifndef WRENCH_LAMBDACOMPUTESERVICEMESSAGE_H
#define WRENCH_LAMBDACOMPUTESERVICEMESSAGE_H


#include <wrench/services/compute/ComputeServiceMessage.h>


namespace wrench {
    namespace hyperflow {
        class LambdaComputeServiceMessage : public ComputeServiceMessage {
        protected:
            LambdaComputeServiceMessage(std::string name, double payload);
        };

        class LambdaComputeServiceJobRequestMessage : public LambdaComputeServiceMessage {
        public:
            LambdaComputeServiceJobRequestMessage(std::string answer_mailbox, std::shared_ptr<StandardJob> job,
                                                  double payload);

            std::string answer_mailbox;

            std::shared_ptr<StandardJob> job;
        };
    }
}
#endif //WRENCH_LAMBDACOMPUTESERVICEMESSAGE_H
