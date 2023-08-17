# Smart Home Control System

This project demonstrates a smart home control system using Arduino and NodeMCU. The system allows you to remotely control lights, doors, windows, and monitor humidity through a web interface.

## Features

- Control the state of lights, doors, and windows remotely.
- Monitor humidity levels in your environment.
- User authentication for door access.
- Real-time updates on the web interface.

## Hardware Components

- Arduino Uno (for remote control and NODEMCU communication)
- NodeMCU (for web interface and control)
- Servo Motors (for door and window control)
- Light Sensors
- Humidity Sensor
- LEDs (for simulating lights)
- Keypad (for password input)
- Water Level Sensor (for safety feature)

## Arduino Code

The Arduino code handles door control, water level monitoring, keypad input, and servo motor control. See the `Arduino.ino` file for the complete code.

## NodeMCU Code

The NodeMCU code implements the web interface, enabling remote control and monitoring. It also handles communication between Arduino and the web interface. See the `NodeMCU.ino` file for the complete code.

## Website Code

The web interface is designed using HTML, CSS, and JavaScript. The interface provides control buttons for lights, doors, and window, along with real-time humidity and brightness monitoring. The code of the document is available within `NodeMCU.ino` file.

## How to Use

1. Upload the Arduino code to your Arduino Uno.
2. Upload the NodeMCU code to your NodeMCU.
3. Assemble the hardware components.
4. Connect to NodeMCU access point.
5. Access the web interface by entering the NodeMCU's IP address `192.168.1.1` in a web browser.
6. Use the web interface to control lights, doors, and windows, and monitor humidity levels.

