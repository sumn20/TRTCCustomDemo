get_filename_component(THIRD_PARTY_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set(THIRD_PARTY_PATH ${THIRD_PARTY_CMAKE_DIR})

include(${THIRD_PARTY_PATH}/boringssl/boringssl.cmake)
include(${THIRD_PARTY_PATH}/oboe/oboe.cmake)
include_directories(
        ${THIRD_PARTY_CMAKE_DIR}/
)

set(THIRD_PARTY_SRC
        ${BORINGSSL_SRC}
        ${OBOE_SRC}
        )