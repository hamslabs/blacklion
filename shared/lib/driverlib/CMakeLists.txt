cmake_minimum_required(VERSION 3.22)

project(driverlib
	LANGUAGES C ASM
	VERSION 1.0.1
	DESCRIPTION "driverlib from TI")

# Get the api include definitions
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/include)

add_library(driverlib
	STATIC
	src/fpu.c
	src/comp.c
	src/aes.c
	src/onewire.c
	src/timer.c
	src/interrupt.c
	src/epi.c
	src/emac.c
	src/flash.c
	src/usb.c
	src/cpu.c
	src/des.c
	src/sw_crc.c
	src/sysctl.c
	src/gpio.c
	src/sysexc.c
	src/systick.c
	src/udma.c
	src/i2c.c
	src/shamd5.c
	src/eeprom.c
	src/watchdog.c
	src/hibernate.c
	src/qei.c
	src/adc.c
	src/lcd.c
	src/ssi.c
	src/pwm.c
	src/crc.c
	src/mpu.c
	src/uart.c
	src/can.c
)

set_target_properties(driverlib PROPERTIES VERSION ${PROJECT_VERSION})

#target_include_directories(driverlib
#	PUBLIC
#	.)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
	.
)



