/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 ***********************************
 * NTP time client example
 * A microcontroller board and MOD WIFI ESP8266
 * https://os.mbed.com/docs/mbed-os/v6.15/apis/wi-fi.html
 * https://os.mbed.com/teams/ESP8266/code/esp8266-driver/
 * https://www.olimex.com/Products/IoT/ESP8266/MOD-WIFI-ESP8266/open-source-hardware
 * https://os.mbed.com/teams/ESP8266/code/esp8266-driver/docs/tip/classESP8266Interface.html
 *
 * L432KC --- MOD WIFI ESP8266 from OLIMEX
 * L432KC D5=PB6=UART1TX --- 3 RXD
 * L432KC D4=PB7=UART1RX --- 4 TXD
 * or
 * L432KC D1=PA9=UART1TX --- 3 RXD
 * L432KC D0=PA10=UART1RX --- 4 TXD
 * L432KC 3V3 --- 1 3.3V
 * L432KC GND --- 2 GND
 *  
 * Timo Karppinen 12.12.2021  Apache-2.0
 ***********************************/
#include "mbed.h"

// MOD WIFI ESP8266
#include "ESP8266Interface.h"   // included in the OS6

#include "ntp-client/NTPClient.h"



ESP8266Interface esp(MBED_CONF_APP_ESP_TX_PIN, MBED_CONF_APP_ESP_RX_PIN);



// WLAN security
const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}

void scan_demo(WiFiInterface *wifi)
{
    WiFiAccessPoint *ap;

    printf("Scan:\r\n");

    int count = wifi->scan(NULL, 0);

    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;

    ap = new WiFiAccessPoint[count];

    count = wifi->scan(ap, count);
    for (int i = 0; i < count; i++) {
        printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n", ap[i].get_ssid(),
               sec2str(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
    }
    printf("%d networks available.\r\n", count);

    delete[] ap;
}

//NTP server is a time server used for delivering timing information for networks.
// Returns 32 bits for seconds and 32 bits for fraction of seconds. 
//#define ntpAddress "2.pool.ntp.org"
#define ntpAddress "time.mikes.fi"  // The VTT Mikes in Helsinki
#define ntpPort 123     // Typically 123 for every NTP server
    
int main() {
    
// Setting up WLAN
 
    printf("WiFi example\r\n\r\n");
    
    //Store device IP
    SocketAddress deviceIP;
    ThisThread::sleep_for(500ms); // waiting for the ESP8266 to wake up.
      
    scan_demo(&esp);

    printf("\r\nConnecting...\r\n");
     int ret = esp.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\r\nConnection error\r\n");
        return -1;
    }

    
    printf("Success\n\n");
    printf("MAC: %s\n", esp.get_mac_address());
    esp.get_ip_address(&deviceIP);
    printf("IP: %s\n", deviceIP.get_ip_address());
    printf("Netmask: %s\n", esp.get_netmask());
    printf("Gateway: %s\n", esp.get_gateway());
    printf("RSSI: %d\n\n", esp.get_rssi());
 
// --- WLAN
    
// NTP demo    
    printf("\nNTP Client example (using WLAN)\r\n");
    
    
    NTPClient ntp(&esp);
    
    ntp.set_server(ntpAddress, ntpPort);
      
    for(int i=0; i<5; i++){
        time_t timestamp = ntp.get_timestamp();
        
        if (timestamp < 0) {
            printf("An error occurred when getting the time. Code: %u\r\n", timestamp);
        } else {
            printf("The timestamp seconds from the NTP server in\r\n  32 bit hexadecimal number is %X\r\n", timestamp);
            printf("  decimal number is %u\r\n", timestamp);
            timestamp += (60*60*3);  //  GMT +3  for Finland for the summer time.
            printf("Current time is %s\r\n", ctime(&timestamp));
        }
        
        printf("Waiting for 10 seconds before trying again...\r\n");
        ThisThread::sleep_for(10s);  
    }  
    printf("Did it succeed to get correct time?\n If not the port might be blocket on the firewall.\n");
    ThisThread::sleep_for(30s);  

    while(1) {  
        printf("\nWe stopped sending more UDP packets to the server.\nUnplug your device!\n");
        ThisThread::sleep_for(3s);
    }
// --- NTP demo
}