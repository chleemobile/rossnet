INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR})
INCLUDE_DIRECTORIES("/opt/local/include")

SET(air_traffic_srcs
	rose_air_traffic.cpp air_traffic.h 
	Aircraft.cpp Aircraft.hpp
	Graph.cpp Graph.hpp 
	Controller.cpp Controller.hpp
	rose_RegionController.cpp RegionController.hpp
	rose_LocalTrafficController.cpp LocalTrafficController.hpp
	Constants.hpp
	)


file(COPY ./adjlist.csv DESTINATION ${PROJECT_BINARY_DIR}/models/air_traffic/)
file(COPY ./run_gvt_batch.sh DESTINATION ${PROJECT_BINARY_DIR}/models/air_traffic/)
file(COPY ./s_run.sh DESTINATION ${PROJECT_BINARY_DIR}/models/air_traffic/)
file(COPY ./p_run.sh DESTINATION ${PROJECT_BINARY_DIR}/models/air_traffic/)

ADD_EXECUTABLE(air_traffic ${air_traffic_srcs})

TARGET_LINK_LIBRARIES(air_traffic ROSS m)
