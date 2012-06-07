INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR})
INCLUDE_DIRECTORIES("/opt/local/include")
#INCLUDE_DIRECTORIES("/Users/lee1017/dev/boost/include/")


SET(air_traffic_srcs
	air_traffic.cpp		air_traffic.h Graph.cpp Graph.hpp)


ADD_EXECUTABLE(air_traffic ${air_traffic_srcs})

TARGET_LINK_LIBRARIES(air_traffic ROSS m)
