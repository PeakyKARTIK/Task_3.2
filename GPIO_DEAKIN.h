#ifndef GPIO_DEAKIN_H
#define GPIO_DEAKIN_H

#include <ArduinoLowPower.h>

// GPIO_DEAKIN class for performing GPIO operations on SAMD microcontrollers
class GPIO_DEAKIN {
public:
    // Configures the specified GPIO pin as INPUT or OUTPUT
    bool Config_GPIO(char PortNum, char PinNum, char Mode);

    // Writes HIGH or LOW to the specified GPIO pin
    bool Write_GPIO(char PortNum, char PinNum, bool State);

    // Reads the state of the specified GPIO pin
    bool Read_GPIO(char PortNum, char PinNum);

    // Configures multiple GPIO pins with the same mode
    bool Config_GPIO_Pins(char* PinArray, char Mode);

    // Display a pattern on multiple GPIO pins
    bool GPIO_Display_Pattern(char* PinArray, char GPIOPattern);

    // Sequentially control GPIO pins with a delay
    void GPIO_Sequential_Control(char* PinArray, int duration);

private:
    // Get the base address of the specified port (A or B)
    uint32_t getPortBaseAddress(char PortNum);

    // Get the bit mask for the specified pin number
    uint32_t getPinMask(char PinNum);
};

#endif
