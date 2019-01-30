#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#include <Servo.h>
#define DATA 6 // digital 6 to pin 14 on the 74HC595
#define LATCH 7 // digital 7 to pin 12 on the 74HC595
#define CLOCK 5 // digital 5 to pin 11 on the 74HC595
#define NOTE_DS8 4978
Servo myservo;
int p1;
int p2;
int p3;
int ext;
int ent;
byte x;
int yellowLed = 3;
int piezo = 2;
int trig1 = 13;
int echo1 = 12;
int trig2 = 15;
int echo2 = 14;
int trig3 = 16;
int echo3 = 17;
int trigEnt = 9;
int echoEnt = 8;
int trigExt = 10;
int echoExt = 11;
void setup() {
  //LCD
  lcd.begin(16, 2);
  lcd.clear();
  //ShiftRegister Pins
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  //UltraSonic Pins
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);
  pinMode(trigEnt, OUTPUT);
  pinMode(echoEnt, INPUT);
  pinMode(trigExt, OUTPUT);
  pinMode(echoExt, INPUT);
  //SerialMonitor
  Serial.begin(9600);
  myservo.attach(4);
  //warning LED
  pinMode(3,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  updateShiftRegister();
  myservo.write(0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome DearCust");
  if(p1<20 && p2<20 && p3<20)
  {
  lcd.setCursor(0, 1);
  lcd.print("Parking full!!");
  }
    if(p1>10){
    lcd.setCursor(0, 1);
    lcd.print("P1");
    }
    if(p2>10){
    lcd.setCursor(3, 1);
    lcd.print("P2");
    }
    if(p3>20){
    lcd.setCursor(7, 1);
    lcd.print("P3");
    }
        if(ext<10)
      if(p1<20 && p2<20 && p3<20)
        return;
      else  
      {
      openGate();
      delay(3000);
      closeGate();
      }
    if(ent<10){
      if(p1<20 && p2<20 && p3<20){
          return;
        }
      else
        {
          openGate();
          delay(3000);
          closeGate();
        }
      }    
}
void ReadSensorP1(){
  analogWrite(trig1, LOW);  
  delay(30);
  digitalWrite(trig1, HIGH);  
  delay(30);
  digitalWrite(trig1, LOW);
  p1 = pulseIn(echo1, HIGH) / 29 / 2;
  Serial.println(p1);
  }
void ReadSensorP2(){
  analogWrite(trig2, LOW);  
  delay(30);
  digitalWrite(trig2, HIGH);  
  delay(30);
  digitalWrite(trig2, LOW);
  p2 = pulseIn(echo2, HIGH) / 29 / 2;
  Serial.println(p2);
  }
void ReadSensorP3(){
  analogWrite(trig3, LOW);  
  delay(30);
  digitalWrite(trig3, HIGH);  
  delay(30);
  digitalWrite(trig3, LOW);
  p3 = pulseIn(echo3, HIGH) / 29 / 2;
  Serial.println(p3);
  }
void ReadSensorEnt(){
  analogWrite(trigEnt, LOW);  
  delay(30);
  digitalWrite(trigEnt, HIGH);  
  delay(30);
  digitalWrite(trigEnt, LOW);
  ent = pulseIn(echoEnt, HIGH) / 29 / 2;
  Serial.println(ent);
  }
void ReadSensorExt(){
  analogWrite(trigExt, LOW);  
  delay(30);
  digitalWrite(trigExt, HIGH);  
  delay(30);
  digitalWrite(trigExt, LOW);
  ext = pulseIn(echoExt, HIGH) / 29 / 2;
  Serial.println(ext);
  }
void openGate(){
  updateShiftRegister();
    for(int i=0; i<=70; i++)
      {
        myservo.write(i);
        digitalWrite(yellowLed,HIGH);
        delay(30);
      }
   updateShiftRegister();
    }
void closeGate(){
  updateShiftRegister();
    for(int i=70; i>=0; i--)
      {
        myservo.write(i);
        digitalWrite(yellowLed,HIGH);
        tone(2, NOTE_DS8, 15);
        delay(15);
      }
      digitalWrite(yellowLed,LOW);
      updateShiftRegister();
      noTone(2);
      }
void updateShiftRegister(){
  readAllSensor();
  if(p1<10 && p1!=0)
  {
    x=B01101010;
  }
  if(p2<10 && p2!=0)
  {
    x=B10011010;
  }
  if(p3<10 && p3!=0)
  {
    x=B10100110;
  }
  if(p1<10 && p2<10 && p1!=0 && p2!=0)
  {
    x=B01011010;
    }
  if(p1<10 && p3<10 && p1!=0 && p3!=0)
  {
    x=B01100110;
    }
  if(p2<10 && p3<10 && p2!=0 && p3!=0)
  {
    x=B10010110;
    }
  if(p1<10 && p2<10 && p3<10 && p1!=0 && p2!=0 && p3!=0)
  {
    x=B01010101;
    }
    //if all parking are free
    if(p1>10 && p2>10 && p3>10 && p1!=0 && p2!=0 && p3!=0)
  {
    x=B10101010;
    }
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, x);
  digitalWrite(LATCH, HIGH);
  }
void readAllSensor(){
  Serial.print("p1 :");
  ReadSensorP1();
  Serial.print("p2 :");
  ReadSensorP2();
  Serial.print("p3 :");
  ReadSensorP3();
  Serial.print("Ent :");
  ReadSensorEnt();
  Serial.print("Ext :");
  ReadSensorExt();
  }
