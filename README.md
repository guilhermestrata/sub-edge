# Environmental Monitoring System
This project is an environmental monitoring system using an Arduino board. It reads temperature and light levels, displays them on an LCD screen, and logs anomalies to the EEPROM. The system also includes a red LED and a buzzer to signal when an anomaly is detected and uses an array of LEDs to visualize light intensity.

## Components
Arduino board
LiquidCrystal_I2C LCD
Temperature sensor (connected to A1)
Light sensor (connected to A0)
Red LED (connected to pin 11)
Buzzer (connected to pin 10)
Luminosity LEDs (connected to pins 0-5)
EEPROM
Circuit Diagram
Make sure to connect the components to the Arduino board according to the pin assignments mentioned above.

## Libraries
This project uses the following libraries:

- LiquidCrystal_I2C
- EEPROM
- Wire
You can install these libraries via the Arduino Library Manager.

## Installation
Connect the components to the Arduino board.
Install the required libraries.
Upload the provided script to the Arduino board.
