# SMART-HOME-SECURITY-SYSTEM
A SMART HOME SECURITY SYSTEM BASED ON ARM CORTEX M4 MICROPROCESSOR

The project uses a network of sensors connected through ESP 8266 or (ESP-01) to detect intrusion on any particular node of entry. The sensors are made of Hall-effect IC's which detect change in magnetic field and transmit this data using ESP-01 to a central ESP-01 acting as a Soft-AP. 
The central ESP-01 also hosts a webserver where the end-user can monitor the security status in real time.

The central microcontroller, TM4C123 works in conjunction with the ESP network. It receives physical inputs from the user and displays the current status on a Nokia 5110 LCD screen. User is presented with an interactive menu on LCD screen which can be used with 3 buttons provided so as to toggle the options and manually override any sensor on the network.

The TM4C123 communicates with central ESP-01 through UART protocol.
The ESP-01's share data on the network using UDP protocol. 


To get a detailed prespective on the project, please follow this link.
https://drive.google.com/open?id=0B_v-e4tCEtdUS0hvVmYtYUF1Z1U
