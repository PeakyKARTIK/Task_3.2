#include "GPIO_DEAKIN.h"

GPIO_DEAKIN gpio;

char pinArray[6] = {10, 11, 12, 13, 14, 15}; 

void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);

    // Configure pins as outputs
    if (!gpio.Config_GPIO_Pins(pinArray, OUTPUT)) {
        Serial.println("Failed to configure pins.");
        while (true) {}  
    } else {
        Serial.println("Pins configured successfully.");
    }

    // Testing the GPIO_Display_Pattern
    char pattern = 0b00101101;  
    if (!gpio.GPIO_Display_Pattern(pinArray, pattern)) {
        Serial.println("Failed to display pattern.");
        while (true) {}  
    } else {
        Serial.println("Pattern displayed successfully.");
    }

    delay(1000);  

    // Test GPIO_Sequential_Control
    int duration = 500;  
    gpio.GPIO_Sequential_Control(pinArray, duration);
}

void loop() {
    
}
