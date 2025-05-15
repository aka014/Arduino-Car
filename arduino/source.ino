#include <IRremote.hpp>

const int RECV_PIN = 2;
const int IN1 = 3;
const int IN2 = 4;
const int IN3 = 5;
const int IN4 = 6;
const int ENA = 9;
const int ENB = 10;

IRrecv irrecv(RECV_PIN);

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode()) {
    int decoded = irrecv.decodedIRData.command;
    if(decoded == 0x18){
      forward();
    }else if(decoded == 0x8){
      left();
    }else if(decoded == 0x5A){
      right();
    }else if(decoded == 0x52){
      backward();
    }
    irrecv.resume();
  }else{
    stop();
  }
  delay(137);
}

void stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void backward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}