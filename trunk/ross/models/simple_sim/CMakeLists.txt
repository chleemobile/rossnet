INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR})
INCLUDE_DIRECTORIES("/opt/local/include")

SET(simple_sim_srcs
	simple_sim.c simple_sim.h )


ADD_EXECUTABLE(simple_sim ${simple_sim_srcs})

TARGET_LINK_LIBRARIES(simple_sim ROSS m)
