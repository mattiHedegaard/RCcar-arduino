#include <NeoSWSerial.h>
#include <Servo.h>

NeoSWSerial SerialBT(8,9);
Servo ServoSteering;
Servo ServoHook;

//DC motor controls
int enA = 3;
int in1 = 4;
int in2 = 2;

//Servo pin setup
int servoSteeringPin = 7;
int servoHookPin = 12;

//Drive motor setup
int maxSpeed = 255;
int currentSpeed = 0;

//Ultra sonic distance setup
int trigPin = 5;
int echoPin = 6;
//distance LED setup
int disGreen = 10;
int disYellow = 11;

float disDistance, disDuration;

//steering angle
int turnAmount = 30;
//grabbing angle
int hookAmount = 75;

//commands
char dCommand = '\0'; //Driving command
char sCommand = '\0'; //Steering command
char hCommand = '\0'; //Hook command


void setup(){
  //Pin setup for motor
	  pinMode(enA, OUTPUT);
	  pinMode(in1, OUTPUT);
	  pinMode(in2, OUTPUT);

    //servo setup
    ServoSteering.attach(servoSteeringPin);
    ServoHook.attach(servoHookPin);
	  //Initial motor turn-off
	  digitalWrite(in1, LOW);
	  digitalWrite(in2, LOW);

    //Distance sensor pin setup
  	pinMode(trigPin,OUTPUT);
  	pinMode(echoPin,INPUT);
    pinMode(disYellow,OUTPUT);
  	pinMode(disGreen,OUTPUT);

    //Baud setup
    Serial.begin(9600);
    SerialBT.begin(38400);
}

void loop() {
  //read bluetothsignal
  String BTsignal = SerialBT.readStringUntil('\n');

  //load command string onto command chars
  if (BTsignal.length() >= 3) {
    dCommand = BTsignal.charAt(0);
    sCommand = BTsignal.charAt(1);
    hCommand = BTsignal.charAt(2);
  } else {
    Serial.println("BT Error");
    //resets commands if bluetooth command signal is missing or is faulty
    dCommand = 'S';
    sCommand = 'M';
    hCommand = 'I';
  }
  
  //execute commands
  switch (dCommand) {
      case 'F': DirectionControle(true);             break; //Accelerates forwads
      case 'B': DirectionControle(false);            break; //Accelerates backwards
      case 'S': Deccelerate(0);                      break; //Deccelerates the car
  }
  switch(sCommand) {
      case 'R': ServoSteering.write(90+turnAmount);  break; //Sets steering right
      case 'L': ServoSteering.write(90-turnAmount);  break; //Sets steering left
      case 'M': ServoSteering.write(90);             break; //Sets steering in center
  }
  switch (hCommand){
      case 'A': ServoHook.write(90+hookAmount);      break; //Hook Active
      case 'I': ServoHook.write(90);                 break; //Hook Inactive
  } 
  CheckDistance();
  delay(10); //debounce delay
}

//Setup how often DC motor can update
unsigned long lastMotorUpdate = 0;
int motorUpdateInterval = 50; // ms

void DirectionControle(bool forwards) {
  //checks time since last update
  if (millis() - lastMotorUpdate >= motorUpdateInterval) {
    //updates timestamp
    lastMotorUpdate = millis();
    
    //turns on DC motor in the right direction
    if (forwards) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    } else {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }

    //increase speed
    if (currentSpeed < maxSpeed) {
      currentSpeed += 10;
      if (currentSpeed > maxSpeed) currentSpeed = maxSpeed; // if it gets above, it is set to equal
      analogWrite(enA, currentSpeed); //writes speed to motor
    }
    else if(currentSpeed > maxSpeed){
      Deccelerate(0);
      if (currentSpeed < maxSpeed) currentSpeed = maxSpeed;
    }
  }
}

void Deccelerate(int dcDelay){  
    //decreases speed until under zero, then sets to 0
    if (currentSpeed > 0){
        currentSpeed-=10;
        if (currentSpeed < 0) currentSpeed = 0;
        analogWrite(enA,currentSpeed);
        delay(dcDelay); // how fast it will slow down
    }
    else analogWrite(enA,0); // writes wpeed to motor
}

//setup for how often it can check distance
unsigned long lastDistanceCheck = 0;
int distanceCheckInterval = 200; // ms

void CheckDistance(){
  if (millis() - lastDistanceCheck >= distanceCheckInterval) {
    //updates timestamp
    lastDistanceCheck = millis();
    
    //triggers the sensor by giving it a 10 microsecond pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    //measures how long it takes for the echo to return
    disDuration = pulseIn(echoPin, HIGH);
    //converts time to cm
    disDistance = (disDuration * 0.0343) / 2;
    
    //chcks yellow light range
    if (disDistance < 30.0 && disDistance > 15.0){
        digitalWrite(disYellow, HIGH);
        //updates max speed to aproach more slowly
        maxSpeed = 200;
    }
    else {
      digitalWrite(disYellow, LOW);
      //defaults max speed to top speed, done here so it dosen't overwrite the slow speed when only yellow light is on
      maxSpeed=255;
      }
    
    //checks green light treshold
    if (disDistance < 15.0) {
      digitalWrite(disGreen, HIGH);
      //updates max speed to aproach even more slowly
      maxSpeed=150;
      }
    else {
      digitalWrite(disGreen, LOW);
      }
  }
}