get_filename_component(BORINGSSL_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set(BORINGSSL_CMAKE_DIR ${BORINGSSL_CMAKE_DIR})

set(jnilibs "${CMAKE_SOURCE_DIR}/../jniLibs")

include_directories(
        ${BORINGSSL_CMAKE_DIR}/openssl
)

#boringssl相关
add_library(ssl STATIC IMPORTED )
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libssl.a)

add_library(crypto STATIC IMPORTED )
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libcrypto.a)

set(BORINGSSL_SRC
        ssl
        crypto
        )