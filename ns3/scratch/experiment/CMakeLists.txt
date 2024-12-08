build_exec(
        EXECNAME experiment
        SOURCE_FILES experiment.cc event-message.cc random-event.cc event-generator.cc event-manager.cc vehicle-app.cc rsu-app.cc
        LIBRARIES_TO_LINK "${ns3-libs}" "${ns3-contrib-libs}"
        EXECUTABLE_DIRECTORY_PATH ${CMAKE_OUTPUT_DIRECTORY}/scratch/experiment
)