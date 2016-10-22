/*
 * Copyright (c) 2016, rootik <roootik@gmail.com>
*/
// Include guard
#pragma once

#include <Arduino.h>
#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

class USBGamepad_
{
public:
	USBGamepad_(void);
  void begin(uint16_t *report);
	void write(uint16_t *report);

private:
  uint16_t _report[];
  void SendReport(void *data);
};

extern USBGamepad_ USBGamepad;

#endif /* if !defined(_USING_HID) */
