INCLUDE_DIRECTORIES(${ROSS_SOURCE_DIR})
INCLUDE_DIRECTORIES("/opt/local/include")

SET(phold_srcs
phold.cpp		phold.h)

ADD_EXECUTABLE(phold ${phold_srcs})

TARGET_LINK_LIBRARIES(phold ROSS m)
