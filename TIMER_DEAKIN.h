#ifndef TIMER_DEAKIN_H
#define TIMER_DEAKIN_H

#include <Arduino.h>

// TIMER_DEAKIN class for timer operations
class TIMER_DEAKIN {
public:
    // Public methods
    void ConfigureTC3(int duration);
    void Start();
    bool CheckExpired();

private:
    // Static method to handle timer interrupt
    static void TC3_Handler();
    static volatile bool expired;  // Static flag to indicate if the timer has expired
    static void ResetExpired();
};

#endif
