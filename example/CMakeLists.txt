
set(SOURCES
    # {{BEGIN_TARGET_SOURCES}}
	cortexm4_crt0.s
	sample_threadx.c
	tx_initialize_low_level.S
	tx_simulator_startup.s
	${CMAKE_CURRENT_LIST_DIR}/src/tx_thread_context_restore.S
	${CMAKE_CURRENT_LIST_DIR}/src/tx_thread_context_save.S
	${CMAKE_CURRENT_LIST_DIR}/src/tx_thread_interrupt_control.S
	${CMAKE_CURRENT_LIST_DIR}/src/tx_thread_schedule.S
	${CMAKE_CURRENT_LIST_DIR}/src/tx_thread_stack_build.S
	${CMAKE_CURRENT_LIST_DIR}/src/tx_thread_system_return.S
	${CMAKE_CURRENT_LIST_DIR}/src/tx_timer_interrupt.S
    # {{END_TARGET_SOURCES}}
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
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

target_link_directories(${PROJECT_NAME}
    PUBLIC 
		${CMAKE_CURRENT_LIST_DIR}/src
)

target_compile_definitions(${PROJECT_NAME} PUBLIC "PART_TM4C1294NCPDT" )

if(CMAKE_C_COMPILER_ID STREQUAL "IAR")
    set_target_linker(${PROJECT_NAME} XXXXX.icf)
else()
    set_target_linker(${PROJECT_NAME} tm4c1249ncpdt.ld)
endif()

post_build(${PROJECT_NAME})

