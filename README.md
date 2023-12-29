# Project Smart Door Berbasis IoT 

**Name:**
1. FERIKHO FATIH AZHAR (1103202190)
2. MOHAMAD AQWAM FARID (1103201263)
3. KRESHNA PUTRA S.    (1103202215)
4. NURDIN              (1103204006)
5. ADAM AL-AZIZ OLII   (1103204045)

# Ringkasan Project
Project ini bertujuan untuk merancang sistem smart door atau pintu cerdas berbasis IoT. Sistem ini nantinya akan dapat mendeteksi apabila terjadi usaha pembobolan pintu yang tidak diinginkan, lalu mengirim sinyal yang dapat memicu alarm untuk berbunyi.

**Perkiraan waktu pengerjaan yang dibutuhkan**

Sekitar 3 jam


# Pendahuluan
**Latar Belakang:**
Kemajuan teknologi mendorong peningkatkan kualitas serta efektivitas segala aspek kehidupan manusia. Keamanan adalah salah satu aspek yang perlu untuk diperhatikan dan senantiasa diperbarui kualitasnya. Dengan sistem smart door yang kami buat, proses monitoring keamanan pintu dapat dilakukan dengan lebih mudah dan efisien serta lebih optimal dalam penggunaan sumber dayanya.

**Tujuan:**
Dapat memahami konsep IoT sederhana serta menerapkannya dengan cara merancang suatu alat, yakni Sistem Smart Door Berbasis IoT.

**Manfaat:**
Dapat memahami dasar-dasar dalam perancangan alat IoT meliputi pembuatan kodingan, rangkaian elektronik, jaringan, serta database. 

# Alat-alat
Komponen | Fungsi | Jumlah | Harga Satuan
-| -| -| -
ESP8266 NodeMCU V3 | Mengumpulkan data dari sensor, melakukan transfer data, menggerakkan aktuator | 2 | Rp 55.000
Buzzer | Digunakan untuk alarm ketika pintu terbuka | 1 | Rp 1.800
MC-38 Sensor | Sensor magnetik yang dapat mengirimkan sinyal biner 0 atau 1 | 1 | Rp 9.000
Solenoid Lock | Sebagai aktuator untuk mengunci pintu | 1 | Rp 43.000
Relay 5V | Sebagai pengendali arus listrik rangkaian | 1 | Rp 5.000
Baterai 9V | Sebagai penyuplai daya pada rangkaian | 4 | Rp 10.000

![material](https://github.com/sweet-nightmare/IoT-Based-Smart-Door-Project/blob/main/AlatAlat.jpg)
Gambar 1 : Alat-alat yang digunakan


# Setup
**Setup IDE:**

Download dan Install PlatformIO pada VSCode untuk menyusun kode program.

Download dan install library yang diperlukan untuk project, antara lain :
1. PubSubClient
2. ESPNow
3. TaskScheduler

# Desain Rangkaian
![Circuit](NodeDesign.png)
Gambar 2 : Desain rangkaian

Sistem terdiri dari dua node. Node pertama sebagai sender dan node kedua sebagai receiver.

Untuk node pertama, kabel positif sensor MC-38 terhubung ke pin D2 node MCU dan kabel negatif ke pin ground. Lalu, pin positif buzzer terhubung ke pin D1 dan kabel negatif ke pin ground node MCU.

Untuk node kedua, pin D1 pada relay dihubungkan dengan pin D1 node MCU, pin 5V terhubung ke pin Vin node MCU, dan pin ground relay terhubung ke pin ground node MCU. Lalu, untuk kabel positif solenoid lock terhubung ke pin COM relay, dan kabel negatif solenoid lock terhubung ke kabel negatif power supply. Sedangkan kabel positif power supply terhubung ke pin NO pada relay.

# Platform yang digunakan
Platform yang digunakan sebagai antarmuka atau dashboard adalah Node-RED. Node-RED adalah alat pemrograman visual berbasis web yang dibangun di atas Node.js. Node-RED dapat digunakan untuk membuat desain antarmuka grafis berbasis browser yang memungkinkan pengguna untuk membuat alur kerja (flow-based programming) dengan menghubungkan node-node yang mewakili fungsi atau layanan berbeda. 

![Node-RED](https://github.com/sweet-nightmare/IoT-Based-Smart-Door-Project/blob/main/NodeRed.png)
Gambar 3 : Node-RED Flow

![Node-REDUI](https://github.com/sweet-nightmare/IoT-Based-Smart-Door-Project/blob/main/NodeRedUI.png)
Gambar 4 : Dashboard Node-RED

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

