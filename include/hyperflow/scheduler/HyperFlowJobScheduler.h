#ifndef WRENCH_HYPERFLOWJOBSCHEDULER_H
#define WRENCH_HYPERFLOWJOBSCHEDULER_H


#include <wrench-dev.h>
#include <hyperflow/lambda/LambdaComputeService.h>


namespace wrench {
    namespace hyperflow {

        class HyperFlowJobScheduler : public StandardJobScheduler {


        public:
            explicit HyperFlowJobScheduler(std::shared_ptr<StorageService> default_storage_service) :
                    default_storage_service(default_storage_service) {}


            void scheduleTasks(const std::set<std::shared_ptr<ComputeService>> &compute_services,
                               const std::vector<WorkflowTask *> &tasks);


        private:
            std::vector<std::string> execution_hosts;
            std::shared_ptr<StorageService> default_storage_service;
            std::shared_ptr<LambdaComputeService> lambda_compute_service;
        };
    }
}
#endif //WRENCH_HYPERFLOWJOBSCHEDULER_H
