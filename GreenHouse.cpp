#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
int sensorvalue1;
float sensorlevel1;

int buzzer = 4;
int smokesensor = A1;
int analogValue=0;
int valve1;
const int temperatureIn = A4;
int humiditysensorOutput = 0;
int RawValue= 0;
double Voltage = 0;
double tempC = 0;
double tempF = 0;
int sensorThresh = 400;
String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; // Our virtual wifi has no password 
String host     = "api.thingspeak.com"; // Open ThingsSpeak API
const int httpPort   = 80;
String uri = "/update?api_key=4LZXHAB9N3AKV47R";
String kit_id="be938215-a2c2-4b66-b994-40518a55368e";

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
   RawValue = analogRead(temperatureIn);
  Voltage = (RawValue / 1023.0) * 5000; // 5000 to get millivots.
  tempC = (Voltage-500) * 0.1; // 500 is the offset
  tempF = (tempC * 1.8) + 32; // convert to F  
  Serial.print("\nTemperature in C = ");
  Serial.print(tempC,1);
  Serial.print("\nTemperature in F = ");
  Serial.println(tempF,1);
  humiditysensorOutput = analogRead(A3);
  Serial.print("Humidity: "); // Printing out Humidity Percentage
  Serial.print(map(humiditysensorOutput, 0, 1023, 10, 70));
  Serial.println("%");
  sensorvalue1 = analogRead(A0);
  sensorlevel1 = (sensorvalue1/876.00)*100;
 
  Serial.print(sensorlevel1);  
  Serial.print("\n");
  
    if(sensorlevel1 <= 50.00){ 
  
  digitalWrite(7, HIGH);
      valve1 =1;
  Serial.print("Solenoid Valve 1 Open \n");
  }else{ 
  digitalWrite(7, LOW);
      valve1 = 0;
  Serial.print("Solenoid Valve 1 close \n");
  }
  analogValue = analogRead(smokesensor);
  Serial.print(analogValue);
  if(analogValue>sensorThresh)
  {
    tone(buzzer,1000,200);
    delay(100);
  }
  else
  {
    noTone(buzzer);
    delay(100);
  }
  
  
  
  
  // Construct our HTTP call
  String httpPacket = "GET " + uri+"&field1=" + String(analogValue) +"&field2=" + String((Voltage-500) * 0.1)+"&field3=" + String(map(humiditysensorOutput, 0, 1023, 10, 70))+"&field4=" + String(sensorlevel1)+"&field5=" + String(valve1)+"&field6=" + String(0)+"&field7=" + String(kit_id)+ " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
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
  pinMode(A3, INPUT);
	pinMode(buzzer,OUTPUT);
	pinMode(smokesensor,INPUT);
}

void loop() {
  
 anydata();
  
  
  
  delay(100);
}
