INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR})
INCLUDE_DIRECTORIES("/opt/local/include")

SET(airport_srcs
	airport.cpp		airport.h)


ADD_EXECUTABLE(airport ${airport_srcs})

TARGET_LINK_LIBRARIES(airport ROSS m)
