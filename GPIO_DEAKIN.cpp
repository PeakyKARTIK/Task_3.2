#include "GPIO_DEAKIN.h"
#include "TIMER_DEAKIN.h"
#include <Arduino.h>
#include <SAMDTimerInterrupt.h>
#include <SAMD_ISR_Timer.h>

// Function to get the base address of our port
uint32_t GPIO_DEAKIN::getPortBaseAddress(char PortNum) {
    return (PortNum == 'A') ? (uint32_t)&PORT->Group[0] : 
           (PortNum == 'B') ? (uint32_t)&PORT->Group[1] : 
           0; 
}

// Function to get the bit mask for a specific pin number
uint32_t GPIO_DEAKIN::getPinMask(char PinNum) {
    return (1ul << PinNum);
}

// Function to configure a GPIO pin as INPUT or OUTPUT
bool GPIO_DEAKIN::Config_GPIO(char PortNum, char PinNum, char Mode) {
    PortGroup *port = (PortGroup *)getPortBaseAddress(PortNum);
    if (!port) return false; // 

    uint32_t pinMask = getPinMask(PinNum);

    if (Mode == OUTPUT) {
        port->DIRSET.reg = pinMask; // Sets pin direction to OUTPUT
        Serial.print("Configured pin ");
        Serial.print(PinNum);
        Serial.println(" as OUTPUT.");
    } else if (Mode == INPUT) {
        port->DIRCLR.reg = pinMask; // Sets pin direction to INPUT
        Serial.print("Configured pin ");
        Serial.print(PinNum);
        Serial.println(" as INPUT.");
    } else {
        return false; // Return false if the mode is invalid
    }
    return true;
}

// Function to write HIGH or LOW to a GPIO pin
bool GPIO_DEAKIN::Write_GPIO(char PortNum, char PinNum, bool State) {
    PortGroup *port = (PortGroup *)getPortBaseAddress(PortNum);
    if (!port) return false; // Return false if the port is invalid

    uint32_t pinMask = getPinMask(PinNum);

    if (State == HIGH) {
        port->OUTSET.reg = pinMask; // Set pin HIGH
        Serial.print("Set pin ");
        Serial.print(PinNum);
        Serial.println(" HIGH.");
    } else if (State == LOW) {
        port->OUTCLR.reg = pinMask; // Set pin LOW
        Serial.print("Set pin ");
        Serial.print(PinNum);
        Serial.println(" LOW.");
    } else {
        return false; 
    }
    return true;
}

// Function to configure multiple GPIO pins with the same mode:

bool GPIO_DEAKIN::Config_GPIO_Pins(char* PinArray, char Mode) {
    for (int i = 0; i < 6; ++i) {
        if (!Config_GPIO('A', PinArray[i], Mode)) {
            return false; // Return false if any pin configuration fails
        }
    }
    return true;
}

// Function to use multiple GPIO pins:

bool GPIO_DEAKIN::GPIO_Display_Pattern(char* PinArray, char GPIOPattern) {
    for (int i = 0; i < 6; ++i) {
        bool state = (GPIOPattern & (1 << i)) != 0; // Determine pin state based on pattern
        if (!Write_GPIO('A', PinArray[i], state)) {
            return false; 
        }
    }
    return true;
}

// Function to sequentially control GPIO pins:

void GPIO_DEAKIN::GPIO_Sequential_Control(char* PinArray, int duration) {
    TIMER_DEAKIN timer;
    timer.ConfigureTC3(duration); // Configure timer with specified duration
    while (true) {
        for (int i = 0; i < 6; ++i) {
            Write_GPIO('A', PinArray[i], HIGH); // Turn pin ON
            timer.Start(); 
            while (!timer.CheckExpired()) {} // Wait for timer to expire
            Write_GPIO('A', PinArray[i], LOW); // Turn pin OFF
        }
        for (int i = 5; i >= 0; --i) {
            Write_GPIO('A', PinArray[i], HIGH); // Turn pin ON
            timer.Start(); 
            while (!timer.CheckExpired()) {} // Waits for timer to expire, then turns OFF
            Write_GPIO('A', PinArray[i], LOW); 
        }
    }
}

// Function to read the state of a GPIO pin:

bool GPIO_DEAKIN::Read_GPIO(char PortNum, char PinNum) {
    PortGroup *port = (PortGroup *)getPortBaseAddress(PortNum);
    if (!port) return false; // Return false if the port is invalid

    uint32_t pinMask = getPinMask(PinNum);
    return (port->IN.reg & pinMask) != 0; // Return true if pin is HIGH, false if LOW
}
