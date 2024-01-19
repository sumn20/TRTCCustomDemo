get_filename_component(BUSINESS_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(BUSINESS_CMAKE_DIR ${BUSINESS_CMAKE_DIR})
set(jnilibs "${CMAKE_SOURCE_DIR}/../jniLibs")

include_directories(
        ${BUSINESS_CMAKE_DIR}/basic/
        ${BUSINESS_CMAKE_DIR}/trtc/
        ${BUSINESS_CMAKE_DIR}/trtc/include/
        ${BUSINESS_CMAKE_DIR}/player/
        ${BUSINESS_CMAKE_DIR}/player/audio/
        ${BUSINESS_CMAKE_DIR}/player/video/
        ${BUSINESS_CMAKE_DIR}/util/
)
file(GLOB BUSINESS_CPP_FILES
        ${BUSINESS_CMAKE_DIR}/*.cpp
        ${BUSINESS_CMAKE_DIR}/util/*.cpp
        ${BUSINESS_CMAKE_DIR}/basic/*.cpp
        ${BUSINESS_CMAKE_DIR}/trtc/*.cpp
        ${BUSINESS_CMAKE_DIR}/player/audio/*.cpp
        ${BUSINESS_CMAKE_DIR}/player/video/*.cpp
        )
add_library(${CMAKE_PROJECT_NAME} SHARED
        ${BUSINESS_CPP_FILES})

#trtc
add_library(trtc SHARED IMPORTED )
set_target_properties(trtc PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libtrtc.so)

set(BUSINESS_SRC
        ${CMAKE_PROJECT_NAME}
        trtc
        )