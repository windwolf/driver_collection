cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# Set up the project
project(drivers
    VERSION 6.0.0 
    LANGUAGES C CXX ASM
)
set(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED DRIVERS_BSP)
    message(FATAL_ERROR "Error: DRIVERS_BSP not defined")
endif()
if(NOT DEFINED DRIVERS_OS)
    message(FATAL_ERROR "Error: DRIVERS_OS not defined")
endif()

add_library(${PROJECT_NAME})
add_library("windwolf::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})

# 临时放自定义配置文件的目录
set(CUSTOM_INC_DIR ${CMAKE_CURRENT_BINARY_DIR}/custom_inc)



# common
list_source_files(${CMAKE_CURRENT_LIST_DIR}/common/src COMMON_SROUCE_FILES)
target_sources(${PROJECT_NAME}
    PRIVATE
	${COMMON_SROUCE_FILES}
)
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/common/inc
    
)

# 驱动内容
list_source_directories(${CMAKE_CURRENT_LIST_DIR} DRIVER_COMPONENTS)
list(REMOVE_ITEM DRIVER_COMPONENTS "common" "bsp" "os")
foreach(DRIVER_COMPONENT ${DRIVER_COMPONENTS})
    list_source_files(${CMAKE_CURRENT_LIST_DIR}/${DRIVER_COMPONENT} DRIVER_COMPONENT_SOURCE_FILES)
    target_sources(${PROJECT_NAME}
        PRIVATE
	    ${DRIVER_COMPONENT_SOURCE_FILES}
    )
    target_include_directories(${PROJECT_NAME} 
        PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/${DRIVER_COMPONENT}/inc
    )
endforeach()

# 如果使用方没有提供自定义, 那么使用默认, 否则将自定义复制到custom_inc下
if (NOT DRIVERS_USER_FILE)
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

# # Enable a build target that produces a ZIP file of all sources
# set(CPACK_SOURCE_GENERATOR "ZIP")
# set(CPACK_SOURCE_IGNORE_FILES
#   \\.git/
#   \\.github/
#   _build/
#   \\.git
#   \\.gitattributes
#   \\.gitignore
#   ".*~$"
# )
# set(CPACK_VERBATIM_VARIABLES YES)
# include(CPack)