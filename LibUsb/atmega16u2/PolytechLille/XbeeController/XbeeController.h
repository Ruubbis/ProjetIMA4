
#ifndef _XBEECONTROLLER_H_
#define _XBEECONTROLLER_H_

#define DATA_MAX 5
	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>

		#include "Descriptors.h"

		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>
		#include <LUFA/Common/Common.h>
		#include <LUFA/Drivers/Peripheral/Serial.h>


	/* Function Prototypes: */
		void SetupHardware(void);
		void PAD_Task(void);
		void SendNextReport(void);
		void ReceiveNextReport(void);
#endif

