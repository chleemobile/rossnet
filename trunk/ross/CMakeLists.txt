PROJECT(ROSS C)
INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR})
#INCLUDE_DIRECTORIES("/Users/lee1017/dev/boost/include/")
INCLUDE_DIRECTORIES("/home/chayong82/bin/boost_1_42/include/")

SET(ross_srcs
clock-${CLOCK}.c
clock-${CLOCK}.h
network-${NETWORK}.c
network-${NETWORK}1.h
network-${NETWORK}2.h
hash-quadratic.c
hash-quadratic.h
queue-${QUEUE}.c
rand-${RAND}.c
rand-${RAND}.h
ross-global.c
signal-sigaction.c
tw-event.c
gvt-${GVT}.c
gvt-${GVT}.h
tw-kp.c
tw-lp.c
tw-opts.c
tw-opts.h
tw-pe.c
tw-timer.c
tw-timer.h
ross-random.c
ross-random.h
tw-sched.c
tw-setup.c
tw-signal.c
tw-state.c
tw-stats.c
tw-timing.c
tw-timing.h
tw-util.c)

IF(MEMORY)
	SET(OPTIONS "${OPTIONS} -DROSS_MEMORY")
	SET(ross_srcs ${ross_srcs} tw-memory.c tw-memoryq.h tw-memory.h)
ENDIF(MEMORY)

IF(RAND_NORMAL)
	SET(OPTIONS "${OPTIONS} -DRAND_NORMAL")
ENDIF(RAND_NORMAL)

IF(TIMING)
	SET(OPTIONS "${OPTIONS} -DROSS_timing")
ENDIF(TIMING)

IF(NETWORK STREQUAL tcp)
	   SET(ross_srcs ${ross_srcs} socket-tcp.c socket-tcp.h)
ENDIF(NETWORK STREQUAL tcp)

SET(OPTIONS "${OPTIONS} -DROSS_QUEUE_${QUEUE} -DROSS_RAND_${RAND} -DROSS_NETWORK_${NETWORK} -DROSS_CLOCK_${CLOCK} -DROSS_GVT_${GVT} -DARCH_${ARCH}")

ADD_DEFINITIONS(${OPTIONS})

#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPTIONS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROSS_OPTION_LIST='\"${OPTIONS}\"'")
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROSS_OPTION_LIST='\"${CMAKE_C_FLAGS}\"'")

#INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})

ADD_LIBRARY(ROSS ${ross_srcs})
ADD_SUBDIRECTORY(models)
