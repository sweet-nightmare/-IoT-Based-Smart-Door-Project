# Smart Door Berbasis IoT 

**Name:**
1. FERIKHO FATIH AZHAR (1103202190)
2. MOHAMAD AQWAM FARID (1103201263
3. KRESHNA PUTRA S. 1103202215
4. NURDIN 1103204006
5. ADAM AL-AZIZ OLII 1103204045

# Short project overview
This project is aimed to construct a simple iot device with 2 sensors for placement besides your plant that monitors the temperature and air humidity around the plant aswell as the humidity in the plants soil in order to se when you need to water your plant. 

**Approxiamtion of time needed for the project:**

Around one hour to two hours


# Objective
**Reasons for choosing the project:**
The reason i choose this project is that i have some plants in my home and have the habit of sometimes forgetting to water them in time before they start to wither so being able to the when i should water the plants depending on the soils moisture level will probably counteract that. 

**Purposes of the project:**
The purpose of this project is firstly to learn about the Inter of things, how to build a iot device and connect it to a platform to visualize its data. And secondly to monitor my plants to see when i need to water them.

**Insights gained by doing the project:**
The project is able to give insights in basic concepts of IOT, programming and electric circuits. 

# Material
Component | Purpose of | Bought at | Price 
-| -| -| -
ESP32 DEVKIT V1 DOIT | A microcontroller used for data collection, data transfer, and data analysis. | sizable.se | 94 sek 
Capacitive soil moisture sensor | A sensor that mesures the humidity in soil  | amazon.se | 72 sek
DHT11 | A sensor used for mesuring the humidity in the air asweel as the temperature | sizable.se | 28 sek
40x jumper wires Male/Male | Wires for connecting the different components | sizable.se | 24 sek
Breadboard 400 points | A board used to easy connect the sensors without soldering | sizable.se | 34 sek
Micro USB cable*

\* USB cable was already in possession.

![material](https://user-images.githubusercontent.com/108582271/177007654-dc706edb-46cb-425e-a652-3f34982a3d64.jpg)
Figure 1: Components used for the project

All components are shown in Figure 1. From left to right:
* Breadboard
* ESP32 DEVKIT V1 DOIT
* Capacitive soil moisture sensor. With its wires beneath it
* DHT11
* Jumper wires

# Computer setup
**Setting up the IDE:**

The first step to set up the ESP32 is by first connecting the device to a computer and then downloading the the latest hardware driver available. The link i used https://www.silabs.com/documents/public/software/CP210x_Windows_Drivers.zip

The next step is to update the firmware of the ESP32. This is done by donwloading this file https://micropython.org/resources/firmware/esp32-20220117-v1.18.bin to your computer and on this website https://nabucasa.github.io/esp-web-flasher/ connect your development board by choosing 460800 Baud in the top right corner and pressing connect. You then press erase to delete the files on the board so that you then can enter aoffset of 100 and in "choose file" select the downloaded file and then press "Program".(It takes a while and during this time don’t disconnect your board from the USB cable)

The next thing to do is to download and install Node js (Windows download link https://nodejs.org/en/) and choose a IDE for the project and i choose work in Atom.io (Windows download link https://atom.io/). Then add the Pymakr plugin to Atom by going to File >> Settings >> Install and Look for Pymakr and Install it. (It takes a while; wait until shows it is successfully installed)

You are now ready to run your code on the development board. In order to uppload the code you need to open the Pymakr plugin that you find at the bottom and go into its setting and choose global settings. Enter the device com port into the Device address (list) field (which can be found in the device manager) and remove the check box selection from Safe-boot before upload. Go back and press Connect device in Pymakr and choose your COM port to connect to your board. Now you just need to press Upload project to device in order to run your code on the board.

# Putting everything together
The setup of the device and the wiring is shown in a circuit diagram in Figure 3. Note that i could not find the exact same microcontroller that i used but it has the same amount of pins on each side, so the diagram shows in what spot you to put the wires. I also could not find the exact soil moisture senor but this one has the exact same wiring.

![Screenshot 2022-07-03 153746](https://user-images.githubusercontent.com/108582271/177042312-ed59c1e5-c0c5-4794-9272-6547b3453352.jpg)

Figure 2: Wiring of the components

The EPS32 is put on in right side of the breadboard so the left side pins of the microcontroller can be used. It is powerd by a micro USB cable either connected to a power supply or to a computer. The Vin pin is connected to the breadboards power supply line to give power to the senors and the GND pin is conneced to the GND line. 

Both sensors have a GND pin that is also connected to the breadboards GND line VCC that is connected to the breadboards power supply line. Then each have one data transfer pin, since the capacitive soil moisture sensor is a analog senor then the data pin is wired to a ADC (Analogue to Digital Converter)  pin in this case GPIO36 and DHT11 is connected to GPIO14.

# Chosen platform
The platform i choose for this project is Adafruit since it is a free cloud service that seemed to fit my project with what it has to offer. It is able to easily visualize your data in real-time online in their dashboard where you can choose if you want to see it in a diagram, as a Gauge or any other form. It also offers the opportunity to analyse the data online in simple ways by going to the feeds.

![Screenshot 2022-07-03 161432](https://user-images.githubusercontent.com/108582271/177043837-c7ffd412-ce5b-40e6-ada5-160e3ae2873e.jpg)
Figure 3: The dashboard page on Adafruit

# The code
The first code part shown in figure 4 has first the necessary libaries that is needed for the project such as machine that is used for connecting to the microcontroller, dht that is used for getting the DHT11 sensor to work, network for connecting the device to wifi and mqtt for sending data over the internet.

Then there is variables that are used later in the code, configuration for network and adrafruit and then Setup of the sensors
![Screenshot 2022-07-04 175535](https://user-images.githubusercontent.com/108582271/177188324-6c395256-11cb-4a19-aa8a-82b2184e3fb5.jpg)
Figure 4: First code snippet

This is the main function of the code that tries to  measures data from the sensors, prints the values and then tries to send that data to adafruit and throws exceptions if anything goes wrong.
![Screenshot 2022-07-04 174224](https://user-images.githubusercontent.com/108582271/177186533-3771a6e4-dc13-4d42-8faf-777e1dd182b1.jpg)
Figure 5: Second code snippet

First we have the function restart_and_reconnect that restart the microcontroller and gets called if a exception gets called in the code that would make the code crash. Then do_connect is a function that connects the device to wife.

The code under that tries to set up a connection between the device and adafruit by using MQTT and calls restart_and_reconnect if something goes wrong. Then lastly we have the infinite loop that calls send_sensor_data() until a exception gets thrown and in that case calls restart_and_reconnect.

![Screenshot 2022-07-04 174313](https://user-images.githubusercontent.com/108582271/177186549-74b6d82a-0870-495f-9138-727c9018c406.jpg)
Figure 6: Third code snippet

# Transmitting the data / connectivity

I decided to send data once every 15 minutes since the purpose of this project is to know when i need to water my plant and since the moisture in the soil does not change that fast then 15 seemed like a good time beetween each measuring. 

Wifi was the wireless protocols used for this project because the micocontroller setup is in my home close to my router and do not need any protocol with longer range because of that. Wifi also has no recurring costs, low latency and less bandwidth restrictions so it seemed like to best options.

As for the transport protocols MQTT and webhooks is used in this project. MQTT is used for sending the data measured by the sensors to adafruit. It was choosen because it is a lightweight, energy-efficient and easy to use Transport protocol. Webhooks is also used for when the moisture level in the soil reaches under 30 percent to send a message to my discord that it is time to water my plant.

# Presenting the data
The dashboard on adafruit is set up by 3 feeds of data one for air moisture, one for soil moisture and one for temperature. Each of these 3 feed have 1 diagram showing how the value of each sensor has changed over the past 7 days. For the two moisture feeds a gauge was chosen to display att what percentage of moisture the sensors was at the last reading and for the temperature a text field shows what temeprature it was at the last reading.
![Screenshot 2022-07-03 191415](https://user-images.githubusercontent.com/108582271/177193430-7dc68de6-9abf-402a-b8de-eedfad6e255d.jpg)
Figure 7: The dashboard page on Adafruit

You can also go in to each feed where you can see a more detaild diagram over the data and get each value in a table where you can see the exact value and at what time it was sent.

![Screenshot 2022-07-03 191349](https://user-images.githubusercontent.com/108582271/177193444-15fbc08c-1af1-4815-84bb-8d8460aab52c.jpg)
Figure 8: The air moisture feed page on Adafruit

# Finalizing the design
After assembling the project this is result.
![20220704_204437](https://user-images.githubusercontent.com/108582271/177205477-455ef7d7-c685-43dc-b70d-454be5df1975.jpg)
Figure 8: Finalized project
![20220704_204443](https://user-images.githubusercontent.com/108582271/177205443-8add9629-6cae-42b0-b9d9-350867fc480f.jpg)
Figure 9: Capacitive soil moisture sensor
![20220704_204429](https://user-images.githubusercontent.com/108582271/177205457-6fb6f156-da45-4cb4-b2da-a7c26689adff.jpg)
Figure 10: Finalized project

**Final Thoughts**

I would say that the outcome of the project is satisfactory since i was able to accomplish what i had set out to do. Having prior experience with programming I found that the coding part of the project not that challenging except for getting the Capacitive soil moisture sensor to messure correct data which took quite some googeling to find a answer to and other than that everything whent on without any big struggels since the course had a lot of good turorials to follow.

