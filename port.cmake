function(drivers_os_port drivers_dir)

target_include_directories(${PROJECT_NAME}
            PRIVATE
            ${drivers_dir}/os
            )

    # driver的OS port
    list_source_files(${drivers_dir}/os/${DRIVERS_OS}/src OS_SOURCE_FILES)
    target_sources(${PROJECT_NAME}
            PRIVATE
            ${OS_SOURCE_FILES}
            )
    target_include_directories(${PROJECT_NAME}
            PRIVATE
            ${drivers_dir}/os/${DRIVERS_OS}/inc
            )

endfunction()

function(drivers_bsp_port drivers_dir)

    # driver的BSP源文件和头文件
    list_source_files(${drivers_dir}/bsp/${DRIVERS_BSP}/src BSP_SOURCE_FILES)
    set(BSP_INCLUDE_PATH ${drivers_dir}/bsp/${DRIVERS_BSP}/inc)
    target_sources(${PROJECT_NAME}
            PRIVATE
            ${BSP_SOURCE_FILES}
            )
    target_include_directories(${PROJECT_NAME}
            PRIVATE
            ${BSP_INCLUDE_PATH}
            )

endfunction()


