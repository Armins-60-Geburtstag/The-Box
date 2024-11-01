
/***************
 * Fun Box für Armins Geburtstag
 * Hardware: NodeMCU ESP8266, 5 Servos, 4 Kippschalter 3Pos
 * Die Servos rotieren 360 Grad und reagieren auf speed nicht auf Position
 * Vorversuch zum Verhalten der Servos
 * Zum Programmieren müssen alle Schalter in der mittleren Position sein!
 * TODO: A Lot
***************************/

const char *whoami = "Armins Funbox - Box Control";
const char *version = "1.0";
const char *date = "2024-11-01";


#include <string.h>
#include <stdlib.h>
#include <Servo.h>
#include <Esp.h>


#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '
#define SESAM 14
#define LS A0   // now via analog Input

// Pins used for switches
#define S00 9
#define S02 10
#define S10 1
#define S12 2
#define S20 3
#define S22 13
#define S30 16
#define S32 12  // Attention GPIO16 aka D0 needs an external Pullup, e.g. 10k to 3.3.V

// Pins used for servos
#define SV0 15  // 1. Servo von rechts (D8)
#define SV1 0  // 2. Servo von rechts
#define SV2 4  // 3. Servo von rechts
#define SV3 5  // 4. Servo von rechts         Anschluss Test erfolgreich aber Servo defekt
#define SVS 14 // Sesam öffne dich! (Verriegelung des Schubkastens)


const char *delimiters            = ", \n";

// adjust time for backtrack
const int backtrack[] = {250, 300, 350, 350, 500};
int adcValue = 0;  /* Variable to store Output of ADC */

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
unsigned long switch_watch = millis();

bool isSesamOpen = false;

void setup() {
  ESP.wdtEnable(10000);
     
  //Serial.begin(115200);
  //Serial.printf("\n\n%s, Version %s, (Date: %s)\n", whoami, version, date);
  Serial.end();      //should disable RX and TX so it can be used as general I/O
  Serial1.end();
  
  delay(500);
  servo0.attach(SV0);
  servo1.attach(SV1);
  servo2.attach(SV2);
  servo3.attach(SV3);
  sesam.attach(SVS);
  
  //pinMode(LS, INPUT);   // Photo sensor high active, doesn't work because of WDT reset
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
  ESP.wdtFeed();
  
  int correctState[] = {0,2,0,2};
  int switchState[] = {1,1,1,1};   //initially switches should be in middle position to allow for correct flashing
  

  if (readSwitchState(switchState)) {       // did it change?
    switch_watch =millis();                 // reset the watch time
  } else {
    if ((millis() - switch_watch) < 2000) {
      // still thinking?
      //Serial.printf("Think time: %i\n", millis() - switch_watch);
    } else {
      if (statusCompare(switchState, correctState)){
        // right answer was guessed
        if (!isSesamOpen) openSesam();
        isSesamOpen = true;
      } else {
        if (isSesamOpen) { 
          closeSesam();
          isSesamOpen = false;
        }
        for (int i = 0; i < 4; i++) {
          resetSwitch(i);
        }
      }
    }
  }

}

bool statusCompare(int state1[],int state2[]) {
  for (int i = 0; i < 4; i++) {
    if (state1[i] != state2[i]) {
      return false;
    }
  }
  return true;
}

