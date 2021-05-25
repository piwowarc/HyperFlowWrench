#include <hyperflow/scheduler/HyperFlowJobScheduler.h>

WRENCH_LOG_CATEGORY(hyperflow_scheduler, "Log category for Cloud Scheduler");

namespace wrench {
    namespace hyperflow {
        void HyperFlowJobScheduler::scheduleTasks(const std::set<std::shared_ptr<ComputeService>> &compute_services,
                                                  const std::vector<WorkflowTask *> &tasks) {

            if (compute_services.size() != 1) {
                throw std::runtime_error("HyperFlowScheduler requires a single compute service");
            }

            auto compute_service = *compute_services.begin();
            auto lambda_service = std::dynamic_pointer_cast<LambdaComputeService>(compute_service);

            if (lambda_service == nullptr) {
                throw std::runtime_error("This HyperFlowScheduler can only handle a lambda service");
            }

            WRENCH_INFO("There are %ld ready tasks to schedule", tasks.size());

            for (auto task : tasks) {

                WRENCH_INFO("Submitting task '%s' for execution on a lambda service", task->getID().c_str());

                // Submitting the task
                std::map<WorkflowFile *, std::shared_ptr<FileLocation>> file_locations;
                for (auto f : task->getInputFiles()) {
                    file_locations[f] = (FileLocation::LOCATION(default_storage_service));
                }
                for (auto f : task->getOutputFiles()) {
                    file_locations[f] = (FileLocation::LOCATION(default_storage_service));
                }
                auto job = this->getJobManager()->createStandardJob(task, file_locations);
                this->getJobManager()->submitJob(job, lambda_service);
            }WRENCH_INFO("Done with scheduling tasks as standard jobs");
        }
    }
}

