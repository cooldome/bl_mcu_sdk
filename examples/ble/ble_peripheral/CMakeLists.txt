set(BSP_COMMON_DIR ${CMAKE_SOURCE_DIR}/bsp/bsp_common)
set(TARGET_REQUIRED_SRCS ${CMAKE_CURRENT_LIST_DIR}/ble_peripheral_tp_server.c)
set(TARGET_REQUIRED_LIBS freertos ble)
set(mains main.c)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/examples/ble/bl702_flash_ble.ld)
generate_bin()


