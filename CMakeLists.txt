add_executable(vine-relay
    src/main.c
    src/config.c
    src/log.c
)

target_include_directories(vine-relay PRIVATE src)

target_link_libraries(vine-relay PRIVATE trellis pthread)

# TURNS/DTLS: vine-relay uses mbedTLS directly for the TURN listener.
# libtrellis links mbedtls PRIVATE so we must add it explicitly here.
if(TARGET mbedtls)
    target_link_libraries(vine-relay PRIVATE mbedtls mbedx509 mbedcrypto)
endif()

set_target_properties(vine-relay PROPERTIES
    OUTPUT_NAME vine-relay
    C_STANDARD 11
)
