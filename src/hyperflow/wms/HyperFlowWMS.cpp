
#include <hyperflow/wms/HyperFlowWMS.h>
#include <iostream>

#include <hyperflow/lambda/LambdaComputeService.h>

WRENCH_LOG_CATEGORY(hyperflow_wms, "Log category for HyperFlowWMS");
namespace wrench {
    namespace hyperflow {
        HyperFlowWMS::HyperFlowWMS(std::unique_ptr<StandardJobScheduler> standard_job_scheduler,
                                   const std::set<std::shared_ptr<ComputeService>> &compute_services,
                                   const std::set<std::shared_ptr<StorageService>> &storage_services,
                                   const std::string &hostname) : WMS(
                std::move(standard_job_scheduler),
                nullptr,
                compute_services,
                storage_services,
                {}, nullptr,
                hostname,
                "hyperflow") {}

        int HyperFlowWMS::main() {

            TerminalOutput::setThisProcessLoggingColor(TerminalOutput::COLOR_GREEN);

            // Check whether the WMS has a deferred start time
            checkDeferredStart();

            WRENCH_INFO("Starting on host %s", S4U_Simulation::getHostName().c_str());
            WRENCH_INFO("About to execute a workflow with %lu tasks", this->getWorkflow()->getNumberOfTasks());

            // Create a job manager
            this->job_manager = this->createJobManager();

            // Create a data movement manager
            std::shared_ptr<DataMovementManager> data_movement_manager = this->createDataMovementManager();

            // Perform static optimizations
            runStaticOptimizations();

            while (true) {

                // Get the ready tasks
                std::vector<WorkflowTask *> ready_tasks = this->getWorkflow()->getReadyTasks();

                // Get the available compute services
                auto compute_services = this->getAvailableComputeServices<ComputeService>();

                if (compute_services.empty()) {
                    WRENCH_INFO("Aborting - No compute services available!");
                    break;
                }

                // Perform dynamic optimizations
                runDynamicOptimizations();

                // Run ready tasks with defined scheduler implementation
                WRENCH_INFO("Scheduling tasks...");
                this->getStandardJobScheduler()->scheduleTasks(this->getAvailableComputeServices<ComputeService>(), ready_tasks);

                // Wait for a workflow execution event, and process it
                try {
                    this->waitForAndProcessNextEvent();
                } catch (WorkflowExecutionException &e) {
                    WRENCH_INFO("Error while getting next execution event (%s)... ignoring and trying again",
                                (e.getCause()->toString().c_str()));
                    continue;
                }
                if (this->abort || this->getWorkflow()->isDone()) {
                    break;
                }
            }

            S4U_Simulation::sleep(10);

            WRENCH_INFO("--------------------------------------------------------");
            if (this->getWorkflow()->isDone()) {
                WRENCH_INFO("Workflow execution is complete!");
            } else {
                WRENCH_INFO("Workflow execution is incomplete!");
            }

            WRENCH_INFO("HyperFlowWMS Daemon started on host %s terminating", S4U_Simulation::getHostName().c_str());

            this->job_manager.reset();

            return 0;
        }

        void HyperFlowWMS::processEventStandardJobFailure(std::shared_ptr<StandardJobFailedEvent> event) {
            auto job = event->standard_job;
            WRENCH_INFO("Notified that a standard job has failed (all its tasks are back in the ready state)");
            WRENCH_INFO("CauseType: %s", event->failure_cause->toString().c_str());
            WRENCH_INFO("As a HyperFlowWMS, I abort as soon as there is a failure");
            this->abort = true;
        }

        void HyperFlowWMS::processEventStandardJobCompletion(std::shared_ptr<StandardJobCompletedEvent> event) {
            auto job = event->standard_job;
            auto task = job->getTasks().at(0);WRENCH_INFO("Notified that a standard job has completed task %s",
                                                          task->getID().c_str());
        }
    }
}