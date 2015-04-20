PROJECT (OS)

SET (SRC_LIST acceptor.c common.c config.c event.c hashtable.c 
    main.c memory.c net.c scheduler.c thread.c)
    
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

INCLUDE_DIRECTORIES (.)

FIND_PACKAGE (Threads REQUIRED)

ADD_DEFINITIONS (-g -Wall -Wpointer-arith -Wno-unused-function)

ADD_EXECUTABLE (os ${SRC_LIST})

TARGET_LINK_LIBRARIES (os ${CMAKE_THREAD_LIBS_INIT})

