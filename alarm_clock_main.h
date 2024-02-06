#ifndef ALARM_CLOCK_MAIN_H
#define ALARM_CLOCK_MAIN_H

#if defined(MCU_IS_ESP32)
#include "WiFi.h"
#endif
#include "pin_defs.h"
#include <PushButtonTaps.h>
#include "rgb_display_class.h"
#include "uRTCLib.h"
// #include <XPT2046_Touchscreen.h>


// forward decleration of other classes
class rgb_display_class;


class alarm_clock_main {

public:

// FUNCTIONS

  // constructor / destructor
  alarm_clock_main() {};
  ~alarm_clock_main() {};

  // function declerations
  void setup(rgb_display_class* disp_ptr);
  void loop();
  void rtc_clock_initialize();
  // interrupt ISR
  static void sqwPinInterruptFn();
  void serialTimeStampPrefix();
  void updateSecond();
  void serial_input_flush();
  void processSerialInput();
  #if defined(MCU_IS_ESP32)
  void print_wakeup_reason(esp_sleep_wakeup_cause_t &wakeup_reason);
  void putEsp32ToLightSleep();
  #endif

// OBJECTS and VARIABLES

  // Push Button
  PushButtonTaps pushBtn;

  // XPT2046_Touchscreen touchscreen(TS_CS_PIN, TS_IRQ_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

  // RTC clock object for DC3231 rtc
  uRTCLib rtc;

  // display object
  rgb_display_class* display;

  // seconds blinker
  bool blink = false;

  // seconds counter to track RTC HW seconds
  // we'll refresh RTC time everytime second reaches 60
  // All other parameters of RTC will not change at any other time
  // at 60 seconds, we'll update the time row
  byte second = 0;

  // flag to refresh RTC time from RTC HW
  bool refreshRtcTime = false;

  // flag to set alarm On or Off
  bool alarmOn = true;

  // counter to note user inactivity seconds
  uint8_t inactivitySeconds = 0;
  const uint8_t INACTIVITY_SECONDS_LIM = 120;

  // seconds flag triggered by interrupt
  static inline volatile bool secondsIncremented;

};


#endif     // ALARM_CLOCK_MAIN_H