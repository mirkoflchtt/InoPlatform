#ifndef __INO_MY_CONFIG_H
#define __INO_MY_CONFIG_H

#define MY_INO_ASSERT

#define INO_PLATFORM_MAJOR          (0x1)
#define INO_PLATFORM_MINOR          (0x1)
#define INO_PLATFORM_MICRO          (0x0)

#define INO_HANDLER_TIMER_INTERVAL_MS           (1000)

#define INO_HAS_LOG                 LOG_TRACE

#define INO_LOG_BUFFER_SIZE         (256)

#define MY_INO_BUTTON_LONG_PRESS    (500)     // msec
#define MY_INO_BUTTON_TIMEOUT       (1500)    // msec
#define MY_INO_BUTTON_DEBOUNCE      (100)     // msec

#define MY_INO_SENSOR_UPDATE_TEMPERATURE_INTERVAL (60*15)   // sec


#endif    /*__INO_MY_CONFIG_H*/
