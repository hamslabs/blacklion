

add_library(utils
	STATIC
	cmdline.c
	cpu_usage.c
	eeprom_pb.c
	flash_pb.c
	isqrt.c
	random.c
	ringbuf.c
	scheduler.c
	sine.c
	softi2c.c
	softssi.c
	softuart.c
	spi_flash.c
	uartstdio.c
	ustdlib.c
)

target_link_libraries(utils driverlib)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	../../inc
	../inc
)

