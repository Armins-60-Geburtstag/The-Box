
/***************
 * Fun Box für Armins Geburtstag
 * Hardware: NodeMCU ESP8266, 5 Servos, 4 Kippschalter 3 Positionen
 * Die Servos rotieren 360 Grad und reagieren auf speed nicht auf Position
 * Vorversuch zur Eingabe der Schalter
 * Drehrichtung mit Blick von hinten auf das Gehäuse
 * TODO: A Lot
***************************/

const char *whoami = "Armins Funbox";
const char *version = "0.0b";
const char *date = "2024-10-26";


#include <string.h>
#include <stdlib.h>
#include <Servo.h>

#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer for incoming commands
char   CommandLine[COMMAND_BUFFER_LENGTH + 1];                 //Read commands into this buffer from Serial.  +1 in length for a termination char

#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '
#define S00 10
#define S02 9
#define S10 15
#define S12 1
#define S20 13
#define S22 3
#define S30 12
#define S32 16

const char *delimiters            = ", \n";

Servo servo0;   //1. Schalterposition von rechts
Servo servo1;   //2. Schalterposition von rechts
Servo servo2;   //3. Schalterposition von rechts
Servo servo3;   //4. Schalterposition von rechts
Servo sesam;    //Riegel für den Schubkasten

int switch0_0;
int switch0_2;
int switch1_0;
int switch1_2;
int switch2_0;
int switch2_2;
int switch3_0;
int switch3_2;

void setup() {
  
  Serial.begin(115200);
  Serial.printf("\n\n%s, Version %s, (Date: %s)\n", whoami, version, date);
  servo0.attach(D4);
  servo1.attach(D3);
  servo2.attach(D2);
  servo3.attach(D1);
  sesam.attach(D5);
  pinMode(S00, INPUT_PULLUP);  //Schalter 0, Position 0
  pinMode(S02, INPUT_PULLUP);  //Schalter 0, Position 2
  pinMode(S10, INPUT_PULLUP);  //Schalter 1, Position 0
  pinMode(S12, INPUT_PULLUP);  //Schalter 1, Position 2
  pinMode(S20, INPUT_PULLUP);  //Schalter 2, Position 0
  pinMode(S22, INPUT_PULLUP);  //Schalter 2, Position 2
  pinMode(S30, INPUT_PULLUP);  //Schalter 3, Position 0
  pinMode(S32, INPUT_PULLUP);  //Schalter 3, Position 2
}

void loop() {
  
  switch0_0 = digitalRead(S00);
  switch0_2 = digitalRead(S02);
  Serial.printf("Digital Input D5: %i, D6: %i \n", switch0_0, switch0_2);

  if (switch0_0 == 0 && switch0_2 == 0){
    //verbotener Zustand, Schalter gleichzeitig oben und unten
    Serial.println("Something terrible happened");
  }
  if (switch0_0 == 0) {
    servo0.write(80);   //slowly to the Left
    Serial.printf("Turning Left");
  } else if (switch0_2 == 0) {
    servo0.write(100);  //slowly to the Right
  } else {
    servo0.write(90);   //stop
  }
  switch1_0 = digitalRead(S10);
  switch1_2 = digitalRead(S12);
  Serial.printf("Digital Input D5: %i, D6: %i \n", switch0_0, switch0_2);

  if (switch1_0 == 0 && switch1_2 == 0){
    //verbotener Zustand, Schalter gleichzeitig oben und unten
    Serial.println("Something terrible happened");
  }
  if (switch1_0 == 0) {
    servo1.write(80);   //slowly to the Left
    Serial.printf("Turning Left");
  } else if (switch0_2 == 0) {
    servo1.write(100);  //slowly to the Right
  } else {
    servo1.write(90);   //stop
  }
  switch2_0 = digitalRead(S20);
  switch2_2 = digitalRead(S22);
  Serial.printf("Digital Input D5: %i, D6: %i \n", switch0_0, switch0_2);

  if (switch2_0 == 0 && switch2_2 == 0){
    //verbotener Zustand, Schalter gleichzeitig oben und unten
    Serial.println("Something terrible happened");
  }
  if (switch2_0 == 0) {
    servo2.write(80);   //slowly to the Left
    Serial.printf("Turning Left");
  } else if (switch2_2 == 0) {
    servo2.write(100);  //slowly to the Right
  } else {
    servo2.write(90);   //stop
  }
  switch3_0 = digitalRead(S30);
  switch3_2 = digitalRead(S32);
  Serial.printf("Digital Input D5: %i, D6: %i \n", switch0_0, switch0_2);

  if (switch3_0 == 0 && switch3_2 == 0){
    //verbotener Zustand, Schalter gleichzeitig oben und unten
    Serial.println("Something terrible happened");
  }
  if (switch3_0 == 0) {
    servo0.write(80);   //slowly to the Left
    Serial.printf("Turning Left");
  } else if (switch3_2 == 0) {
    servo0.write(100);  //slowly to the Right
  } else {
    servo0.write(90);   //stop
  }

  
  
  
  
  delay(100);
  
}
