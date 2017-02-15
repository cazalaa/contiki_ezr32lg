#ifndef __PLATFORM_CONF_H__
#define __PLATFORM_CONF_H__

/*	CONFIG FILES */

typedef unsigned long clock_time_t;
typedef unsigned long long rtimer_clock_t;

/* 	CLOCK
	it handles most of the timed events used by the system and its applications. 
	The objective of the clock module is to generate a certain amount of ticks each second and to continue processes if their etimer or ctimer have expired.
*/

/* The frequency of the main processor */
// ARM Cortex-M3: up to 48 MHz
#define F_CPU 32000000
/* The desired amount of ticks per second */
#define CLOCK_CONF_SECOND 256

/*	RTIMER
	It differs from the clock module by giving extensive control to its users and not being used by Contiki itself. 
	Its sole purpose is to precisely schedule one event at a time and it is used by configuring a wake up time and a callback function 
*/

/* The desired amount of ticks per second */
#define RTIMER_ARCH_SECOND 32768
#define RTIMER_CLOCK_LT(a,b) ((signed short)((a)-(b)) < 0)

/*	WATCHDOG
	The watchdog is a hardware device that is able to recover a microcontroller from system errors
*/

// LSI clock frequency
#define F_LSI 37000
// Start watchdog
#define ENABLE_WATCHDOG 1
// Use independant watchdog
#define WATCHDOG_USE_IWDG 1
// Independant watchdog timeout in millisecond
#define WATCHDOG_IWDG_TIMEOUT 250

/* LEDS
*/

// Indicate that LEDs are available to use
#define PLATFORM_HAS_LEDS 1

/*	SENSORS
	list all sensor interfaces which can be used to retrieve the current measured value or to get notified on sensor changes.
*/
// Indicate that a button is present for use
#define PLATFORM_HAS_BUTTON 1

/* 	NODE MAC & NODE ID 
	In order to be able to connect to a device on the data link layer, it needs a unique MAC address.
*/

/*	RADIO DRIVER */
/* 	SLIP DRIVER */
/* 	SERIAL LINE DRIVER */

/* 	LOW POWER MODE */

// The frequency of the LSE clock */
#define F_LSE 32768
// Time in microseconds to wake up from low power mode
#define MCU_WAKE_UP_TIME 3400


#endif /* __PLATFORM_CONF_H__ */
