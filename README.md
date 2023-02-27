# PLANT CARE INTEGRAL SYSTEM
The PLANT CARE INTEGRAL SYSTEM is a physical computing and IoT project that aims to monitor the state of a plant using different sensors and components. The project uses an ESP8266 and an ESP32-cam, connected to various sensors and devices to obtain data, process it, and send it to a website created manually and specifically for this project and to a phone app.

## Project Directory
The project directory is structured as follows:
```
root
│   README.md
│
└───code
│   │   esp32.ino
│   │   esp8266.ino
│   │   index.php
│   │   post-data.php
│
└───image
    │   sketch-fritzing.fzz
    │   photo-project-1.jpg
    │   photo-project-2.jpg
    │   photo-project-3.jpg
    │   diagram.jpeg
    │   Pre-build proposal-ESP8266
    │   Pre-build proposal-ESP32-CAM
    │   Post-build-complete-prototype.jpg
```
The code folder contains the code for both the ESP8266 and the ESP32-cam, as well as the PHP files for the website.
- The esp32.ino file contains the code for the ESP32-cam, while the esp8266.ino file contains the code for the ESP8266. 
- The index.php file is the code for the website's homepage, while the post-data.php file is the code that processes the requests from the boards and stores the data in the database.

The image folder contains the visual documentation for the project. 
- The sketch-fritzing.fzz file is the sketch file of the project for the Fritzing application. 
- The photo-project-1.jpg, photo-project-2.jpg, and photo-project-3.jpg files are pictures of the final project already built. 
- The diagram.jpeg file is a high-level diagram of the components and relations. 
- The Pre-build proposal-ESP8266 and Pre-build proposal-ESP32-CAM files are prototype designs from before building the project for each board. 
- Finally, the Post-build-complete-prototype.jpg file is the prototype design of the actual final connections.

## Installation
To use the PLANT CARE INTEGRAL SYSTEM, you will need to follow these steps:

1. Clone the repository to your computer
2. Open the code folder and open esp32.ino and esp8266.ino files in the Arduino IDE.
3. Connect the sensors and devices to the appropriate pins on the ESP8266 and ESP32-cam boards as per the circuit diagram.
4. Upload the code to both the ESP8266 and the ESP32-cam boards using the Arduino IDE.
5. Open the index.php and post-data.php files and update the database information and website URL.
6. Upload the files to a web server or a local development environment with PHP and MySQL installed. In the root folder create an `/uploads` directory where the photo of your plant will be saved.
7. Create a table called Sensor with the columns for each value (moisture, humidity, airTemperature, soilTemperature, reading_time (timestamp)).
8. Open the website in your browser and check if the data is being displayed.
9. Install the Blynk application on your mobile device and create a new project.
10. Add the appropriate widgets to the project and connect it to the ESP8266 board.
11. Control the water pump and LCD screen through the Blynk app.

## Components
The PLANT CARE INTEGRAL SYSTEM uses the following components:

ESP8266 connected to:
- DS18B20 sensor
- Soil moisture sensor
- DHT11 sensor
- Relay module and water pump
- Two push buttons (added via software in an application, the buttons don't exist physically)
- LCD screen

ESP32-cam connected to:
- OV2640 camera
- Being powered by an Arduino UNO (Arduino uno only used to give energy to ESP32)
- Connected to ESP8266 to load data to the ESP32

## Functionality
The PLANT CARE INTEGRAL SYSTEM can measure and display the following plant metrics:

- Moisture
- Soil temperature
- Air humidity
- Air temperature

The system transmits the moisture, soil temperature, air humidity, and air temperature to the LED screen, where it is displayed.

The system transfers this information of the plant to the iot.bluehost-testite-nunyvega.blog every 5 seconds, and the information is stored in a database. It is also displayed in the website, both the latest values and the historical values.

The system also sends a picture of the plant every 1 minute to the website, and this picture is displayed in iot.bluehost-testite-nunyvega.blog.

Additionally, the information is displayed in the Blynk application, and the water pump and LCD screen can be turned on and off from the app.

## Usage
To use the PLANT CARE INTEGRAL SYSTEM, you will need to follow these steps:

Ensure that the system is powered on and connected to the internet.
Check the website for the latest values of the plant metrics and the plant picture.
Use the Blynk app to turn on and off the water pump and LCD screen.

## Credits
The PLANT CARE INTEGRAL SYSTEM was created by Alvaro Vega. Special thanks to the #cm3040-physical-computing-internet-of-things module and UOL team, Fritzing team for providing the circuit diagram software, to the Arduino team for creating the Arduino IDE, and the tens of creators that share their knowledge in tutorials online.