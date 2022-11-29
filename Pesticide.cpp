#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
int sensorvalue1;
float sensorlevel1;

int sensorvalue3;
float sensorlevel3;
int valve1;

String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; // Our virtual wifi has no password 
String host     = "api.thingspeak.com"; // Open Weather Map API
const int httpPort   = 80;
String uri = "/update?api_key=IJIR86HOIWBA1TXV";
String kit_id="fcde29c6-ef08-48b5-8e7b-b9f8a618196b";

int setupESP8266(void) {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 1;
    
  // Connect to 123D Circuits Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2;
  
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 3;
  
  return 0;
}

void anydata(void) {
  
  sensorvalue1 = analogRead(A0);
  sensorlevel1 = (sensorvalue1/876.00)*100;
 
  sensorvalue3 = analogRead(A2);
  sensorlevel3 = (sensorvalue3/876.00)*100;
  Serial.print(sensorlevel1);  
  Serial.print("\n");
  Serial.print(sensorlevel3);
  Serial.print("\n");
  float avgSensor= (sensorlevel1+sensorlevel3)/2;
  Serial.println(avgSensor);
    if(avgSensor <= 50.00){ 
  
  digitalWrite(7, HIGH);
  Serial.print("Solenoid Valve 1 Open \n");
  }else{ 
  digitalWrite(7, LOW);
      valve1 = 0;
  Serial.print("Solenoid Valve 1 close \n");
  }
  
  
  
  
  
  // Construct our HTTP call
  String httpPacket = "GET " + uri+"&field1=" + String(sensorlevel1) +"&field2=" + String(sensorlevel3)+"&field3=" + String(valve1)+"&field4=" + String(kit_id)+ " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

  // Send our http request
  Serial.print(httpPacket);
  // Wait a little for the ESP to respond
  if (!Serial.find("SEND OK\r\n")) {};
  
}

void setup() {
  
  setupESP8266();
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void loop() {
  
 anydata();
  
  
  
  delay(100);
}
