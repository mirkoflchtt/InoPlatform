#ifndef INO_MY_CONFIG_H
#define INO_MY_CONFIG_H

#define MY_INO_ASSERT

#define INO_PLATFORM_MAJOR          (0x1)
#define INO_PLATFORM_MINOR          (0x6)
#define INO_PLATFORM_MICRO          (0x0)

#define INO_HANDLER_TIMER_INTERVAL_MS           (1000)

#define INO_HAS_LOG                 LOG_TRACE

#define MY_INO_BUTTON_LONG_PRESS    (500)     // msec
#define MY_INO_BUTTON_TIMEOUT       (1500)    // msec
#define MY_INO_BUTTON_DEBOUNCE      (100)     // msec

#define MY_INO_SENSOR_UPDATE_TEMPERATURE_INTERVAL (60*5)   // sec


#endif    /*INO_MY_CONFIG_H*/
