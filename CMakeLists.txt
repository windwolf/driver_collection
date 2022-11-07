cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if(NOT DEFINED BSP_PORT)
    message(FATAL_ERROR "Error: BSP_PORT not defined")
else()
    message(STATUS "BSP_PORT: ${BSP_PORT}")
endif()

# BSP PORT
list_source_files(${CMAKE_CURRENT_LIST_DIR}/bsp/${BSP_PORT} _BSP_SOURCE_FILES)
set(BSP_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/bsp/${BSP_PORT})
target_sources(${PROJECT_NAME}
    PRIVATE
    ${_BSP_SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${BSP_INCLUDE_PATH}
)

process_src_dir(${CMAKE_CURRENT_LIST_DIR}/common ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/accessor ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/device ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/device/cpp ${PROJECT_NAME})

if(NOT DEFINED WWDEVICE_USE_DEMO)
    message(STATUS "WWDEVICE_USE_DEMO: not set, use default value : OFF")
    set(WWDEVICE_USE_DEMO OFF)
else()
    message(STATUS "WWDEVICE_USE_DEMO: ${WWDEVICE_USE_DEMO}")
endif()

if(NOT DEFINED UART_PRINT_CHANNEL)
    message(STATUS "UART_PRINT_CHANNEL: not set, use default value : 1")
    set(UART_PRINT_CHANNEL 1)
endif()

if(${UART_PRINT_CHANNEL} EQUAL 1)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DUART_PRINT_CHANNEL1)
elseif(${UART_PRINT_CHANNEL} EQUAL 2)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DUART_PRINT_CHANNEL2)
elseif(${UART_PRINT_CHANNEL} EQUAL 3)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DUART_PRINT_CHANNEL3)
elseif(${UART_PRINT_CHANNEL} EQUAL 4)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DUART_PRINT_CHANNEL4)
endif()

if(${ENABLE_TEST})
    process_src_dir(${CMAKE_CURRENT_LIST_DIR}/demo ${PROJECT_NAME})
endif()

# 如果使用方没有提供自定义, 那么使用默认, 否则将自定义复制到custom_inc下

# 临时放自定义配置文件的目录
set(CUSTOM_INC_DIR ${CMAKE_CURRENT_BINARY_DIR}/custom_inc)

if(NOT DRIVERS_USER_FILE)
    message(STATUS "Using default drivers_user.h file")
    set(DRIVERS_USER_FILE ${CMAKE_CURRENT_LIST_DIR}/drivers_user_sample.h)
else()
    message(STATUS "Using custom drivers_user.h file from ${DRIVERS_USER_FILE}")
endif()

configure_file(${DRIVERS_USER_FILE} ${CUSTOM_INC_DIR}/drivers_user.h COPYONLY)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CUSTOM_INC_DIR}
)
