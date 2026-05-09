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
String command;

void setup() {
  Serial.begin(9600);
  SerialBT.begin(38400);  // Start with 9600 baud
  //Serial.println("Master Ready");

pinMode(fwdBtn, INPUT_PULLUP);
pinMode(bwdBtn, INPUT_PULLUP);
pinMode(rhtBtn, INPUT_PULLUP);
pinMode(lftBtn, INPUT_PULLUP);
pinMode(hookBtn, INPUT_PULLUP);


}

void loop()
{
  	// button reads
  	bool fwd = digitalRead(fwdBtn) == LOW;
    bool bwd = digitalRead(bwdBtn) == LOW;
    bool rht = digitalRead(rhtBtn) == LOW;
    bool lft = digitalRead(lftBtn) == LOW;
    bool hk  = digitalRead(hookBtn) == LOW;


    if (fwd) drive = 'F'; //FORWARD
    else if (bwd) drive = 'B'; //BACKWARD
    else drive = 'S'; // STOP

    if (rht) steer = 'R'; //RIGHT
    else if (lft) steer = 'L'; //LEFT
    else steer = 'M'; // MIDDLE

    if (hk) hook = 'A'; //hook active
    else hook = 'I'; //hook inctive

    command = String(drive) + String(steer) + String(hook);
    //Serial.println(command);
if (rht) Serial.println("R");
if (fwd) Serial.println("F");
if (lft) Serial.println("L");
if (bwd) Serial.println("B");
if (hk) Serial.println("H");
Serial.print("-");
    SerialBT.println(command);

  delay(100); //debounce delay
}