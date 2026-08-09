# CSULB_ASME-Spring_2026-RoboticArm
Control code for a 4 DOF robotic arm built under the CSULB ASME chapter, based on the open source EEZYbotARM linkage system with several mechanical and electrical modifications.

## Overview
This code runs on an ESP32 and controls four servos corresponding to base rotation, vertical arm movement, horizontal arm movement, and gripper opening/closing. Wireless control is handled through the Dabble Bluetooth app using the DabbleESP32 library.

## Hardware
- ESP32
- HS-422 servos (x4)
- PCA9685 servo driver
- Power supplied by two 18650 Li-ion batteries, a power distribution board, a buck converter, and a 3.3V regulator

## Joint Ranges
Determined through testing rather than calculation:

| Joint | Range |
|---|---|
| Base rotation | 0 to 180 degrees |
| Vertical movement | 60 to 160 degrees |
| Horizontal movement | 0 to 120 degrees |
| Gripper | 0 to 180 degrees |

## Development Notes
Our team does not have a strong computer science or software background. Our general process was to attempt the programming ourselves first, using references found online, and turn to AI assistance when we got stuck. AI was used to help write a base version of this code, which we then adjusted based on our own testing and observations. For example, the joint ranges above came from physical testing, and the arm's smooth startup movement (rather than snapping directly into position) was implemented with AI's help after we described the issue we were observing.

## Status
This code reflects the first semester of development for this project. At the time development paused, pick and place task testing had not yet been completed due to time constraints.

Contributors:

Aymard Allen Padua: https://www.linkedin.com/in/aymard-allen-padua-b91302212/

Thy Le: https://www.linkedin.com/in/thynle/

Lorenzo Flores: https://www.linkedin.com/in/lorenzo-flores-a1781b387/

Jason Holzgang: https://www.linkedin.com/in/jason-holzgang-4334a9382/

Brian Vu: https://www.linkedin.com/in/brianpvu/



Special thanks to: 

Jon Tolden: https://www.linkedin.com/in/jontolden/
