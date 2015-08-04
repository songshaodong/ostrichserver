PROJECT (OS)

SET (SRC_LIST acceptor.c common.c config.c event.c hashtable.c 
    main.c memory.c net.c scheduler.c thread.c osstring.c queue.c eventpoll.c
    continuation.c server.c connection.c processor.c mutex.c atomiclist.c
    atomic.c spinlock.c application.c http.c timer.c signals.c session.c notify.c dict.c 
    http_request.c recv.c http_parse.c reconfig.c)
    
CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )

INCLUDE_DIRECTORIES (.)

FIND_PACKAGE (Threads REQUIRED)

#ADD_DEFINITIONS (-g -Wall -Wpointer-arith -Wno-unused-function )

ADD_DEFINITIONS (-g)

ADD_EXECUTABLE (os ${SRC_LIST})

TARGET_LINK_LIBRARIES (os ${CMAKE_THREAD_LIBS_INIT} rt)

