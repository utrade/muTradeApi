add_executable(sampleStrategy
  application.cpp
) 

add_library(muTradeAPI STATIC IMPORTED)


set_target_properties(muTradeAPI PROPERTIES IMPORTED_LOCATION 
                      "${futFutTest_SOURCE_DIR}/bin/libmuTradeAPI.a")


if(WIN32)
target_link_libraries(sampleStrategy
   muTradeAPI
   ssleay32
   libeay32
   pthreadVC2
   ${Boost_SYSTEM_LIBRARY}
   ${Boost_PROGRAM_OPTIONS_LIBRARY}
   ${Boost_THREAD_LIBRARY}
   ${Boost_REGEX_LIBRARY}
   )
endif(WIN32)

if(UNIX)
target_link_libraries(sampleStrategy
   muTradeAPI
   ssl
   crypto
   #${Boost_SYSTEM_LIBRARY}
   #${Boost_PROGRAM_OPTIONS_LIBRARY}
   #${Boost_THREAD_LIBRARY}
   #   ${Boost_REGEX_LIBRARY}
   )
endif(UNIX)

