# Parking Assistant

A visual assistant for preventing car bumper collisions

This setup relies on creating a setpoint for a vehicle. The application continually
checks for obstructing objects, e.g., a car bumper and should show a solid green
color at reduced brightness when in idle mode. As a vehicle enters the sensor's
domain (within an advertised 400cm or 13 ft), the light will emit a bright green
to grab the driver's attention. As the vehicle moves towards the sensor, individual
pixels will illuminate to yellow indicating that the distances is decreasing and
approaching the setpoint. When the setpoint threshold is reached, all yellow pixels
will turn to red and will begin to flash as a secondary warning.

## Materials

1. Arduino platform (Microduino Core+ used here)
1. HC-SR04 ultrasonic sensor
1. Momentary push-button
1. Adafruit Neopixel strip (8 pixel bar used)

## Overview

Two primary modes exist: _Normal_ and _Train_. The _Train_ mode allows a setpoint to be set automatically when the button is released. To enter _Train_ mode, hold down the button. Typical operation, in _Normal_ mode does not require any user interaction.

### Train

![alt text](./graphic-mode-train.png 'Normal')

### Normal Operation (Idle)

![alt text](./graphic-mode-normal.png 'Normal')

### Object Within Range

![alt text](./graphic-mode-range.png 'Normal')

### Setpoint Reached

![alt text](./graphic-mode-warning.png 'Normal')
