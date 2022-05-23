
/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define TX_TIMER_TICKS_PER_SECOND ((ULONG) 25)

#include <stdint.h>
#include <stdbool.h>
#include "tx_api.h"
#include "hw_memmap.h"
#include "debug.h"
#include "gpio.h"
#include "pin_map.h"
#include "sysctl.h"
#include "uart.h"
#include "uartstdio.h"

#define DEMO_STACK_SIZE         1024
#define DEMO_BYTE_POOL_SIZE     9120    // Reserve RAM for threads
#define DEMO_BLOCK_POOL_SIZE    100
#define DEMO_QUEUE_SIZE         100

#define ARR_SIZE(a) sizeof(a)/sizeof(*a)

/* Define the ThreadX object control blocks...  */

TX_THREAD     cmd_thread_0;
TX_THREAD     thread_0;
TX_THREAD     thread_1;
TX_THREAD     thread_1_1;
TX_THREAD     thread_temp;
TX_BYTE_POOL  byte_pool_0;
UCHAR         memory_area[DEMO_BYTE_POOL_SIZE];


/* Define the counters used in the demo application...  */

ULONG thread_0_counter;
ULONG thread_1_counter;

/* Define thread prototypes.  */

void thread_0_entry(ULONG thread_input);
void thread_1_entry(ULONG thread_input);
void thread_temp_entry(ULONG thread_input);
void cmd_fill_tank(ULONG thread_input);


char *text = "World!\r\n"; // TODO: Figure out a better way to pass data to functions




void
SysTickIntHandler(void)
{
}

//*****************************************************************************
//
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//
//*****************************************************************************
void
InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}



/* Define main entry point.  */

int main()
{

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    InitConsole();
  /* Enter the ThreadX kernel.  */
  tx_kernel_enter();
}



/* Define what the initial system looks like.  */

void tx_application_define(void *first_unused_memory)
{
  // TODO: I think all command threads shoud be initialized here using the config file 
  CHAR *pointer = TX_NULL;

  /* Create a byte memory pool from which to allocate the thread stacks.  */
  tx_byte_pool_create(&byte_pool_0, "byte pool 0", memory_area, DEMO_BYTE_POOL_SIZE);

  /* Put system definition stuff in here, e.g. thread creates and other assorted
      create information.  */

  /* Allocate the stack for thread 0.  */
  tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

  /* TODO: Lets manaully do this as a test!
  {
    "id": "cmd_fill_tank",
    "functions": [ ... ]
  }
  */
  bool power_value = true;
  /* Create the thread 1.  */
  tx_thread_create(&cmd_thread_0, "cmd_fill_tank", cmd_fill_tank, (ULONG)&power_value, pointer, DEMO_STACK_SIZE, 
            1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Create the main thread 0.  */
  // tx_thread_create(
  //   &thread_0,        // global point to thread -> Can this be created dynamically during init?
  //   "thread 0",       // thread point name --> Set using the "ID" from config.json for command. Need to be unique?
  //   thread_0_entry,   // thread function call -> Set based on command type(?)
  //   0,                // 32-bit entry_input used to pass data to app (thread_input ?)
  //   pointer,          // stack start address -> Use tx_byte_allocate beforehand to get the next available location
  //   DEMO_STACK_SIZE,  // stack size --> set to worst case value (needs to account for nested functions & local variable)
  //   1,                // priority --> 0 (highest) to TX_MAX_PRIORITES-1 (lowest)
  //   1,                // only priorities higher than this can prempt this thread. -> valuw equal to priority disables this. 
  //   TX_NO_TIME_SLICE, // number of timer ticks allowed for this thread to run before letting others time --> If prempt is used, this is disabled
  //   TX_AUTO_START);   // auto start thread or use thread resume to start
}

/* Define the test threads.  */
// TODO: Figure out if there is a way to cmd thread generic (or does that not matter) --> Could use thread_input to specify command type 
void cmd_fill_tank(ULONG thread_input)
{
  /* TODO: Think about how these are created. 
    - Should the functions have their thread initialized in tx_application_define or on the fly (in this command) or just call a function? 
      --> I think if there is space, reserving the function thread in tx_application_define is best (then controlling them from this thread).
        --> Controlling threads could be done using Mutex, Semaphores, Queues, etc.
        --> The command thread would end, but the function threads would continue to run in case other command threads need them + feedback to UI + Safety + etc.
      --> Otherwise, there will need to be checks for memory space and clean up of function threads.
    - How does config data pass to function / command threads ? --> This would probably be easier to do during init process. Unless, the json is quick to read
      from flash, then we can do it on the fly.
    - How do we pass data between threads? --> There are tools for that... Semaphores, Queues, etc.
  ...
  "functions": [
  {
    "id": "level_sensor_01",
    "ouputs": [ "level" ]
  },
  {
    "id": "motor_01",
    "inputs": [ "on_off" ]
  }]
  ...
  */

  // TODO: Temporary (very simple) example of tank fill command + functions
  // uint8_t cmd_level = 220; // TODO: Get input for command somehow instead of static variable
  // bool temp = (bool*)thread_input;
  // while(1) {
    // Turn on leds (double check schematic)
//        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, !(bool)thread_input);
//    gpio_set_pin_level(RX_LED, !(bool)thread_input);
//    gpio_set_pin_level(TX_LED, !(bool)thread_input);
  //   uint8_t level;
  //   level = readSensor("level_sensor_01");

  //   if (level < 220) {
  //     digital_out("motor_01", true);
  //     break;
  //   } else {
  //     digital_out("motor_01", false);
  //   }
  //   tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND * 2); // Sleep for 1 sec using ticks.
  // }

  /* TODO: How do threads return with values?
            --> I do not think we need a return but rather Queues to send data to other threads 
                Examples...
                - Communicate with black lion
                - Communicate with other threads
  */
}
