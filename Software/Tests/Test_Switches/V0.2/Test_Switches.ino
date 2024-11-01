
/***************
 * Fun Box für Armins Geburtstag
 * Hardware: NodeMCU ESP8266, 5 Servos, 4 Kippschalter 3 Positionen
 * 
 * Vorversuch zur Eingabe der Schalter
 * 
 * TODO: A Lot
***************************/

const char *whoami = "Armins Funbox - Test Switches";
const char *version = "0.2";
const char *date = "2024-10-31";


#include <string.h>
#include <stdlib.h>


#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer for incoming commands
char   CommandLine[COMMAND_BUFFER_LENGTH + 1];                 //Read commands into this buffer from Serial.  +1 in length for a termination char

#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '
#define S00 9   //check
#define S02 10  //check
#define S10 15  //check
#define S12 1   //TX seems to be not working
#define S20 3  //check
#define S22 13   //check        confused with S20, but why?
#define S30 16
#define S32 12  // Attention GPIO16 aka D0 needs an external Pullup, e.g. 10k to 3.3.V      confused with S30, but why?

#define LED 2 // buidin LED

// using UART1 for serial communication, GPIO2 not usable for servo

const char *delimiters            = ", \n";

int switch0_0;
int switch0_2;
int switch1_0;
int switch1_2;
int switch2_0;
int switch2_2;
int switch3_0;
int switch3_2;
unsigned long switch_watch = millis();

void setup() {
  
  //Serial.begin(115200);
  //Serial.set_tx(2);
  Serial.end();      //should disable RX and TX so it can be used as general I/O
  Serial1.end();
  
  delay(500);
  //Serial.printf("\n\n%s, Version %s, (Date: %s)\n", whoami, version, date);
  pinMode(S00, INPUT_PULLUP);  //Schalter 0, Position 0
  pinMode(S02, INPUT_PULLUP);  //Schalter 0, Position 2
  pinMode(S10, INPUT_PULLUP);  //Schalter 1, Position 0
  pinMode(S12, OUTPUT); //Spezialbehandlung
  digitalWrite(S12, HIGH);
  digitalWrite(S12, LOW);       //
  pinMode(S12, INPUT_PULLUP);  //Schalter 1, Position 2
  pinMode(S20, INPUT_PULLUP);  //Schalter 2, Position 0
  pinMode(S22, INPUT_PULLUP);  //Schalter 2, Position 2
  pinMode(S30, INPUT_PULLUP);  //Schalter 3, Position 0
  pinMode(S32, INPUT_PULLUP);  //Schalter 3, Position 2
  pinMode(LED, OUTPUT); // for indication of switch position
  delay(500);
  blinkNumber(6);
}

void loop() {
  
  int correctState[] = {0,2,0,2};
  int switchState[] = {1,1,1,1};

  if (readSwitchState(switchState)) {       // did it change?
    switch_watch =millis();                 // reset the watch time
  } else {
    if ((millis() - switch_watch) < 2000) {
      // still thinking?
      //Serial.printf("Think time: %i\n", millis() - switch_watch);
    } else {
      if (statusCompare(switchState, correctState)){
        // right answer was guessed
        //Serial.printf("Success! The Drawer will be opened");
        openSesam();
      } else {
        for (int i = 0; i < 4; i++) {
          //resetSwitch(i);
          //Serial.printf("Attempting to reset switch %i",i);
        }
      }
    }
  }

  
  
} /* loop() */

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
  if (switch0_0 == 0 && switch0_2 == 1) { //ok 31.10.
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
    blinkNumber(1);
    delay(1000);
    blinkNumber(state[0]+1);
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
    state[1] = 3; // should never happen!           But it happens on Pos 0
  }
  if (state[1] != previous[1]) {
    previous[1] = state[1];
    changed = true;
    blinkNumber(2);
    delay(1000);
    blinkNumber(state[1]+1);
    
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
    blinkNumber(3);
    delay(1000);
    blinkNumber(state[2]+1);
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
    blinkNumber(4);
    delay(1000);
    blinkNumber(state[3]+1);
  }


//if (changed) Serial.printf("Serial switches are %i %i %i %i, changed = %s.\n", state[0],state[1], state[2], state[3], changed ? "true" : "false");
//if (changed) blinkState(state);
return changed;  
}

void blinkState(int state[]) {
  for (int i = 0; i < 4; i++) {
    blinkNumber(i);
    delay(500);
    blinkNumber(state[i]);
    delay(1000);
  }
}

void blinkNumber(int number) {
  for (int i = 0; i < number; i++) {
    digitalWrite(LED, LOW);
    delay(200);
    digitalWrite(LED, HIGH);
    delay(200);
  }
}

void openSesam() {
  blinkNumber(10);
}

