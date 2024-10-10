# Automated Robotic Arm That Learns | Ft. Tinkercad, Arduino Mega, EEZYbotARM

**Author:** Aarav Garg  
**Category:** Circuits, Arduino  
**License:** MIT License

---

## Introduction

Welcome to the "Automated Robotic Arm That Learns" project, featuring **Tinkercad**, **Arduino Mega**, and **EEZYbotARM**. In this project, you'll build a robotic arm that can either be manually controlled or taught to perform tasks automatically. It features two joysticks for control and a 2.4" TFT touchscreen display for interaction and visualization.

This robotic arm has **three Degrees of Freedom (DOF)** and a gripper, powered by **four MG90S metal gear servo motors**. You can control the arm manually or teach it tasks, which it can repeat in a loop until stopped.

Let’s dive into the details of how to build and program this learning robotic arm!

---

## Components

### 3.1 Components Required
- **Arduino Mega** (more I/O pins than Arduino UNO)
- **2 x Joystick Module**
- **2.4" TFT Touchscreen Display**
- **4 x MG90S Metal Gear Servo Motors**
- Jumper wires, male and female header pins
- LEDs (optional, for PCB)

### 3.2 Tools Required
- Soldering iron, wire cutter, and stripper
- Screwdriver, tweezers

### 3.3 Software Required
- **Arduino IDE**
- **Autodesk Tinkercad** (3D CAD software)
- **Autodesk Eagle** (for PCB design)

---

## Key Features

- **Manual Mode:** Control the arm using two joysticks for real-time operation.
- **Automated Mode:** Record sequences of tasks for the robotic arm to perform autonomously.
- **Touchscreen Control:** Utilize a responsive GUI on the TFT display for easy navigation and control.

---

## Evolution from V1.0 to V2.0

### Problems with V1.0:
- **Servo Jittering:** The SG90 plastic servos couldn’t handle the arm’s weight.
- **Display Issues:** The 16x2 alphanumeric display was insufficient.
- **Messy Wiring:** A makeshift Arduino shield and salvaged button panel caused a cluttered design.

### Changes in V2.0:
- **Improved Servos:** Upgraded to MG90S metal gear servos for stability.
- **Touchscreen Display:** Replaced the button panel with a larger TFT touch display for input and output.
- **Custom PCB:** Reduced wiring complexity by using a custom PCB for better organization.
- **3D-Printed Control Panel:** Designed and printed a control panel box using **Tinkercad** and **IAmRapid**.

---

## Working Algorithm

The robotic arm operates in two modes:
- **Manual Mode:** Control the arm directly using the joysticks.
- **Automated Mode:** Record and replay task sequences using the touch display. The arm will repeat the recorded movements in a loop until stopped.

---

## Building the Robotic Arm

### Why EEZYbotARM?
The EEZYbotARM is an open-source robotic arm that fits the purpose of this project. You can easily download the `.stl` files from Thingiverse and print the parts.

### Custom PCB:
I designed a custom PCB using **Eagle** to simplify wiring. The PCB serves as an **Arduino Mega shield** and organizes connections for the servos, joysticks, and display. You can either use this custom PCB or connect the components directly to the Arduino Mega, though it may result in a mess of wires.

---

## Control Panel Box

### Design:
The control panel was designed in **Tinkercad** and includes cutouts for the TFT display and joystick module. The robotic arm is placed at the front of the control panel on an extended base plate.

### Printing:
I used **IAmRapid** for 3D printing, and the parts were delivered within two days. The prints are of high quality and durable.

---

## Wiring the Components

### Without PCB:
- **Joysticks:**
  - X1: A8, Y1: A9
  - X2: A10, Y2: A11
  - GND, VCC: GND, +5V on Arduino Mega
- **Servo Motors:**
  - S1 OUT: Pin 22, S2 OUT: Pin 24, S3 OUT: Pin 26, S4 OUT: Pin 28
  - GND and VCC: Arduino Mega GND and +5V

### With Custom PCB:
If you’re using the custom PCB, simply plug the joystick modules and servo motors into their designated headers on the PCB.

---

## Coding the Robotic Arm

The provided code controls the robotic arm in both manual and automated modes. Detailed comments in the code explain the logic behind each part of the program, making it easy to understand.

---

## Troubleshooting

### Common Issues:
- **Display Not Working:** Check display connections or initialization in the code.
- **Servo Motors Jittering:** Replace faulty motors.
- **Joystick Control Inverted:** Reverse polarity of the joystick module.
- **Automated Mode Fails:** Review the code for logic errors.

---

## Conclusion

Congratulations on building an automated robotic arm that can learn! If you have any issues, feel free to troubleshoot using the guide above or check out the detailed comments in the code. Enjoy exploring more possibilities with your robotic arm!

---

## License

This project is licensed under the MIT License.
