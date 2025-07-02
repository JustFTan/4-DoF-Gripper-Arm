# 4-DoF Gripper Arm

This project is an implementation of a robotic arm with 4 Degrees of Freedom (DoF), equipped with a gripper and a VL53L0X distance sensor. The system is designed to detect objects using both IR and VL53L0X sensors, then automatically approach, grip, and move objects to predefined positions.

## Features

- **4-DoF Arm:** Control of 4 servos for base, joints, and gripper.
- **Automatic Gripper:** Gripper opens and closes automatically based on object detection.
- **VL53L0X Distance Sensor:** For precise distance measurement of objects.
- **IR Sensor:** To detect the presence of objects.
- **Configurable Default, Start, and End Positions:** Key positions can be set in the code.
- **PWM Servo Driver (I2C) Control.**

## Required Hardware

- 1x 4-DoF robotic arm with servos
- 1x Adafruit PWM Servo Driver (PCA9685)
- 1x VL53L0X Distance Sensor (I2C)
- 1x IR (infrared) sensor
- Jumper wires, breadboard, and a suitable power supply

## Arduino Library Installation

Make sure you have installed the following libraries in the Arduino IDE:

- [Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)
- [Adafruit VL53L0X Library](https://github.com/adafruit/Adafruit_VL53L0X)

## Wiring

- Connect the servos to the PCA9685 channels as defined in the code (`BASE_SERVO`, `JOINT1_LEFT`, etc).
- Connect the VL53L0X to the I2C lines (SDA, SCL).
- Connect the IR sensor to the pin defined by `IR_PIN` (e.g., GPIO 32 on ESP32).

## How it Works

1. **Initialization:** All servos move to their default positions.
2. **Object Detection:** If the IR sensor detects an object, the arm will approach the object gradually based on the VL53L0X distance reading.
3. **Gripping:** Once close enough, the gripper closes to grip the object.
4. **Move:** The arm moves to the target position and releases the object.
5. **Reset:** Returns to the initial position, ready for the next cycle.

## Servo Pin Mapping (as per PCA9685 channel)

| Servo Name           | Channel |
|----------------------|---------|
| BASE_SERVO           | 0       |
| JOINT1_LEFT          | 1       |
| JOINT1_RIGHT (mirror)| 2       |
| JOINT2               | 3       |
| GRIPPER_ROTATE       | 4       |
| GRIPPER_OPEN_CLOSE   | 5       |

## Main Code

The main code can be found in [`src/main.cpp`](src/main.cpp).  
Make sure to adjust the PWM parameters in the `defaultPosition`, `posisiAwal`, and `posisiAkhir` arrays if your robot’s physical construction is different.

## Upload Instructions

1. Connect your board (e.g., ESP32/Arduino) to your computer.
2. Open `main.cpp` in the Arduino IDE.
3. Select the correct board and port.
4. Upload the code to the board.

## License

This project is free to use for educational and further development purposes.

**Contributor:**  
[JustFTan](https://github.com/JustFTan)
