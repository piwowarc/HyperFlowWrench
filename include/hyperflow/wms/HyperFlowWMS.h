
#ifndef WRENCH_HYPERFLOWWMS_H
#define WRENCH_HYPERFLOWWMS_H

#include <wrench-dev.h>

namespace wrench {
    class Simulation;
    namespace hyperflow {

        class HyperFlowWMS : public WMS {
        public:

            HyperFlowWMS(std::unique_ptr<StandardJobScheduler> standard_job_scheduler,
                         const std::set<std::shared_ptr<ComputeService>> &compute_services,
                         const std::set<std::shared_ptr<StorageService>> &storage_services,
                         const std::string &hostname);

        protected:

            // Overriden method
            void processEventStandardJobCompletion(std::shared_ptr<StandardJobCompletedEvent>) override;

            void processEventStandardJobFailure(std::shared_ptr<StandardJobFailedEvent>) override;

        private:
            int main() override;

            std::shared_ptr<JobManager> job_manager;
            bool abort = false;

        };
    }
}
#endif //WRENCH_HYPERFLOWWMS_H
