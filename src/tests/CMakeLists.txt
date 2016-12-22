project(fluentcontainer-tests)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(libdir ${CMAKE_SOURCE_DIR}/src/lib)
SET(fluentdir ${libdir}/fluent)

include_directories(
    ${libdir}
)

set(${PROJECT_NAME}_LIB_SRCS
    ${fluentdir}/fluentcontainer.h
    ${fluentdir}/fluentcontainer.impl.fwd.h
    ${fluentdir}/fluentcontainer.impl.algo.h
    ${fluentdir}/fluentcontainer.impl.copy.h
    ${fluentdir}/fluentcontainer.impl.transform.h
    ${fluentdir}/fluentcontainer.impl.filter.h
    ${fluentdir}/fluentcontainer.hash.h
    ${fluentdir}/fluentcontainer.map.h
    ${fluentdir}/fluentcontainer.set.h
    ${fluentdir}/fluentcontainer.vector.h
)

set(${PROJECT_NAME}_SRCS
    testdata.cpp
    tst_contiguous.cpp
    tst_set.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_LIB_SRCS} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    gtest gtest_main gmock
)
