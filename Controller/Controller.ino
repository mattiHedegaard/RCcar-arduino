#include <NeoSWSerial.h>
NeoSWSerial SerialBT(9,8);

int fwdBtn = 2;
int bwdBtn = 3;
int rhtBtn = 4;
int lftBtn = 5;
int hookBtn = 7;

char drive;
char steer;
char hook;

void setup() {
  Serial.begin(9600);
  SerialBT.begin(38400);  // Start with 9600 baud
  //Serial.println("Master Ready");

  //setting up buttons for driving
  	pinMode(fwdBtn, INPUT);
    pinMode(bwdBtn, INPUT);
  
  	//setting up buttons for turning
  	pinMode(rhtBtn, INPUT);
  	pinMode(lftBtn, INPUT);
}

void loop()
{
  	// button reads
  	bool fwd = digitalRead(fwdBtn);
  	bool bwd = digitalRead(bwdBtn);
  	bool rht = digitalRead(rhtBtn);
  	bool lft = digitalRead(lftBtn);
    bool hk = digitalRead(hookBtn);

    if (fwd) SerialBT.write('Ef'); //FORWARD
    else if (bwd) SerialBT.write('Eb'); //BACKWARD
    else SerialBT.write('Es'); // STOP

    if (rht) SerialBT.write('Sr'); //RIGHT
    else if (lft) SerialBT.write('Sl'); //LEFT
    else SerialBT.write('Sm'); // MIDDLE

    if (hk) SerialBT.write('Ha'); //hook active
    else (SerialBT.write('Hi')); //hook inctive

  delay(100); //debounce delay
}