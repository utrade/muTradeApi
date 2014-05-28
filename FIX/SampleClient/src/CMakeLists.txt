add_executable(fixClient
  samplestrategy.cpp
  application.cpp
  tradeclient.cpp
  getopt.c
  futFutStrategy.cpp
)

target_link_libraries(fixClient
  ${QUICKFIX_LIBRARIES}
  -lboost_system
  -lboost_program_options
)
