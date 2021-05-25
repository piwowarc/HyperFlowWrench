
#include <hyperflow/lambda/LambdaComputeService.h>


#include <nlohmann/json.hpp>

#include <wrench/services/compute/cloud/CloudComputeService.h>
#include <wrench/wms/WMS.h>
#include "wrench/exceptions/WorkflowExecutionException.h"
#include "wrench/logging/TerminalOutput.h"
#include "wrench/services/compute/batch/BatchComputeService.h"
#include "wrench/services/compute/batch/BatchComputeServiceMessage.h"
#include "wrench/services/compute/bare_metal/BareMetalComputeService.h"
#include "wrench/simgrid_S4U_util/S4U_Mailbox.h"
#include "wrench/simgrid_S4U_util/S4U_Simulation.h"
#include "wrench/simulation/Simulation.h"
#include "wrench/util/PointerUtil.h"
#include "wrench/util/TraceFileLoader.h"
#include "wrench/workflow/job/PilotJob.h"
#include <cfloat>
#include <memory>
#include <numeric>
#include <wrench/workflow/failure_causes/JobTypeNotSupported.h>

#include "wrench/exceptions/WorkflowExecutionException.h"
#include "wrench/logging/TerminalOutput.h"
#include "wrench/services/helpers/ServiceTerminationDetector.h"
#include "wrench/services/helpers/ServiceTerminationDetectorMessage.h"
#include "wrench/services/compute/cloud/CloudComputeService.h"
#include "wrench/services/compute/bare_metal/BareMetalComputeService.h"
#include "wrench/simgrid_S4U_util/S4U_Mailbox.h"
#include <hyperflow/lambda/LambdaComputeServiceMessage.h>
#include <hyperflow/StandardJobExecutorMessage.h>


WRENCH_LOG_CATEGORY(lambda_compute_servivce, "Log category for Lambda Compute Service");

namespace wrench {
    namespace hyperflow {
        LambdaComputeService::~LambdaComputeService() {}


        LambdaComputeService::LambdaComputeService(const std::string &hostname,
                                                   std::vector<std::string> compute_hosts,
                                                   std::map<std::string, std::string> property_list,
                                                   std::map<std::string, double> messagepayload_list) :
                ComputeService(hostname,
                               "lambda",
                               "lambda",
                               "") {
            WRENCH_INFO("Initializing LambdaComputeService");
            std::cerr << "Initializing LambdaComputeService" << std::endl;
            this->compute_hosts = compute_hosts;

            // Set default and specified properties
            this->setProperties(this->default_property_values, std::move(property_list));
            // Set default and specified message payloads
            this->setMessagePayloads(this->default_messagepayload_values, std::move(messagepayload_list));
        }


        void LambdaComputeService::submitStandardJob(std::shared_ptr<StandardJob> job,
                                                     const std::map<std::string, std::string> &job_args) {
            std::cerr << "LambdaComputeService::submitStandardJob()" << std::endl;
            std::string answer_mailbox = S4U_Mailbox::generateUniqueMailboxName("lambda_standard_job_mailbox");
            try {
                S4U_Mailbox::dputMessage(this->mailbox_name,
                                         new LambdaComputeServiceJobRequestMessage(
                                                 answer_mailbox, job,
                                                 this->getMessagePayloadValue(
                                                         LambdaComputeServiceMessagePayload::SUBMIT_STANDARD_JOB_REQUEST_MESSAGE_PAYLOAD)));
            } catch (std::shared_ptr<NetworkError> &cause) {
                throw WorkflowExecutionException(cause);
            }

            // Get the answer
            std::unique_ptr<SimulationMessage> message = nullptr;
            try {
                message = S4U_Mailbox::getMessage(answer_mailbox, this->network_timeout);
            } catch (std::shared_ptr<NetworkError> &cause) {
                throw WorkflowExecutionException(cause);
            }


            if (auto msg = dynamic_cast<ComputeServiceSubmitStandardJobAnswerMessage *>(message.get())) {
                std::cerr << "Got job answer message" << msg->getName() << std::endl;
                // If no success, throw an exception
                if (not msg->success) {
                    throw WorkflowExecutionException(msg->failure_cause);
                }
                return;
            }

            throw std::runtime_error(
                    "LambdaComputeService::submitStandardJob(): Received an unexpected [" + message->getName() +
                    "] message!");
        }

