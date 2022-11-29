#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
int sensorvalue1;
float sensorlevel1;
int sensorvalue2;
float sensorlevel2;
int sensorvalue3;
float sensorlevel3;
int valve1;
int valve2;
int valve3;

String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; // Our virtual wifi has no password 
String host     = "api.thingspeak.com"; // Open Weather Map API
const int httpPort   = 80;
String uri = "/update?api_key=FQC1P9MP7AEDIYXV";
String kit_id="23ee94ed-7a37-4320-8461-377eff04ea01";

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
  sensorvalue2 = analogRead(A1);
  sensorlevel2 = (sensorvalue2/876.00)*100; 
  sensorvalue3 = analogRead(A2);
  sensorlevel3 = (sensorvalue3/876.00)*100;
  Serial.print(sensorlevel1);  
  Serial.print("\n");
  Serial.print(sensorlevel2);
  Serial.print("\n");
  Serial.print(sensorlevel3);
  Serial.print("\n");
  
    if(sensorlevel1 <= 50.00){ 
  
  digitalWrite(7, HIGH);
      valve1 = 1;
  Serial.print("Solenoid Valve 1 Open \n");
  }else{ 
  digitalWrite(7, LOW);
      valve1 = 0;
  Serial.print("Solenoid Valve 1 close \n");
  }
   if(sensorlevel2 <= 50.00){
    
  digitalWrite(6, HIGH);
     valve2 = 1;
  Serial.print("Solenoid Valve 2 Open \n");
  }else{  
    digitalWrite(6, LOW);
     valve2 = 0;
  Serial.print("Solenoid Valve 2 close \n");
  }
  if(sensorlevel3 <= 50.00){
    
  digitalWrite(5, HIGH);valve3=1;
  Serial.print("Solenoid Valve 3 Open \n");
  }else{  
    digitalWrite(5, LOW);
    valve3=0;
  Serial.print("Solenoid Valve 3 close \n");
  }
  
  
  
  // Construct our HTTP call
  String httpPacket = "GET " + uri+"&field1=" + String(sensorlevel1) +"&field2=" + String(sensorlevel2)+"&field3=" + String(sensorlevel3)+"&field4=" + String(valve1)+"&field5=" + String(valve2)+"&field6=" + String(valve3)+"&field7=" + String(kit_id) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
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
