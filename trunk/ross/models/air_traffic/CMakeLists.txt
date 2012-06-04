INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR})
INCLUDE_DIRECTORIES("/opt/local/include")

SET(air_traffic_srcs
	air_traffic.cpp air_traffic.h Graph.cpp Graph.hpp )


file(COPY ./adjlist.csv DESTINATION ${PROJECT_BINARY_DIR}/models/air_traffic/)
file(COPY ./p_run_loop.sh DESTINATION ${PROJECT_BINARY_DIR}/models/air_traffic/)

ADD_EXECUTABLE(air_traffic ${air_traffic_srcs})

TARGET_LINK_LIBRARIES(air_traffic ROSS m)
