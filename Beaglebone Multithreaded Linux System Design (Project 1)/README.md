# Advanced-Embedded-Software-Development

# Beaglebone Multithreaded Linux System Design

## Author: Siddhant Jajoo and Satya Mehta.

This project was completed in the course Advanced Embedded Software Development at University of Colorado, Boulder under the guidance of professor Richard Heiderbecht in March 2019.


## Hardware Components:
- Beaglebone Green
- Temperature Sensor (TMP102) [Click here](https://www.sparkfun.com/products/13314)
- Light Sensor (APDS 9301) [Click here](https://www.broadcom.com/products/optical-sensors/ambient-light-photo-sensors/apds-9301)

## Software Components:
- Multithreading, Heartbeat, BIST, Timers
- Log Level and Error Checking/Handling
- Inter Process Communication (IPC)
- Mutexes, Signal Handlers
- I2C device driver (Incomplete)
- Request data from Remote Host using Sockets.
- Unit Testing.

## Features:
- Record Temperature at regular Intervals of time.
- Record Luminosity at regular Intervals of time.
- Log the data using a logger thread.
- Implementing different log levels - INFO, DEBUG, ERROR.
- Heartbeat, BIST, Unit Testing.
- Implementing Signal Handlerfor safe exit.
- Request Tempertaure and Light data from remote host using Sockets.

## Block Diagram:
<img src="Images/block%20diagram.PNG">


## Overview:
This project designs and implements a smart environment monitoring device using Beaglebone Green development board along with two off-board sensors. The project design includes concurrent software concepts for Linux that will interact with both User-Space and Kernel-Space in addition to multiple connected devices.

The two off-board sensors are:
- Temperature Sensor TMP102
- Light Sensor APDS 9301

The project is implemented by creating 4 threads named: 1. Temperature task 2. Light Task 3.Logger Task 4. Remote Host Task. Temperature and Light threads monitor the two sensors concurrently using POSIX timers to read data at specified intervals of time. The data is simultaneously recorded from the two sensors in independent threads. The data obtained is then logged into a text file which is obtained at runtime (i.e passing arguments to main). This data is backed up and a new text file is created whenever the program is run again. The logging of data to a text file is implemented by the logger thread. There is also a provision to provide specified amounts of data to a remote off-system host via remote request socket thread i.e the Remote Host thread. This thread uses Socket as an IPC Mechanism to request and send data to the remote host. All the data and each and every update in the system is written to a text file via the logger task.

The Zip file for the Code can be found in [Code Zip File](https://github.com/jajoosiddhant/Advanced-Embedded-Software-Development-AESD/tree/master/Beaglebone%20Multithreaded%20Linux%20System%20Design%20(Project%201)/Code%20Zip%20File) Folder.

The entire documentation of the project can be found in [Project 1 Report](https://github.com/jajoosiddhant/Advanced-Embedded-Software-Development-AESD/blob/master/Beaglebone%20Multithreaded%20Linux%20System%20Design%20(Project%201)/Project%201%20Report.pdf) pdf.
