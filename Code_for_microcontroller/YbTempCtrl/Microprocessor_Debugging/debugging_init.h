#include <Arduino.h>

/* Provides CONSOLE_LOG functionality

Usage:
	Include "debugging_init.h" once in your main code
	Include "debugging_enable.h" to enable CONSOLE_LOG in a header file.
	Include "debugging_disable.h" to disable

	Once serial comms have been setup, set "__serial_is_ready = true" to enable output

*/

// Flag to inform debugging scripts that they shoud output from now on
bool __serial_is_ready = false;

#define CONSOLE_LOG(...)
#define CONSOLE_LOG_LN(...)
#undef DEBUGGING_ENABLED
