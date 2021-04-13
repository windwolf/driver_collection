# driver的BSP源文件和头文件
list_source_files(${CMAKE_CURRENT_LIST_DIR}/bsp/${DRIVERS_BSP} BSP_SOURCE_FILES)
set(BSP_INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/bsp/${DRIVERS_BSP}/inc)
target_sources(${PROJECT_NAME} 
    PRIVATE 
    ${BSP_SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${BSP_INCLUDE_PATH}
)

# driver的OS port
list_source_files(${CMAKE_CURRENT_LIST_DIR}/os/${DRIVERS_OS} OS_SOURCE_FILES)
target_sources(${PROJECT_NAME} 
    PRIVATE 
    ${OS_SOURCE_FILES}
)