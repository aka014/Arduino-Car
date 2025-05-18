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

const int REMOTE_NUM_2 = 0x18;

const int REMOTE_NUM_4 = 0x8;

const int REMOTE_NUM_6 = 0x5A;

const int REMOTE_NUM_7 = 0x42;

const int REMOTE_NUM_8 = 0x52;

const int REMOTE_NUM_9 = 0x4A;






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

    if(decoded == REMOTE_NUM_2){
      forward();
    }else if(decoded == REMOTE_NUM_4){
      left();
    }else if(decoded == REMOTE_NUM_6){
      right();
    }else if(decoded == REMOTE_NUM_8){
      backward();
    }else if(decoded == REMOTE_NUM_7){
      driftLeft();
    }else if(decoded == REMOTE_NUM_9){
      driftRight();
    }

    // Re-enable the IR receiver
    irrecv.resume();
  }else{
    stop();
  }
  delay(TICK);
}

void stop(){


  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void spinForward(){

  // Spin all wheels forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void driftLeft(){

  analogWrite(ENA, 0);
  analogWrite(ENB, 255);
  spinForward();
}

void driftRight(){

  analogWrite(ENA, 255);
  analogWrite(ENB, 0);
  spinForward();
}

void backward(){

  // Spin all wheels backward
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void forward(){
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  spinForward();
}

void left(){

  analogWrite(ENA, 127);
  analogWrite(ENB, 255);
  spinForward();
}

void right(){

  analogWrite(ENB, 96);
  analogWrite(ENA, 255);
  spinForward();
}