        void LambdaComputeService::submitPilotJob(const std::shared_ptr<PilotJob>,
                                                  const std::map<std::string, std::string> &job_args) {
            throw std::runtime_error("LambdaComputeService::submitPilotJob(): not implemented");
        }

        void LambdaComputeService::terminateStandardJob(std::shared_ptr<StandardJob> job) {
            throw std::runtime_error("LambdaComputeService::terminateStandardJob(): not implemented");
        }

        void LambdaComputeService::terminatePilotJob(std::shared_ptr<PilotJob> job) {
            throw std::runtime_error("LambdaComputeService::terminatePilotJob(): not implemented");
        }

        int LambdaComputeService::main() {
            TerminalOutput::setThisProcessLoggingColor(TerminalOutput::COLOR_YELLOW);
            this->state = Service::UP;

            WRENCH_INFO("New LambdaComputeService (%s) starting on %ld hosts",
                        this->mailbox_name.c_str(), this->compute_resources.size());

            std::cerr << "New LambdaComputeService " << std::endl;
            while (this->processNextMessage()) {
            }

            WRENCH_INFO("LambdaComputeService on host %s terminating cleanly!", S4U_Simulation::getHostName().c_str());
            return 0;
        }

        bool LambdaComputeService::processNextMessage() {
            TerminalOutput::setThisProcessLoggingColor(TerminalOutput::COLOR_YELLOW);
            S4U_Simulation::computeZeroFlop();

            // Wait for a message
            std::shared_ptr<SimulationMessage> message;
            try {
                message = S4U_Mailbox::getMessage(this->mailbox_name);
            } catch (std::shared_ptr<NetworkError> &error) { WRENCH_INFO(
                        "Got a network error while getting some message... ignoring");
                return true;
            }

            WRENCH_INFO("Got a [%s] message", message->getName().c_str());
            std::cerr << "Got a " << message->getName().c_str() << " message" << std::endl;

            if (auto msg = std::dynamic_pointer_cast<ServiceStopDaemonMessage>(message)) {
                // TODO: Forward the stop request to the executor
                // This is Synchronous
                try {
                    S4U_Mailbox::putMessage(msg->ack_mailbox,
                                            new ServiceDaemonStoppedMessage(this->getMessagePayloadValue(
                                                    LambdaComputeServiceMessagePayload::DAEMON_STOPPED_MESSAGE_PAYLOAD)));
                } catch (std::shared_ptr<NetworkError> &cause) {
                    return false;
                }
                return false;
            } else if (auto msg = dynamic_cast<LambdaComputeServiceJobRequestMessage *>(message.get())) {
                processJobSubmission(msg->job, msg->answer_mailbox);
                return true;
        } else if (auto msg = dynamic_cast<StandardJobExecutorDoneMessage *>(message.get())) {
            processStandardJobCompletion(msg->executor, msg->job);
            return true;
            } else {
                throw std::runtime_error(
                        "LambdaComputeService::processNextMessage(): Received an unexpected [" + message->getName() +
                        "] message!");
            }
        }

