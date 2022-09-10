#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Servo.h>

int SoilMoistureSensorPin = A0;
int PIR_pin = 7;
int buzzer = 6;
int buzzer2 = 32;
Servo servo , servo2;
int servo_pos = 0 , servo2_pos=0;
int ldr = A14;
int ledpin = 4;
int flamePIN=40;
int dcmotorforward=34;
int dcmotorbackward=35;
int trigpin=24;
int echopin=25;
long duration;
int distance;
int tankmotorforward=45;
int tankmotorbackward=46;
int pumpmotorforward=27;
int pumpmotorbackward=28;

int rs = 13, en = 12 , d4 = 11 , d5 = 10 , d6 = 9 , d7 = 8;


LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
LiquidCrystal lcd2(rs,en,A4,A5,A6,A7);

void setup() {
  
  lcd.begin(16,4);
  lcd.setCursor(0,0);
  lcd2.begin(16,4);
  lcd2.setCursor(0,0);
  Serial1.begin(9600);
  pinMode(PIR_pin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(flamePIN,INPUT);
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(dcmotorforward, OUTPUT);
  pinMode(dcmotorbackward, OUTPUT);
  pinMode(tankmotorforward, OUTPUT);
  pinMode(tankmotorbackward, OUTPUT);
   pinMode(pumpmotorforward, OUTPUT);
  pinMode(pumpmotorbackward, OUTPUT);
  servo.attach(5);
  servo2.attach(3);
  delay(100);

}

void SendMessage(int SensorValue)
{
  
  PIRsensor();
  LDR();
  FlameSensor();
  ultrasonic();
  Serial1.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);  
  Serial1.println("AT+CMGS=\"+8801876862480\"\r"); 
  delay(10);

  if(SensorValue < 500)
  {
      Serial1.println("Soil is too dry.Start watering now.");
      lcd.setCursor(0,2);
      lcd.print(String(SensorValue));
      lcd.setCursor(0,3);
      lcd.print("Soil is too dry");
      digitalWrite(pumpmotorforward,HIGH);
      digitalWrite(pumpmotorbackward,LOW);
      delay(100);
  }
  else if(SensorValue > 700)
  {
     Serial1.println("Soil is too wet.You need to stop watering "); 
      lcd.setCursor(0,2);
      lcd.print(String(SensorValue));
      lcd.setCursor(0,3);
      lcd.print("Soil is too wet");
      digitalWrite(pumpmotorforward,LOW);
      digitalWrite(pumpmotorbackward,LOW);
      delay(100);
  }
  
  delay(10);
  
}

void PIRsensor()
{
  if(digitalRead(PIR_pin) == HIGH)
  {
     //Serial1.println("Motion detected.");
     tone(buzzer,100);

     for(servo_pos=0;servo_pos<=180;servo_pos+=5)
     {
        servo.write(servo_pos);
        //delay(100);
     }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Motion Detected");
        lcd.setCursor(0,1);
        lcd.print("Trap Activated");
     
     delay(10);
  }
  if(digitalRead(PIR_pin) == LOW)
  {
     //Serial1.println("Motion ended.");
     noTone(buzzer);
     
     for(servo_pos=180;servo_pos>=0;servo_pos-=5)
     {
        servo.write(servo_pos);
        
        //delay(100);
     }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("No motion found");
        lcd.setCursor(0,1);
        lcd.print("Trap is inactive");
     
     delay(10);
  }
  
}

void LDR()
{
  int value= analogRead(ldr);
  Serial1.println("Intensity of the LDR is =");
  Serial1.println(value);
  delay(10);
  if(value<=300)
  {
    digitalWrite(ledpin,HIGH);
    for(servo2_pos=0;servo2_pos<=180;servo2_pos+=5)
     {
        servo2.write(servo2_pos);
        //delay(100);
     }
     delay(10);
  }
  else
  {
    digitalWrite(ledpin,LOW);
    for(servo2_pos=180;servo2_pos>=0;servo2_pos-=5)
     {
        servo2.write(servo2_pos);
        //delay(100);
     }
     delay(10);
  }
}

void FlameSensor()
{
  if(digitalRead(flamePIN)==HIGH)
  {
    digitalWrite(dcmotorforward,HIGH);
    digitalWrite(dcmotorbackward,LOW);
    tone(buzzer2,1000);
    delay(10);
  }
  else
  {
    digitalWrite(dcmotorforward,LOW);
    digitalWrite(dcmotorbackward,LOW);
    noTone(buzzer2);
    delay(10);
  }
}

void ultrasonic()
{
  digitalWrite(trigpin,LOW);
  delay(2);

  digitalWrite(trigpin,HIGH);
  delay(10);
  digitalWrite(trigpin,LOW);
  
  duration = pulseIn(echopin,HIGH);
  distance = (duration*0.034)/2;

  Serial1.println(distance);

  if(distance<300)
  {
    digitalWrite(tankmotorforward,LOW);
    digitalWrite(tankmotorbackward,LOW);
    lcd2.clear();
    lcd2.setCursor(0,0);
    lcd2.print("Water level");
    lcd2.setCursor(0,1);
    lcd2.print("in tank(cm):");
    lcd2.setCursor(0,2);
    lcd2.print(String(distance));
    lcd2.setCursor(0,3);
    lcd2.print("Motor Off");
    delay(10);
  }
  else
  { 
    digitalWrite(tankmotorforward,HIGH);
    digitalWrite(tankmotorbackward,LOW);
    lcd2.clear();
    lcd2.setCursor(0,0);
    lcd2.print("Water level");
    lcd2.setCursor(0,1);
    lcd2.print("in tank(cm):");
    lcd2.setCursor(0,2);
    lcd2.print(String(distance));
    lcd2.setCursor(0,3);
    lcd2.print("Motor on");
    delay(10);
  }
  
  
}

void loop() {
 
  
  PIRsensor();
  LDR();
  FlameSensor();
  ultrasonic();
  delay(10);
  
  int SensorValue = analogRead(SoilMoistureSensorPin);
  PIRsensor();

  if(SensorValue < 500)
  {
      PIRsensor();
      LDR();
      FlameSensor();
      ultrasonic();
      //lcd.clear();
      lcd.setCursor(0,2);
      lcd.print(String(SensorValue));
      lcd.setCursor(0,3);
      lcd.print("Soil is too dry");
      delay(10);
  }
  else if(SensorValue>=500 && SensorValue<=700)
  {
      PIRsensor();
      LDR();
      FlameSensor();
      ultrasonic();
      //lcd.clear();
      lcd.setCursor(0,2);
      lcd.print(String(SensorValue));
      lcd.setCursor(0,3);
      lcd.print("Soil is good");
      delay(10);
  }
  else if(SensorValue > 700)
  { 
      PIRsensor();
      LDR();
      FlameSensor();
      ultrasonic();
      //lcd.clear();
      lcd.setCursor(0,2);
      lcd.print(String(SensorValue));
      lcd.setCursor(0,3);
      lcd.print("Soil is too wet");
      delay(10);
  }
  
  if(SensorValue<500 || SensorValue>700)
  {
      PIRsensor();
      LDR();
      FlameSensor();
      ultrasonic();
     SendMessage(SensorValue);
  }

}
