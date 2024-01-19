
cmake_minimum_required(VERSION 3.22.1)

project("trtccustom")

set(THIRD_PARTY_PATH ${CMAKE_SOURCE_DIR}/third_party)

include(${THIRD_PARTY_PATH}/thirdparty.cmake)

include(${CMAKE_SOURCE_DIR}/business.cmake)

target_link_libraries(
        ${BUSINESS_SRC}
        ${THIRD_PARTY_SRC}
        log
        #zlib
        z
        #数学函数动态库
        m
        OpenSLES
        EGL
        GLESv2
        atomic
        android
        camera2ndk
        mediandk
)