cmake_minimum_required(VERSION 2.8.3)
project(dlib_train_chain)

FIND_PACKAGE(dlib REQUIRED)

SET(OpenCV_MIN_VERSION "2.4.8")
FIND_PATH(OpenCV_DIR NAMES opencv-config.cmake OpenCVConfig.cmake PATH_SUFFIXES lib/cmake lib)
FIND_PACKAGE( OpenCV ${OpenCV_MIN_VERSION} REQUIRED COMPONENTS core contrib features2d imgproc legacy objdetect video highgui)
IF(OpenCV_FOUND)
        IF(${OpenCV_VERSION} VERSION_LESS ${OpenCV_MIN_VERSION} AND (SUPPORT_VIDEO OR SUPPORT_IMAGE) )
                MESSAGE(FATAL_ERROR "OpenCV version (${OpenCV_VERSION} is present, while ${OpenCV_MIN_VERSION}+ is required")
        ENDIF()
        include_directories(${OpenCV_INCLUDE_DIRS})
        MESSAGE("Found OpenCV ${OpenCV_VERSION} libraries ${OpenCV_LIBRARIES} and headers ${OpenCV_INCLUDE_DIRS}")
        IF(${OpenCV_VERSION_MAJOR} MATCHES 2 AND ${OpenCV_VERSION_MINOR} MATCHES 2)
                ADD_DEFINITIONS(-DUSE_OPENCV22)
        ELSE()
                ADD_DEFINITIONS(-DUSE_OPENCV)
        ENDIF()
ELSE()
        MESSAGE(FATAL_ERROR "OpenCV not found.")
ENDIF()

SET(CMAKE_BUILD_TYPE Release)

include_directories(${dlib_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -march=core-avx2 -O2")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -march=core-avx-i -O2")

# Unfortuantely this is necessary for my ide (qt-creator) to resolve dependencies...
file(GLOB DUMMY_HEADER_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h src/*.h)

add_executable(dtc-trainer
        training/src/dtc-trainer.cpp
        ${DUMMY_HEADER_LIST}
)

add_executable(dtc-single-detection
        detection/src/dtc-single-detection.cpp
        ${DUMMY_HEADER_LIST}
)


target_link_libraries(dtc-trainer
        ${dlib_LIBRARIES}
)

target_link_libraries(dtc-single-detection
        ${dlib_LIBRARIES}
        ${OpenCV_LIBRARIES}
)

set_property(TARGET dtc-trainer PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
set_target_properties(dtc-trainer PROPERTIES COMPILE_FLAGS -DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

set_property(TARGET dtc-single-detection PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
set_target_properties(dtc-single-detection PROPERTIES COMPILE_FLAGS -DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")

INSTALL(TARGETS dtc-trainer RUNTIME DESTINATION bin)
INSTALL(TARGETS dtc-single-detection RUNTIME DESTINATION bin)
INSTALL(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/training/dtc-collect-data" DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)