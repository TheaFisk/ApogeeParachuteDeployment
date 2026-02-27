# Overview
This project is an Arduino-based flight computer designed for a home-built rocket using an Adafruit Feather M0 Adalogger. The program reads altitude data in real time from a BMP280 or BMP390 barometric pressure sensor connected via I2C. The software continuously analyzes altitude changes to detect apogee, which is the highest point reached during flight. When apogee is detected, the program sends a signal to trigger parachute deployment. At the same time, all altitude readings and important flight events are logged with timestamps to a microSD card for post-flight analysis. A buzzer and LED provide a system-ready signal prior to launch so there is no uncertainty.

My goal with this software was to create a reliable and practical embedded system that supports safe rocket recovery while collecting valuable flight data. I wanted to demonstrate my understanding of embedded C programming, sensor integration, hardware communication protocols, and real-time data analysis. This project showcases my ability to work with microcontrollers, interface with external sensors using I2C, implement event detection algorithms, and perform persistent data logging to removable storage media. It also reflects my interest in aerospace systems and experimental rocketry.

[Software Demo Video](INSERT_VIDEO_LINK_HERE)

---

# Development Environment
This project was developed using the following tools and technologies:

* Arduino IDE
* Visual Studio Code with Arduino Extension
* C / Arduino Embedded Programming Language
* Adafruit Feather M0 Adalogger (ATSAMD21 Microcontroller)
* BMP280 / BMP390 Barometric Pressure Sensor
* microSD Card Storage
* Git and GitHub for version control

The program is compiled using the Arduino toolchain and deployed directly to the Feather M0 microcontroller. The design allows for cross-platform development using Windows, macOS, or Linux systems.

---

# Useful Websites
The following resources were helpful while developing this project:

* https://www.arduino.cc/en/Reference/HomePage
* https://learn.adafruit.com/adafruit-feather-m0-adalogger
* https://learn.adafruit.com/adafruit-bmp280-barometric-pressure-plus-temperature-sensor-breakout
* https://learn.adafruit.com/adafruit-bmp388-bmp390-bmp3xx
* https://www.arduino.cc/reference/en/libraries/sd/
* https://www.arduino.cc/reference/en/language/functions/communication/wire/
* https://cdn-learn.adafruit.com/downloads/pdf/using-the-sd-card.pdf

---

# AI Disclosure
I used Claude AI as a development partner during this project. I primarily used it to help structure the flight computer logic, review embedded programming practices, and explore safe and reliable methods for detecting rocket apogee using altitude data.

In one specific part of the project, I asked for assistance designing the apogee detection algorithm that compares current altitude readings to previous values while tracking maximum altitude. The AI suggested using a threshold-based drop detection method combined with continuous altitude tracking. I did not copy the implementation verbatim. I adapted the approach to include configurable thresholds, integrated it into my event logging system, and added additional logic to prevent false triggers before launch detection.

Through this process, I learned how to implement event-driven logic in embedded systems, manage timing using microcontroller clock functions, and structure sensor data collection loops efficiently. I felt confident using the suggestions because I fully understood how the altitude comparison algorithm worked, tested it with simulated data, and successfully integrated it with hardware deployment controls and SD logging.

---

# Future Work
If I were to continue improving this project, I would like to:

* Add dual deployment support for both drogue and main parachutes
* Implement velocity-based apogee detection for improved reliability
* Add redundant sensor support for increased fault tolerance
* Implement filtering techniques such as moving averages or Kalman filtering to reduce sensor noise
* Add battery voltage monitoring and power health reporting
* Create a pre-flight diagnostic mode with sensor and storage verification
* Add wireless telemetry for live ground station monitoring
* Implement EEPROM or flash backup logging in case of SD failure
* Add a physical arming and safety switch system
* Expand the software into a modular multi-file architecture
* Develop a ground data visualization tool for analyzing flight logs
* Integrate GPS tracking for flight recovery assistance
