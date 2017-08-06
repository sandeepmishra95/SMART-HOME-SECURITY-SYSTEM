/* Server IP is 192.168.0.1 and is listening on UDP port 2222*/
// this is 2nd client esp

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
//#include <Ticker.h>
#define GPIO0 0
#define GPIO2 2

//Webserver object
ESP8266WebServer client1(80); 

//Creating UDP Listener Object. 
WiFiUDP UDP;
unsigned int UDPPort = 3333;

// Ticker object
//Ticker checker;
//Ticker hall_sensor;
int count = 0;


char server_ssid[] = "ESP_SERVER";

char pwd[] = "***********";

IPAddress server_ip(192, 168, 0, 1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress self_ip (192,168,0,11);


//Packet Buffer 
const int packetSize = 8;
byte packetBuffer[packetSize]; 
int led_state = HIGH;
int hall = HIGH;


void setup() 
{
  // put your setup code here, to run once:
  
  //configuring for AP access
  WiFi.mode(WIFI_STA);
  WiFi.config(self_ip,gateway,subnet);
  
  //GPIO 0 Pin as LED
  pinMode(GPIO0,OUTPUT);

  //GPIO 2 Pin as Input from Hall Sensor
  pinMode(GPIO2,INPUT);
  
  while(WiFi.begin(server_ssid,pwd)!= WL_CONNECTED)
  {
    digitalWrite(GPIO0, led_state);
    led_state = (led_state== HIGH)? LOW : HIGH ;
    delay(100);
  }
  
  digitalWrite(GPIO0, LOW);

  //Attach function to ticker object
  //checker.attach(0.5,check_wifi);
  //hall_sensor.attach(0.5,check_sensor);
  
  
  client1.begin(); //Initialising Webserver on client1 necessary for UDP
  //Initialising UDP
  UDP.begin(UDPPort); //Start listening on UDP port
}

void loop() 
{
  int cb = UDP.parsePacket();

  if (cb) 
  {
    UDP.read(packetBuffer, packetSize);

    String myData = "";
    
    for(int i = 0; i < packetSize; i++)
    {
      myData += (char)packetBuffer[i];
    }

    if(myData == "SEND")
      send_data();
  }
  check_sensor();
  check_wifi();
}


void send_data()
{
  char char_buf[8];
  String msg = "";
  if(hall== HIGH)
  {
    msg += "S1_HIGH";
  }
  else
  {
    msg += "S1_LOW ";
  }

  msg.toCharArray(char_buf,8);
  
  WiFiUDP sendUDP;
  sendUDP.beginPacket(server_ip, 2222);
  sendUDP.write(char_buf,8);
  sendUDP.endPacket();
  delay(1);
}

void check_wifi()
{
  if (WiFi.status() != WL_CONNECTED)
    count++;
  if(count > 10)
    ESP.restart();
}

void check_sensor()
{
  if(digitalRead(GPIO2) == HIGH)
    {
      hall = HIGH;
      digitalWrite(GPIO0,LOW);
    }
  else
    {
      hall = LOW;
      digitalWrite(GPIO0,HIGH);
     }
}

