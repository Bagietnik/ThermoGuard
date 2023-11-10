# http_server
http_server build with the esp32 board

**How to run this code?**

1. Install ESP-IDF according to https://www.youtube.com/watch?v=5IuZ-E8Tmhg
2. Clone the repository to the local environment
3. Open terminal and write following commands
4. idf.py build
5. idf.py -p COM[number of your port] flash monitor

**The summary**

- Hardware
  - mcp9808 temperature sensor (i2c)
  - ESP32 DevKit (http server)
  - Raspberry Pi 4 (mqtt broker)
  - Radiator Thermostat (zigbe2mqtt)

- Software
  
Sending an HTTP request to http://localhost will display a simple webpage where the current temperature value is presented

![Zrzut ekranu 2023-11-06 193119](https://github.com/Bagietnik/http_server/assets/84154206/03cf4b28-9f49-4410-8350-39e0faac223a)


An HTTP request to http://localhost/json will receive a JSON-formatted message with extended sensor data, including alarm state and measurement resolution

![Zrzut ekranu 2023-11-06 193505](https://github.com/Bagietnik/http_server/assets/84154206/1939c50c-c9e6-499f-8e09-91e1b271153b)

The system sets target temperature on the thermostat by manipulating the data received from the components.
The selected setpoint depends on various factors, including the time of day and the user's presence

![image](https://github.com/Bagietnik/http_server/assets/84154206/d95a3766-4091-44d4-b1c0-addedcf4f9b3)


The temperature values are loged in the data base (InfluxDB). Grafana enables convenient visualization.

![image](https://github.com/Bagietnik/http_server/assets/84154206/a42dd988-a8da-415b-932d-d18140273513)


