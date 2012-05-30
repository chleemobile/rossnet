
INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR})
INCLUDE_DIRECTORIES("/opt/local/include")

SET(ping_pong_srcs
	ping_pong.cpp ping_pong.hpp )


ADD_EXECUTABLE(ping_pong ${ping_pong_srcs})

TARGET_LINK_LIBRARIES(ping_pong ROSS m)
