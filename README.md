
Exercises for communications. Versions for 
NXP FRDM K64F with not external hardware,
ST NUCLEO L476RG with IDW04A1 WLAN module,
ST NUCLEO L432KC with MOD WIFI ESP8266.
UDP message is used for getting a timestamp
from a NTP server. 
Ethernet: https://os.mbed.com/docs/mbed-os/v6.2/apis/ethernet.html
WLAN WiFI: https://os.mbed.com/docs/mbed-os/v6.2/apis/wi-fi.html
A UDP Socket: UDPSocket.open(), UDPSocket.sendto(), UDPSocket.recvfrom()
Based on the OS6.2 API reference UDP Socket example
https://os.mbed.com/teams/mbed_example/code/mbed-os-example-udp-sockets//file/cf516d904427/main.cpp/