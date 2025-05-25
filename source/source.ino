#include <IRremote.hpp>

// IR receiver pin
const int RECV_PIN = 2;
// Ultrasonic distance sensor Trigger pin
const int TRIG_PIN = 12;
// Ultrasonic distance sensor Echo pin
const int ECHO_PIN = 13;
// Light intensity sensitive LED pin
// THIS PIN MIGHT NOT WORK FOR PWM RIGHT IF MILLIS() OR DELAY() IS USED!
const int LED_PIN = 6;

// Spin left wheels forward
const int IN1 = 3;
// Spin left wheels backward
const int IN2 = 4;
// Spin right wheels backward
const int IN3 = 5;
// Spin right wheels forward
const int IN4 = 7;
// Start motor A (left wheels)
const int ENA = 9;
// Start motor B (right wheels)
const int ENB = 10;


// IR remote controller button 2
const int REMOTE_NUM_2 = 0x18;
// IR remote controller button 4
const int REMOTE_NUM_4 = 0x8;
// IR remote controller button 6
const int REMOTE_NUM_6 = 0x5A;
// IR remote controller button 7
const int REMOTE_NUM_7 = 0x42;
// IR remote controller button 8
const int REMOTE_NUM_8 = 0x52;
// IR remote controller button 9
const int REMOTE_NUM_9 = 0x4A;
// Joystick neutral position
const int STOP = 0x20;

// Used to measure time elapsed from the last LED brightness update
int lastTick = 0;

// Delay duration
const int TICK = 127;
// If photoresistor's voltage is below this threshold, LED turns on (Note: Voltage is read in units where a unit = 4.9 mV)
const int BR_THRESHOLD = 300;
// Minimum allowed distance from obstacle while going backward
const int DIST_THRESHOLD = 20;

// Create an IR receiver object
IRrecv irrecv(RECV_PIN);


void setup() {

  //Testing
  Serial.begin(9600);

  // Initialize pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Ensure the trigger pin is LOW initially
  digitalWrite(TRIG_PIN, LOW);
  delay(1000); // Give ultrasonic sensor some time to settle

  
  // Start L298N motor driver
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);


  // CHECK IF NECCESSARY
  digitalWrite(LED_PIN, HIGH);

  // Initialize and start the IR receiver
  irrecv.enableIRIn();

  // Initial value of lastTick
  //lastTick = millis();
}



void loop() {

  // Try to receive and decode value
  if (irrecv.decode()) {

    // Get the value of the last successfully received signal
    int decoded = irrecv.decodedIRData.command;

    // Testing
    //Serial.println(decoded, HEX);

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
    }else if(decoded == STOP){
      stop();
    }

    // Do not let car go backward in case the distance to obstacle is less than DIST_THRESHOLD cm
    if (checkDistance() < DIST_THRESHOLD and decoded == REMOTE_NUM_8) stop();

    // Update LED brightness depending on the photoresistor's measured voltage
    // if (millis() - lastTick >= 3000) {
    //   int val = analogRead(A0);

    //   if (val < BR_THRESHOLD) {
    //     int brightness = (int) ((BR_THRESHOLD - val) * 1.275);
    //     brightness = brightness > 255 ? 255 : brightness;
    //     analogWrite(LED_PIN, brightness);
    //   }
    //   else analogWrite(LED_PIN, 0);

    //   lastTick = millis();
    // }

    // Re-enable the IR receiver
    irrecv.resume();
  }

  // Delay is required in order to make the car feel responsive to commands
  delay(TICK);
}

void stop(){
  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, LOW);
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, LOW);

  // Disable all motors
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

  // Enable only right wheels and spin forward
  analogWrite(ENA, 0);
  analogWrite(ENB, 127);
  spinForward();
}

void driftRight(){

  // Enable only left wheels and spin forward
  analogWrite(ENA, 127);
  analogWrite(ENB, 0);
  spinForward();
}

void backward(){

  // Enable all motors
  analogWrite(ENA, 127);
  analogWrite(ENB, 127);
  // Spin all wheels backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void forward(){

  // Enable all motors and spin forward
  analogWrite(ENA, 127);
  analogWrite(ENB, 127);
  spinForward();
}

void left(){

  //Fully enable right wheels and partially enable left wheels, then spin forward
  analogWrite(ENA, 96);
  analogWrite(ENB, 127);
  spinForward();
}

void right(){

  //Fully enable left wheels and partially enable right wheels, then spin forward
  analogWrite(ENB, 96);
  analogWrite(ENA, 127);
  spinForward();
}

float checkDistance(){
  // Clear the TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send a 10 microsecond HIGH pulse to trigger the ultrasonic burst
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the time it takes for the echo to return
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout after 30 ms (max ~5 meters)

  // Check if a valid pulse was received
  if (duration == 0) {
    Serial.println("Warning: No echo received");
    return -1;
  }else{

    // Calculate the distance in centimeters
    float distance = duration / 58.3;

    // Print the distance to the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    return distance;
  }
}