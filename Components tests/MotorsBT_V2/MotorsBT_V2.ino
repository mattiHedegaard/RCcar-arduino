#include <NeoSWSerial.h>
#include <Servo.h>

NeoSWSerial SerialBT(8,9);
Servo ServoSteering;
Servo ServoHook;

int enA = 3;
int in1 = 4;
int in2 = 2;

int servoSteeringPin = 7;
int servoHookPin = 12;

int maxSpeed = 255;
int currentSpeed = 0;

int trigPin = 5;
int echoPin = 6;
int disGreen = 10;
int disYellow = 11;

float disDistance, disDuration;

int turnAmount = 30;
int hookAmount = 30;


char dCommand = '\0';
char sCommand = '\0';
char hCommand = '\0';


void setup(){
  // Set all the motor control pins to outputs
	  pinMode(enA, OUTPUT);
	  pinMode(in1, OUTPUT);
	  pinMode(in2, OUTPUT);

    //servo setup
    ServoSteering.attach(servoSteeringPin);
    ServoHook.attach(servoHookPin);
	  // Turn off motors - Initial state
	  digitalWrite(in1, LOW);
	  digitalWrite(in2, LOW);

    // Distance sensor
  	pinMode(trigPin,OUTPUT);
  	pinMode(echoPin,INPUT);
    pinMode(disYellow,OUTPUT);
  	pinMode(disGreen,OUTPUT);

    Serial.begin(9600);
    SerialBT.begin(38400);
    //Serial.println("Slave Ready");
}

void loop() {
  String BTsignal = SerialBT.readStringUntil('\n');

  if (BTsignal.length() >= 3) {
    dCommand = BTsignal.charAt(0);
    sCommand = BTsignal.charAt(1);
    hCommand = BTsignal.charAt(2);
  } else {
    Serial.println("BT Error");
  }
  Serial.print(dCommand);
  Serial.print(sCommand);
  Serial.print(hCommand);
  Serial.println();
  
  switch (dCommand) {
      case 'F': DirectionControle(true);             break; // FWD
      case 'B': DirectionControle(false);            break; // BWD
      case 'S': Deccelerate(0);                      break; //STOP
  }
  switch(sCommand) {
      case 'R': ServoSteering.write(90+turnAmount);  break; // RHT
      case 'L': ServoSteering.write(90-turnAmount);  break; // LFT
      case 'M': ServoSteering.write(90);             break; // MIDDLE
  }
  switch (hCommand){
      case 'A': ServoHook.write(90+hookAmount);      break;
      case 'I': ServoHook.write(90);                 break;
  } 
  CheckDistance();
  delay(100);
}

unsigned long lastMotorUpdate = 0;
int motorUpdateInterval = 50; // ms

void DirectionControle(bool forwards) {
  if (millis() - lastMotorUpdate >= motorUpdateInterval) {
    lastMotorUpdate = millis();
    
    if (forwards) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    } else {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }

    if (currentSpeed < maxSpeed) {
      currentSpeed += 10;
      if (currentSpeed > maxSpeed) currentSpeed = maxSpeed;
      analogWrite(enA, currentSpeed);
    }
  }
}

void Deccelerate(int dcDelay){  
    if (currentSpeed > 0){
        currentSpeed-=10;
        if (currentSpeed < 0) currentSpeed = 0;
        analogWrite(enA,currentSpeed);
        delay(dcDelay);
    }
    else analogWrite(enA,0);
}

unsigned long lastDistanceCheck = 0;
int distanceCheckInterval = 200; // ms

void CheckDistance(){
  if (millis() - lastDistanceCheck >= distanceCheckInterval) {
    lastDistanceCheck = millis();
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    disDuration = pulseIn(echoPin, HIGH);
    disDistance = (disDuration * 0.0343) / 2;
    
    if (disDistance < 20.0 && disDistance > 10) digitalWrite(disYellow, HIGH);
    else digitalWrite(disYellow, LOW);
    
    if (disDistance < 10.0) digitalWrite(disGreen, HIGH);
    else digitalWrite(disGreen, LOW);
  }
}