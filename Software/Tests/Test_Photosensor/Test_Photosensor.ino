
/***************
 * Fun Box für Armins Geburtstag
 * Hardware: NodeMCU ESP8266, 5 Servos, 4 Kippschalter 3 Positionen
 * Die Servos rotieren 360 Grad und reagieren auf speed nicht auf Position
 * Vorversuch zum Auslesen der Lichtschranke über Analogeingang
 * Test im ausgebauten Zustand
 * 
 * 
 * TODO: A Lot
***************************/

const char *whoami = "Armins Funbox - Test Lichtschranke";
const char *version = "0.1";
const char *date = "2024-11-01";


#define LS A0

int adcValue = 0;  /* Variable to store Output of ADC */

void setup()
{
  Serial.begin(115200); /* Initialize serial communication at 115200 */
  Serial.printf("\n\n%s, Version %s, (Date: %s)\n", whoami, version, date);
}

void loop()
{
  adcValue = analogRead(LS);
 
  /* Print the output in the Serial Monitor */
  Serial.printf("ADC Value = %d\n", adcValue);
  
  
  delay(1000);
}


