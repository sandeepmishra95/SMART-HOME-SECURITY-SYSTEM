/* Client IP is 192.168.10.1 and is listening on UDP port 3333*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <Ticker.h>

char self_ssid[] = "ESP_SERVER";
char pwd[] = "***********";

IPAddress ip(192, 168, 0, 1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress client0_ip (192,168,0,10);
IPAddress client1_ip (192,168,0,11);


ESP8266WebServer server(80); //Web server listening on Port 80

Ticker client0;
Ticker client1;
Ticker UART;

//Creating UDP Listener Object. 
WiFiUDP UDP; //listener
unsigned int UDPPort = 2222;

//Packet Buffer 
const int packetSize = 8;
byte packetBuffer[packetSize]; 

int sensor0_arm = HIGH;
int sensor1_arm = HIGH;
int sensor0 = HIGH;
int sensor1 = HIGH;

//flags
int flag0=0;
int flag1=0;
int uart_flag=0;


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //configuring for AP access
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip,gateway,subnet);
  WiFi.softAP(self_ssid,pwd);


  //Initialising Server
  server.begin();
  server.on("/status", handleRoot);
  server.onNotFound(handleNotFound);

  //Initialising UDP
  UDP.begin(UDPPort);

  //Initialising Ticker object
  client0.attach(1,check_client0);
  delay(500);
  client1.attach(1,check_client1);
  UART.attach(1,send_to_UART);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  server.handleClient();
  handleUDPServer() ;
  if(Serial.available())
    handleSerial();

  if(flag0==1)
  {
    WiFiUDP sendUDP;
    sendUDP.beginPacket(client0_ip,3333);
    sendUDP.write("SEND");
    sendUDP.endPacket();
    flag0=0;
  }
  
  if(flag1==1)
  {
    WiFiUDP sendUDP;
    sendUDP.beginPacket(client1_ip,3333);
    sendUDP.write("SEND");
    sendUDP.endPacket();
    flag1=0;
  }

  if(uart_flag==1)
  {
    if(sensor0==HIGH && sensor1==HIGH)
    Serial.write(0);
  else if(sensor0==LOW && sensor1==HIGH)
    Serial.write(1);
  else if(sensor0==HIGH && sensor1==LOW)
    Serial.write(2);
  else 
    Serial.write(3);
    uart_flag=0;
  }
  

}

void handleRoot()
{
  int var;
  String base = "<!DOCTYPE html>";
  String start = "<html><body>";
  String html_end = "</body></html>";
  String title_secure = "<h1><marquee>The System is Secure!</marquee></h1>";
  String title_off = "<h1><marquee>The System is off</marquee></h1>";
  String title_alert = "<h1><marquee>ALERT!</marquee></h1>";
  String body1 = "<p> All sensors are armed. </p>";
  String body2 = "<p> Only Sensor1 is armed. </p>";
  String body3 = "<p> Only Sensor0 is armed. </p>";
  String body4 = "<p> All sensors are disarmed. </p>";
  String body5 = "<p> All sensors are off. </p>";
  String body6 = "<p> Sensor0 is off. </p>";
  String body7 = "<p> Sensor1 is off. </p>";

  if(sensor0_arm==HIGH && sensor1_arm==HIGH)
  {
    if(sensor0==HIGH && sensor1==HIGH)
      var=0;
    else if(sensor0==LOW && sensor1==HIGH)
      var=5;
    else if(sensor0==HIGH && sensor1==LOW)
      var=6;
     else 
     var=4;
  }
  else if(sensor0_arm==LOW && sensor1_arm==HIGH)
  {
    if(sensor1==HIGH)
      var=2;
    else
      var=6;
  }
   else if(sensor0_arm==HIGH && sensor1_arm==LOW)
  {
    if(sensor0==HIGH)
      var=1;
    else
      var=5;
  }
  else
    var = 3;

  switch(var)
  {
    case 0:
      server.send(200, "text/html", base+start+title_secure+body1+html_end);
      break;
    case 1:
      server.send(200, "text/html", base+start+title_secure+body2+html_end);
      break;
    case 2:
      server.send(200, "text/html", base+start+title_secure+body3+html_end);
      break;
    case 3:
      server.send(200, "text/html", base+start+title_off+body4+html_end);
      break;
    case 4:
      server.send(200, "text/html", base+start+title_alert+body5+html_end);
      break;
    case 5:
      server.send(200, "text/html", base+start+title_alert+body6+html_end);
      break;
    case 6:
      server.send(200, "text/html", base+start+title_alert+body7+html_end);
      break;
  }


  
  server.send(200, "text/html", "<!DOCTYPE html><html><body><h1> Alert! </h1><p> All sensors are off. </p></body></html>");
  delay(1);
}

void handleUDPServer() 
{
  int cb = UDP.parsePacket();
  if (cb) 
  {
    UDP.read(packetBuffer, packetSize);

    String myData = "";
    for(int i = 0; i < packetSize; i++) {
      myData += (char)packetBuffer[i];
    }

    if(myData == "S0_HIGH")
      sensor0 = HIGH;
   
    else if(myData == "S0_LOW ")
      sensor0 = LOW;

    else if(myData == "S1_HIGH")
      sensor1 = HIGH;

    else if(myData == "S1_LOW ")
      sensor1 = LOW;

    //clear the packet  
    for(int i=0;i<packetSize;i++)
    {
      packetBuffer[i] = 0;
    }

  }
}

void handleSerial ()
{
  char msg;
  msg = Serial.read();

  if(msg==0)
  {
    sensor0_arm=HIGH;
    sensor1_arm=HIGH;
  }
  
  else if(msg==1)
  {
    sensor0_arm=LOW;
    sensor1_arm=HIGH;
  }
  
  else if(msg==2)
  {
    sensor0_arm=HIGH;
    sensor1_arm=LOW;
  }
  
  else if(msg==3)
  {
    sensor0_arm=LOW;
    sensor1_arm=LOW;
  }
}


void check_client0()
{
//  WiFiUDP sendUDP;
//  
//  sendUDP.beginPacket(client0_ip,3333);
//  sendUDP.write("SEND");
//  sendUDP.endPacket();
//  
//  delay(1);
  flag0=1;
}

void check_client1()
{
//  WiFiUDP sendUDP;
//  
//  sendUDP.beginPacket(client1_ip,3333);
//  sendUDP.write("SEND");
//  sendUDP.endPacket();
//  
  //delay(1);
  flag1=1;
}

void send_to_UART()
{
//  if(sensor0==HIGH && sensor1==HIGH)
//    Serial.write(0);
//  else if(sensor0==LOW && sensor1==HIGH)
//    Serial.write(1);
//  else if(sensor0==HIGH && sensor1==LOW)
//    Serial.write(2);
//  else 
//    Serial.write(3);

  uart_flag=1;
}

void handleNotFound()
{
  server.send(200, "text/html", "<!DOCTYPE html><html><body><h1>Operation not Allowed!</h1><a href=\"http://192.168.0.1/status\">Visit here to check status of sensors!</a></body></html>");
}


