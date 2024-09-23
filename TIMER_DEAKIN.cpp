#include "TIMER_DEAKIN.h"

// Initialize static member to keep track of the timer expiration status
volatile bool TIMER_DEAKIN::expired = false;

// Configures the TC3 timer
void TIMER_DEAKIN::ConfigureTC3(int duration) {
    // Enable the Generic Clock (GCLK) for TC3 and TC4 using GCLK0 as the clock source
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_TC4_TC5) | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;

    // Wait for synchronization to complete before proceeding
    while (GCLK->STATUS.bit.SYNCBUSY);

    // Performing a software reset of the TC3 peripheral 
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);    // Wait for the reset to complete
    while (TC3->COUNT16.CTRLA.bit.SWRST);        // Ensure reset bit is cleared

    // Configure TC3 for 16-bit counter mode
    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;

    // Set the prescaler to divide the clock by 256, and set the wave generation mode to match frequency (MFRQ)
    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV256 | TC_CTRLA_WAVEGEN_MFRQ;

    // Set the Compare-Capture (CC) register to trigger the timer at the specified duration (in milliseconds)
    TC3->COUNT16.CC[0].reg = (48000000 / 256) * (duration / 1000.0);  // Timer frequency calculation
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);                         // Wait for synchronization

    // Enable the TC3 interrupt in the Nested Vectored Interrupt Controller (NVIC)
    NVIC_EnableIRQ(TC3_IRQn);
    TC3->COUNT16.INTENSET.reg = TC_INTENSET_OVF;  // Enable overflow interrupt

    // Enable the timer by setting the ENABLE bit
    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);     // Wait for the timer to synchronize
}

// Starts the timer by resetting the expired flag and retriggering the counter
void TIMER_DEAKIN::Start() {
    ResetExpired();  // Reset the expired flag before starting
    TC3->COUNT16.CTRLBSET.reg = TC_CTRLBSET_CMD_RETRIGGER;  // Retrigger the timer
    while (TC3->COUNT16.STATUS.bit.SYNCBUSY);  // Wait for synchronization
}

// Checks if the timer has expired by returning the status of the expired flag
bool TIMER_DEAKIN::CheckExpired() {
    return expired;  // Return true if the timer has expired, false otherwise
}

// Resets the expired flag to indicate the timer has not yet expired
void TIMER_DEAKIN::ResetExpired() {
    expired = false;  // Clear the expired flag
}

// Interrupt Service Routine (ISR) for TC3
void TIMER_DEAKIN::TC3_Handler() {
    // Check if the overflow (OVF) interrupt has occurred
    if (TC3->COUNT16.INTFLAG.bit.OVF) {
        TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;  // Clear the overflow interrupt flag
        expired = true;                             // Set the expired flag to true
    }
}
