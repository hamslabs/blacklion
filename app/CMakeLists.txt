# Copyright (c) Microsoft Corporation.
# Licensed under the MIT License.

set(SOURCES
    startup/${THREADX_TOOLCHAIN}/startup_gcc.c
    main.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        threadx
		driverlib
		utils
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        .
)

target_link_directories(${PROJECT_NAME}
    PUBLIC 
       startup/${THREADX_TOOLCHAIN}
)
target_compile_definitions(${PROJECT_NAME} PUBLIC "PART_TM4C1294NCPDT" )


if(CMAKE_C_COMPILER_ID STREQUAL "IAR")
    set_target_linker(${PROJECT_NAME} XXXXX.icf)
else()
    set_target_linker(${PROJECT_NAME} tm4c1249ncpdt.ld)
endif()

post_build(${PROJECT_NAME})
