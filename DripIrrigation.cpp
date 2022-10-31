// C++ code
//
int sensorvalue1;
float sensorlevel1;
int sensorvalue2;
float sensorlevel2;
int sensorvalue3;
float sensorlevel3;
void setup()
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

Serial.begin(9600);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void loop()
{
  sensorvalue1 = analogRead(A0);
  sensorlevel1 = (sensorvalue1/539.00)*100;  
  if(sensorlevel1 <= 50.00){ 
  
  digitalWrite(7, HIGH);
  Serial.print("Solenoid Valve 1 Open \n");
  }else{ 
  digitalWrite(7, LOW);
  Serial.print("Solenoid Valve 1 close \n");
  }
  
  sensorvalue2 = analogRead(A1);
  sensorlevel2 = (sensorvalue2/539.00)*100;  
  if(sensorlevel2 <= 50.00){
    
  digitalWrite(6, HIGH);
  Serial.print("Solenoid Valve 2 Open \n");
  }else{  
    digitalWrite(6, LOW);
  Serial.print("Solenoid Valve 2 close \n");
  }
 
  sensorvalue3 = analogRead(A2);
  sensorlevel3 = (sensorvalue3/539.00)*100;  
  if(sensorlevel3 <= 50.00){
    
  digitalWrite(5, HIGH);
  Serial.print("Solenoid Valve 3 Open \n");
  }else{  
    digitalWrite(5, LOW);
  Serial.print("Solenoid Valve 3 close \n");
  }
  delay(1000);
}