
set(SOURCE_FILES
        LambdaBagOfTasks.cpp
        )

add_executable(wrench-example-lambda-bag-of-tasks ${SOURCE_FILES})

if (ENABLE_BATSCHED)
    find_library(ZMQ_LIBRARY NAMES zmq)
    target_link_libraries(wrench-example-lambda-bag-of-tasks hyperflow ${WRENCH_PEGASUS_LIBRARY} ${WRENCH_LIBRARY} ${SimGrid_LIBRARY} ${PUGIXML_LIBRARY} ${ZMQ_LIBRARY})
else ()
    target_link_libraries(wrench-example-lambda-bag-of-tasks hyperflow ${WRENCH_PEGASUS_LIBRARY} ${WRENCH_LIBRARY}  ${SimGrid_LIBRARY} ${PUGIXML_LIBRARY})
endif ()

install(TARGETS wrench-example-lambda-bag-of-tasks DESTINATION bin)

