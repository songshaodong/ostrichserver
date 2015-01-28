PROJECT (OS)

SET (SRC_LIST os.c config_parser.c memory.c core_log.c error_log.c 
    access_log.c event.c eventpoll.c signal.c evthread.c netevent.c
    acceptor.c)
    
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

INCLUDE_DIRECTORIES (.)

FIND_PACKAGE (Threads REQUIRED)

ADD_DEFINITIONS (-g -Wall -Wpointer-arith -Wno-unused-function)

ADD_EXECUTABLE (os ${SRC_LIST})

TARGET_LINK_LIBRARIES (os ${CMAKE_THREAD_LIBS_INIT})
