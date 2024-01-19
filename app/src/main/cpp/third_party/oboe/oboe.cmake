get_filename_component(OBOE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set(OBOE_CMAKE_DIR ${OBOE_CMAKE_DIR})

set(jnilibs "${CMAKE_SOURCE_DIR}/../jniLibs")

include_directories(
        ${OBOE_CMAKE_DIR}/
)

#oboe
add_library(oboe SHARED IMPORTED )
set_target_properties(oboe PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/liboboe.so)


set(OBOE_SRC
        oboe
        )