#include <Arduino.h>

/* Provides CONSOLE_LOG functionality

Usage:
	Include "debugging_init.h" once in your main code
	Include "debugging_enable.h" to enable CONSOLE_LOG in a header file.
	Include "debugging_disable.h" to disable

	Once serial comms have been setup, set "__serial_is_ready = true" to enable output

*/

#ifndef SERIAL_READY
#define SERIAL_READY

// Flag to inform debugging scripts that they shoud output from now on
extern bool __serial_is_ready;

#endif

#define CONSOLE_LOG(...)  if (__serial_is_ready) { Serial.print(__VA_ARGS__); }
#define CONSOLE_LOG_LN(...) if (__serial_is_ready) { Serial.println(__VA_ARGS__); }
#define DEBUGGING_ENABLED
