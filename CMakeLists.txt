PROJECT (OS)
SET (SRC_LIST os.c config_parser.c memory.c core_log.c error_log.c 
    access_log.c)
INCLUDE_DIRECTORIES(.)
ADD_DEFINITIONS(-Wall -Wpointer-arith -Wno-unused-function)
ADD_EXECUTABLE(os ${SRC_LIST})
