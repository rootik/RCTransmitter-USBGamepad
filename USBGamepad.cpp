/* 
 * Copyright (c) 2016, rootik <roootik@gmail.com>
 * License: MIT
*/

#include "USBGamepad.h"

#if defined(_USING_HID)

static const uint8_t _hidReportDescriptor[] PROGMEM = {
	0x05, 0x01,			// USAGE_PAGE (Generic Desktop)
	0x09, 0x05,			// USAGE (Gamepad)
	0xA1, 0x01,			// COLLECTION (Application)
	0xA1, 0x00,     //   COLLECTION (Physical)
	0x85, 0x01,			//   REPORT_ID (1)
	0x05, 0x01,     //   USAGE_PAGE (Generic Desktop)
  0x09, 0x30,     //     USAGE (X - ROLL)
  0x09, 0x31,     //     USAGE (Y - PITCH)
	0x09, 0x32,     //     USAGE (Z - THROTTLE)
  0x09, 0x35,     //     USAGE (Rz - YAW)
  0x09, 0x36,     //     USAGE (Slider - AUX1)
  0x09, 0x36,     //     USAGE (Slider - AUX2)
  0x16, 0x00, 0x00,    //     LOGICAL_MINIMUM (0)
  0x26, 0xE4, 0x03,    //     LOGICAL_MAXIMUM (1000)
	0x95, 6,        //     REPORT_COUNT (6)
	0x75, 16,				//	   REPORT_SIZE (16)
	0x81, 0x02,		  //     INPUT (Data,Var,Abs)
	0xc0,				//   END_COLLECTION
	0xc0				// END_COLLECTION
};

USBGamepad_::USBGamepad_(void)
{
     static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
     HID().AppendDescriptor(&node);
}

void USBGamepad_::begin(uint16_t *report)
{
  memset(_report, 0x00, 6);
  SendReport(_report);
}

void USBGamepad_::write(uint16_t *report)
{
 for (int i = 0; i < 6; i++)
 {
  if (report[i] < 1000)
  {
    report[i] = 1000;
  }  
  _report[i] = constrain(report[i]-1000,1,1000);
 }
   SendReport(_report);
}

void USBGamepad_::SendReport(void *data)
{
  HID().SendReport(0x01, data, 12);
}

USBGamepad_ USBGamepad;

#endif /* if defined(_USING_HID) */