bool readSwitchState(int state[]){
  static int previous[4];
  bool changed = false;
  switch0_0 = digitalRead(S00);
  switch0_2 = digitalRead(S02);
  if (switch0_0 == 0 && switch0_2 == 1) {
    state[0] = 0;
    
  } else if (switch0_0 == 1 && switch0_2 == 1) {
    state[0] = 1;
    
  } else if (switch0_0 == 1 && switch0_2 == 0){
    state[0] = 2;
  } else {
    state[0] = 3; // should never happen!
  }
  if (state[0] != previous[0]) {
    previous[0] = state[0];
    changed = true;
  }
  
  switch1_0 = digitalRead(S10);
  switch1_2 = digitalRead(S12);
  if (switch1_0 == 0 && switch1_2 == 1) {
    state[1] = 0;
    
  } else if (switch1_0 == 1 && switch1_2 == 1) {
    state[1] = 1;
    
  } else if (switch1_0 == 1 && switch1_2 == 0){
    state[1] = 2;
  } else {
    state[1] = 3; // should never happen!
  }
  if (state[1] != previous[1]) {
    previous[1] = state[1];
    changed = true;
  }
  
  switch2_0 = digitalRead(S20);
  switch2_2 = digitalRead(S22);
  if (switch2_0 == 0 && switch2_2 == 1) {
    state[2] = 0;
    
  } else if (switch2_0 == 1 && switch2_2 == 1) {
    state[2] = 1;
    
  } else if (switch2_0 == 1 && switch2_2 == 0){
    state[2] = 2;
  } else {
    state[2] = 3; // should never happen!
  }
  if (state[2] != previous[2]) {
    previous[2] = state[2];
    changed = true;
  }

  switch3_0 = digitalRead(S30);
  switch3_2 = digitalRead(S32);
  if (switch3_0 == 0 && switch3_2 == 1) {
    state[3] = 0;
    
  } else if (switch3_0 == 1 && switch3_2 == 1) {
    state[3] = 1;
    
  } else if (switch3_0 == 1 && switch3_2 == 0){
    state[3] = 2;
  } else {
    state[3] = 3; // should never happen!
  }
  if (state[3] != previous[3]) {
    previous[3] = state[3];
    changed = true;
  }


// if (changed) Serial.printf("Serial switches are %i %i %i %i, changed = %s.\n", state[0],state[1], state[2], state[3], changed ? "true" : "false");
return changed;  
}

void resetSwitch(int nr) {
  switch (nr){
    case 0:
      if (digitalRead(S00)) { 
        servo0.write(0);
        while (digitalRead(S00)){   //warte bis Schalter auf 0
          ESP.wdtFeed();
        }
        turnServo(1, 180, backtrack[0]);     //time controlled backtrack, don't ask why numbering is inconsistend ;-)
      }
      break;
    case 1:
      if (digitalRead(S10)){
        servo1.write(0);
        while (digitalRead(S10)) {
          ESP.wdtFeed();
        }
        turnServo(2, 180, backtrack[1]);
      }
      
      
      break;
    case 2:
      if (digitalRead(S20)){
        servo2.write(180);
        while (digitalRead(S20)) {
          ESP.wdtFeed();
        }
        turnServo(3, 0, backtrack[2]);
      }
      
      break;
    case 3:
      if (digitalRead(S30)){
        servo2.write(180);
        while (digitalRead(S30)) {
          ESP.wdtFeed();
        }
        turnServo(3, 0, backtrack[3]);
      }
    
      // ignore for the time being
      break;
  }
  
}


void closeSesam() {
  turnServo(5, 180, backtrack[4]);   // ca. 180 deg time controlled
}

void openSesam() {
  turnServo(5, 0, 400);     // get close
  unsigned long watch = millis();
  sesam.write(80);    //slowly to spare current
  
  while ((adcValue = analogRead(LS)) < 100) {
    Serial.printf("ADC Value = %d\n", adcValue);
    if((millis() - watch) > 300) break;
  }

  sesam.write(90);
  isSesamOpen = true;
}

void turnServo(int srvo, int s_speed, unsigned long s_time) {
  switch(srvo) {
    case 1:
      servo0.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      servo0.write(90);
      break;
    case 2:
      servo1.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      servo1.write(90);
      break;
    case 3:
      servo2.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      servo2.write(90);
      break;
    case 4:
      servo3.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      servo3.write(90);
      break;
    case 5:
      sesam.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      sesam.write(90);
      break;
    default:
      Serial.println("I did not understand");
  }
}

