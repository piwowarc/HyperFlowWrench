#ifndef S_LAMBDACOMPUTESERVICE_H
#define S_LAMBDACOMPUTESERVICE_H

#include <deque>
#include <queue>
#include <set>
#include <tuple>
#include <wrench/services/compute/ComputeService.h>
#include <wrench/services/compute/standard_job_executor/StandardJobExecutor.h>
#include <wrench/services/compute/cloud/CloudComputeService.h>
#include "LambdaComputeServiceProperty.h"
#include "LambdaComputeServiceMessagePayload.h"

namespace wrench {
    namespace hyperflow {
        class LambdaComputeService : public ComputeService {


        public:


            LambdaComputeService(const std::string &hostname,
                                 std::vector<std::string> compute_hosts,
                                 std::map<std::string, std::string> property_list = {},
                                 std::map<std::string, double> messagepayload_list = {}
            );


            ~LambdaComputeService() override;


            std::set<std::shared_ptr<StandardJobExecutor>> running_standard_job_executors;
            std::set<std::shared_ptr<StandardJobExecutor>> finished_standard_job_executors;


        private:

            std::map<std::string, std::string> default_property_values = {
                    {LambdaComputeServiceProperty::LAMBDA_STARTUP_OVERHEAD,       "0.0"},
                    {LambdaComputeServiceProperty::TASK_STARTUP_OVERHEAD,         "0"},
                    {LambdaComputeServiceProperty::TASK_SELECTION_ALGORITHM,      "maximum_flops"},
                    {LambdaComputeServiceProperty::SIMULATE_COMPUTATION_AS_SLEEP, "false"},
                    {LambdaComputeServiceProperty::LAMBDA_RAM,                    "1769"},

            };


            std::map<std::string, double> default_messagepayload_values = {
                    {LambdaComputeServiceMessagePayload::STOP_DAEMON_MESSAGE_PAYLOAD,                 1024},
                    {LambdaComputeServiceMessagePayload::SUBMIT_STANDARD_JOB_REQUEST_MESSAGE_PAYLOAD, 1024},
                    {LambdaComputeServiceMessagePayload::SUBMIT_STANDARD_JOB_ANSWER_MESSAGE_PAYLOAD,  1024},
                    {LambdaComputeServiceMessagePayload::STANDARD_JOB_DONE_MESSAGE_PAYLOAD,           1024},
            };

            std::set<std::string> compute_resources;
            std::vector<std::string> compute_hosts;
            CloudComputeService *cloud_service;

            //submits a standard job
            void
            submitStandardJob(std::shared_ptr<StandardJob>,
                              const std::map<std::string, std::string> &job_args) override;

            //submits a standard job
            void
            submitPilotJob(const std::shared_ptr<PilotJob>,
                           const std::map<std::string, std::string> &job_args) override;

            // terminate a standard job
            void terminateStandardJob(std::shared_ptr<StandardJob> job) override;

            // terminate a pilot job
            void terminatePilotJob(std::shared_ptr<PilotJob> job) override;

            int main() override;

            bool processNextMessage();

            void processJobSubmission(std::shared_ptr<StandardJob> job, std::string answer_mailbox);

            void startJob(std::map<std::string, std::tuple<unsigned long, double>>,
                          std::shared_ptr<StandardJob>);

            void processStandardJobCompletion(std::shared_ptr<StandardJobExecutor>, std::shared_ptr<StandardJob>);

            double getComputeCores(double ram_capacity);
        };
    }
}

#endif //S_LAMBDACOMPUTESERVICE_H
