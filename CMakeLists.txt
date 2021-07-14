cmake_minimum_required(VERSION 3.13)
project(PreProjetStage)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fPIC")

add_executable(PreProjetStage main.cpp RedisSubdivision.cpp RedisUsingEvents.cpp RedisUsingEvents.h RedisSubdivision.h Ensemble.h Ensemble.cpp Couple.h Couple.cpp RedisZSet.h RedisZSet.cpp RedisSizeManager.cpp RedisSizeManager.h RedisInserter.h RedisInserter.cpp ConnectedTool.cpp ConnectedTool.h funcFromApp.cpp funcFromApp.h TestInserter.cpp TestInserter.h)

# <------------ add cassandra dependency --------------->
find_path(CASSANDRA_HEADER apache-cassandra-3.11.10)
target_include_directories(PreProjetStage PUBLIC ${CASSANDRA_HEADER})

# <------------ add cassandra driver dependency -------------->
find_path(CASSANDRA_HEADER cassandra)
target_include_directories(PreProjetStage PUBLIC ${CASSANDRA_HEADER})
set(CASSANDRA_LIB /usr/local/lib/libcassandra.so)
#find_library(CASSANDRA_LIB cassandra) # Je ne sais pas pourquoi cette ligne ne fonctionne pas. Du coup je donne la localisation en brut
target_link_libraries(PreProjetStage ${CASSANDRA_LIB})


# <------------ add hiredis dependency --------------->
find_path(HIREDIS_HEADER hiredis)
target_include_directories(PreProjetStage PUBLIC ${HIREDIS_HEADER})
find_library(HIREDIS_LIB hiredis)
target_link_libraries(PreProjetStage ${HIREDIS_LIB})

# <------------ add redis-plus-plus dependency -------------->
find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(PreProjetStage PUBLIC ${REDIS_PLUS_PLUS_HEADER})
find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(PreProjetStage ${REDIS_PLUS_PLUS_LIB})

#message(STATUS "CASSANDRA_HEADER=${CASSANDRA_HEADER}")
#message(STATUS "CASSANDRA_LIB=${CASSANDRA_LIB}")
#message(STATUS "")
#message(STATUS "REDIS_PLUS_PLUS_HEADER=${REDIS_PLUS_PLUS_HEADER}")
#message(STATUS "REDIS_PLUS_PLUS_LIB=${REDIS_PLUS_PLUS_LIB}")
#message(STATUS "")
#message(STATUS "HIREDIS_HEADER=${HIREDIS_HEADER}")
#message(STATUS "HIREDIS_LIB=${HIREDIS_LIB}")
#message(STATUS "")
#message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")