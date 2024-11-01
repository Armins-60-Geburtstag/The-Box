
/***************
 * Fun Box für Armins Geburtstag
 * Hardware: NodeMCU ESP8266, 5 Servos, 4 Kippschalter 3 Positionen
 * Die Servos rotieren 360 Grad und reagieren auf speed nicht auf Position
 * Vorversuch zur Ansteuerung  der Servos
 * Zum Programmieren müssen alle Schalter in der mittleren Position sein!
 * Zum Programmieren die Servos besser abstecken
 * 
 * Drehrichtung mit Blick von hinten auf das Gehäuse
 * TODO: A Lot
***************************/

const char *whoami = "Armins Funbox - Test Servos";
const char *version = "0.1";
const char *date = "2024-10-30";


#include <string.h>
#include <stdlib.h>
#include <Servo.h>
#include <Esp.h>

#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer for incoming commands
char   CommandLine[COMMAND_BUFFER_LENGTH + 1];                 //Read commands into this buffer from Serial.  +1 in length for a termination char

#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '
#define SV0 2  // 1. Servo von rechts
#define SV1 0  // 2. Servo von rechts
#define SV2 4  // 3. Servo von rechts
#define SV3 5  // 4. Servo von rechts         Anschluss Test erfolgreich aber Servo defekt
#define SVS 14 // Sesam öffne dich! (Verriegelung des Schubkastens)
#define LS 8

const char *delimiters            = ", \n";


Servo servo0;   //1. Position von rechts
Servo servo1;   //2. Position von rechts
Servo servo2;   //3. Position von rechts
Servo servo3;   //4. Position von rechts
Servo sesam;    //Riegel für den Schubkasten

void setup() {
  ESP.wdtEnable(10000);
  Serial.begin(115200);
  Serial.printf("\n\n%s, Version %s, (Date: %s)\n", whoami, version, date);
  servo0.attach(SV0);
  servo1.attach(SV1);
  servo2.attach(SV2);
  servo3.attach(SV3);
  sesam.attach(SVS);
//  pinMode(LS, INPUT);   // Photo sensor high active, doesn't work because of WDT reset
}

void loop() {

  bool received = getCommandLineFromSerialPort(CommandLine);      //global CommandLine is defined in CommandLine.h
  if (received) {
    if (parseCommandLine(CommandLine)){
      Serial.println("parse success.");
    } else {
      Serial.println("parse error.");
      help();
    }    
  }
  

}

bool getCommandLineFromSerialPort(char * commandLine) {
  static uint8_t charsRead = 0;                      //note: COMAND_BUFFER_LENGTH must be less than 255 chars long
  //read asynchronously until full command input
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case CR:      //likely have full command in buffer now, commands are terminated by CR and/or LS
      case LF:
        commandLine[charsRead] = NULLCHAR;       //null terminate our command char array
        if (charsRead > 0)  {
          charsRead = 0;                           //charsRead is static, so have to reset
          return true;
        }
        break;
      case BS:                                    // handle backspace in input: put a space in last char
        if (charsRead > 0) {                        //and adjust commandLine and charsRead
          commandLine[--charsRead] = NULLCHAR;
          Serial << byte(BS) << byte(SPACE) << byte(BS);  //no idea how this works, found it on the Internet
        }
        break;
      default:
        // c = tolower(c);
        if (charsRead < COMMAND_BUFFER_LENGTH) {
          commandLine[charsRead++] = c;
        }
        commandLine[charsRead] = NULLCHAR;     //just in case
        break;
    }
  }
  return false;
}

/*
 * expected: s <servo> <speed> <milliseconds>
 */
 
bool parseCommandLine(char * commandLine) {
  Serial.println(commandLine );
  char *ptrToCommandName = strtok(commandLine, delimiters)    ;
  if (strlen(ptrToCommandName) > 1){
    Serial.println("Syntax Error: One letter Command only!");
    return false;
  } else {
    switch(ptrToCommandName[0]) {
      case 's':
        {
          char *argument = strtok(NULL, delimiters); // get servo number
          int srvo;
          int s_speed;
          int s_time;
          Serial.printf("\n1. argument: %s\n",argument);
          if ((srvo = atoi (argument)) > 0 && srvo <= 5) {
            
            char* argument2 = strtok(NULL, delimiters); // get speed
            Serial.printf("\n2. argument: %s\n",argument2);
            if ((s_speed = atoi (argument2)) >= 0 && s_speed <= 180) {

              char* argument3 = strtok(NULL, delimiters); // get turning time
              Serial.printf("\n3. argument: %s\n",argument3);
              if ((s_time = atoi (argument3)) > 0 && s_time < 1000) {
                // now we should have all valid parameters
                Serial.printf("\nAttempt to turn Servo %i with speed %i for %i ms.", srvo, s_speed, s_time);
                turnServo(srvo, s_speed, s_time);
                return true;
              } else {
                Serial.println("invalid running time.");
                help();
                return false;
              }
              
              
            } else {
              Serial.println("invalid servo speed.");
              help();
              return false;
            }
            break;
          } else {
            Serial.println("invalid servo number.");
            help();
            return false;
          }
        }
      case 'o':     // Sesam: open!
        openSesam();
        break;
      default:  
        Serial.println("Unknown command");  
        return false;
    }
  }
  return true;  
  
}
void openSesam() {
  turnServo(5,180,630);
  /*
   * usage of GPIO8 seems to be a problem
   *
  sesam.write(180);   // full speed
  int status;
  status = digitalRead(LS);
  Serial.printf("Photo sensor is %i.", status);
  return;
  while (!digitalRead(LS));
  sesam.write(90);
  */
}

void turnServo(int srvo, int s_speed, unsigned long s_time) {
  switch(srvo) {
    case 1:
      pinMode(SV0, OUTPUT);   // activate OUTPUT only when necessary
      servo0.attach(SV0);
      servo0.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      servo0.write(90);
      pinMode(SV0, INPUT);    // deaktivate the pin to avoid interference next time programming
      break;
    case 2:
      pinMode(SV1, OUTPUT);   // activate OUTPUT only when necessary
      servo0.attach(SV1);
      servo1.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      servo1.write(90);
      pinMode(SV1, INPUT);    // deaktivate the pin to avoid interference next time programming
      
      break;
    case 3:
      pinMode(SV2, OUTPUT);   // activate OUTPUT only when necessary
      servo0.attach(SV2);
      servo2.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      servo2.write(90);
      pinMode(SV2, INPUT);    // deaktivate the pin to avoid interference next time programming
      
      break;
    case 4:
      pinMode(SV3, OUTPUT);   // activate OUTPUT only when necessary
      servo0.attach(SV3);
      servo3.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      servo3.write(90);
      pinMode(SV3, INPUT);    // deaktivate the pin to avoid interference next time programming
      break;
    case 5:
      pinMode(SVS, OUTPUT);   // activate OUTPUT only when necessary
      servo0.attach(SVS);
      sesam.write(s_speed);
      delay(s_time);            // very crude, timer should be used
      sesam.write(90);
      pinMode(SVS, INPUT);    // deaktivate the pin to avoid interference next time programming
      break;
    default:
      Serial.println("I did not understand");
  }
}

void help() {
  Serial.println("usage:");
  Serial.println("[so] <servo> <speed> <milliseconds>");
  Serial.println("s means turn servo");
  Serial.println("o means open the drawer latch");
  Serial.println("<servo> number of servo, valid 1 .. 5");
  Serial.println("<speed> servo speed, 0 .. 89 - turning left, 91 .. 180 - turning right, 90 - stop");
  Serial.println("<milliseconds> turning time in ms, valid 10 .. 1000");
}