        void LambdaComputeService::processJobSubmission(std::shared_ptr<StandardJob> job, std::string answer_mailbox) {
            std::cerr << "Processing job" << job->getName() << std::endl;

            Simulation::sleep(
                    this->getPropertyValueAsDouble(LambdaComputeServiceProperty::LAMBDA_STARTUP_OVERHEAD));

            S4U_Mailbox::dputMessage(answer_mailbox,
                                     new ComputeServiceSubmitStandardJobAnswerMessage(
                                             job,
                                             this->getSharedPtr<LambdaComputeService>(),
                                             true,
                                             nullptr,
                                             this->getMessagePayloadValue(
                                                     LambdaComputeServiceMessagePayload::SUBMIT_STANDARD_JOB_ANSWER_MESSAGE_PAYLOAD)));

            std::map<std::string, std::tuple<unsigned long, double>> resources = {};
            auto ram_capacity = this->getPropertyValueAsDouble(LambdaComputeServiceProperty::LAMBDA_RAM);
            auto cpu_cores = getComputeCores(ram_capacity);
            resources.insert(
                    std::make_pair(this->compute_hosts[0], std::make_tuple(cpu_cores, ram_capacity)));WRENCH_INFO(
                    "Got a [%f] message", cpu_cores);

            this->startJob(resources, job);
        }

        double LambdaComputeService::getComputeCores(double ram_capacity) {
            return ram_capacity / 1769;
        }

        void LambdaComputeService::startJob(std::map<std::string, std::tuple<unsigned long, double>> resources,
                                            std::shared_ptr<StandardJob> sjob) {


            // Create a standard job executor
            std::shared_ptr<StandardJobExecutor> executor = std::shared_ptr<StandardJobExecutor>(
                    new StandardJobExecutor(
                            this->simulation,
                            this->mailbox_name,
                            std::get<0>(*resources.begin()),
                            sjob,
                            resources,
                            this->getScratch(),
                            false,
                            nullptr,
                            {{StandardJobExecutorProperty::TASK_STARTUP_OVERHEAD,
                                     this->getPropertyValueAsString(
                                             LambdaComputeServiceProperty::TASK_STARTUP_OVERHEAD)},
                             {StandardJobExecutorProperty::SIMULATE_COMPUTATION_AS_SLEEP,
                                     this->getPropertyValueAsString(
                                             LambdaComputeServiceProperty::SIMULATE_COMPUTATION_AS_SLEEP)},
                             {StandardJobExecutorProperty::TASK_SELECTION_ALGORITHM,
                                     this->getPropertyValueAsString(
                                             LambdaComputeServiceProperty::TASK_SELECTION_ALGORITHM)}
                            },
                            {}));
            executor->start(executor, true, false); // Daemonized, no auto-restart
            this->running_standard_job_executors.insert(executor);

            return;

        }

        void LambdaComputeService::processStandardJobCompletion(std::shared_ptr<StandardJobExecutor> executor,
                                                                std::shared_ptr<StandardJob> job) {
            bool executor_on_the_list = false;
            std::set<std::shared_ptr<StandardJobExecutor>>::iterator it;

            for (it = this->running_standard_job_executors.begin();
                 it != this->running_standard_job_executors.end(); it++) {
                if (*it == executor) {
                    PointerUtil::moveSharedPtrFromSetToSet(it, &(this->running_standard_job_executors),
                                                           &(this->finished_standard_job_executors));
                    executor_on_the_list = true;
                    break;
                }
            }
            this->finished_standard_job_executors.clear();


            if (not executor_on_the_list) { WRENCH_WARN(
                        "LambdaComputeService::processStandardJobCompletion(): Received a standard job completion, but the executor is not in the executor list - Likely getting wires crossed due to concurrent completion and time-outs.. ignoring")
                return;
//            throw std::runtime_error(
//                    "BatchComputeService::processStandardJobCompletion(): Received a standard job completion, but the executor is not in the executor list");
            }




            // Send the callback to the originator
            S4U_Mailbox::dputMessage(job->popCallbackMailbox(),
                                     new ComputeServiceStandardJobDoneMessage(
                                             job, this->getSharedPtr<LambdaComputeService>(),
                                             this->getMessagePayloadValue(
                                                     LambdaComputeServiceMessagePayload::STANDARD_JOB_DONE_MESSAGE_PAYLOAD)));


            return;
        }
    }
}