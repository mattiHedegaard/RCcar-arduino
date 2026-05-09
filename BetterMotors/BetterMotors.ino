#include <Servo.h> 

int fwdBtn = 13;
int bwdBtn = 12;
int rhtBtn = 8;
int lftBtn = 7;

int enA = 3;
int in1 = 4;
int in2 = 2;

int servo1Pin = 9;
Servo Servo1;

int maxSpeed = 100;
int currentSpeed = 0;

int trigPin = 5;
int echoPin = 6;
int disGreen = 10;
int disYellow = 11;

float disDuration,disDistance;

void setup()
{
  	Servo1.attach(servo1Pin);
  
  	//setting up buttons for driving
  	pinMode(fwdBtn, INPUT);
    pinMode(bwdBtn, INPUT);
  
  	//setting up buttons for turning
  	pinMode(rhtBtn, INPUT);
  	pinMode(lftBtn, INPUT);
  
  	// Set all the motor control pins to outputs
	  pinMode(enA, OUTPUT);
	  pinMode(in1, OUTPUT);
	  pinMode(in2, OUTPUT);
	
	  // Turn off motors - Initial state
	  digitalWrite(in1, LOW);
	  digitalWrite(in2, LOW);
  
  	// Distance sensor
  	pinMode(trigPin,OUTPUT);
  	pinMode(echoPin,INPUT);
    pinMode(disYellow,OUTPUT);
  	pinMode(disGreen,OUTPUT);
  
  	Serial.begin(9600);
}

void loop()
{
  	// button reads
  	bool fwd = digitalRead(fwdBtn);
  	bool bwd = digitalRead(bwdBtn);
  	bool rht = digitalRead(rhtBtn)==HIGH;
  	bool lft = digitalRead(lftBtn)==HIGH;
  
    // DC motor
  	if (fwd) DirectionControle(20,true);
  	else if (bwd) DirectionControle(20,false);
    else if (!fwd && !bwd && currentSpeed > 5) Deccelerate(20);
  
  	// servo motor
  	if (rht) Servo1.write(180);
  	else if (lft) Servo1.write(0); 
  	else Servo1.write(90);
      
    CheckDistance();
    
}

// controles the speed and direction of the motor
void DirectionControle(int acDelay, bool forwards) {
	// Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
  	int pin = 0;
  
  	if (forwards)
    {
      	digitalWrite(in1, LOW);
		    digitalWrite(in2, HIGH);
      	pin = fwdBtn;
    }
  	else
    {
      	digitalWrite(in1, HIGH);
		    digitalWrite(in2, LOW);
      	pin = bwdBtn;
    }

    if (currentSpeed < maxSpeed){
          currentSpeed++;
          analogWrite(enA,currentSpeed);
          delay(acDelay);
          Serial.print(currentSpeed);
    }
}

void Deccelerate(int dcDelay){  
    if (currentSpeed > 5){
        currentSpeed--;
        analogWrite(enA,currentSpeed);
        delay(dcDelay);
    }
    else analogWrite(enA,0);
}

void CheckDistance(){
      digitalWrite(trigPin, LOW);  
      delayMicroseconds(2);  
      digitalWrite(trigPin, HIGH);  
      delayMicroseconds(10);  
      digitalWrite(trigPin, LOW);
    
      disDuration = pulseIn(echoPin, HIGH); 
    
      disDistance = (disDuration*.0343)/2;  
    
      //Serial.print("Distance: ");  
      //Serial.println(disDistance);
    
      if (disDistance < 20.0 && disDistance > 10.0){
          digitalWrite(disYellow,HIGH);
          //Serial.println("teat");
      }
      else if (disDistance < 10.0 && disDistance > 0.5){
          digitalWrite(disYellow,LOW);
          digitalWrite(disGreen,HIGH);
          //Serial.println("klar");
      }
      else{
          digitalWrite(disYellow,LOW);
          digitalWrite(disGreen,LOW);
          //Serial.println("veak");
      }
}