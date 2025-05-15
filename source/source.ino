#include <IRremote.hpp>

// IR receiver pin
const int RECV_PIN = 2;
// Spin left wheels forward
const int IN1 = 3;
// Spin left wheels backward
const int IN2 = 4;
// Spin right wheels backward
const int IN3 = 5;
// Spin right wheels forward
const int IN4 = 6;
// Start motor A (left wheels)
const int ENA = 9;
// Start motor B (right wheels)
const int ENB = 10;

// Create an IR receiver object
IRrecv irrecv(RECV_PIN);

// Delay duration
const int TICK = 137;

void setup() {
  
  // Initialize pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Start L298N motor driver
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  // Initialize and start the IR receiver
  irrecv.enableIRIn();
}

void loop() {

  // Try to receive and decode value
  if (irrecv.decode()) {

    // Get the value of the last successfully received signal
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

    // Re-enable the IR receiver
    irrecv.resume();
  }else{
    stop();
  }
  delay(TICK);
}

void stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward(){

  // Spin all wheels forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left(){

  // Spin right wheels forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void right(){

  // Spin left wheels forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void backward(){

  // Spin all wheels backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}