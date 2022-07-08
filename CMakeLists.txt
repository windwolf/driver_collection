cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if(NOT DEFINED DRIVERS_BSP_PORT_NAME)
    message(FATAL_ERROR "Error: DRIVERS_BSP_PORT_NAME not defined")
else()
    message(STATUS "DRIVERS_BSP_PORT_NAME: ${DRIVERS_BSP_PORT_NAME}")
endif()

# BSP PORT
list_source_files(${CMAKE_CURRENT_LIST_DIR}/bsp/${DRIVERS_BSP_PORT_NAME} _BSP_SOURCE_FILES)
set(BSP_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/bsp/${DRIVERS_BSP_PORT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
    ${_BSP_SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${BSP_INCLUDE_PATH}
)

process_src_dir(${CMAKE_CURRENT_LIST_DIR}/common ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/device ${PROJECT_NAME})
process_src_dir(${CMAKE_CURRENT_LIST_DIR}/fonts ${PROJECT_NAME})

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
