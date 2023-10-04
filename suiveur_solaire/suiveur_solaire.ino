/*
bricbric
Prog initial 170823
maj  21 09 23 secont axe
Ajout  wachtdog    27092023



sur  A0   on  lit  la valeur entre deux photoresistance
sur  A1   on  lit  la valeur entre deux photoresistance

sur 8  on active le relais  gauche
sur 9  on active le relais  droit
sur 10  on active le relais  haut
sur 11  on active le relais  bas

sur 6  si on  met a la masse on obtient  un delai 10 fois plus important

si la carte relais  reagit inversement a notre  choix   changer les affectation Relaisactif Relaisnonactif

* 
*/
 
#include <avr/wdt.h>

int delai = 10; // temps en seconde x 2 de reation des actionneurs

// info pinoche
#define gauche 8
#define droite 9
#define haut 10
#define bas 11
#define reaction 6



///   en lien avec la carte relais  decommanter la bonne fonction en fonction des relais
#define Relaisactif HIGH
#define Relaisnonactif LOW
//#define Relaisactif LOW
//#define Relaisnonactif HIGH

#define delai_theorique 10
#define hysteresis 50 // valeur neutre sans changement 
#define centre 512



int retardhb, retardgd = 0;
long   resetplaquette =  0;


//Variables:
const uint32_t loopTime = 500; //2hz
uint32_t lastTime = loopTime;
uint32_t currentTime = loopTime;




// the setup routine runs once when you press reset:
void setup() {

  //    WDTO_15MS
  //    WDTO_30MS
  //    WDTO_60MS
  //    WDTO_120MS
  //    WDTO_250MS
  //    WDTO_500MS
  //    WDTO_1S
  //    WDTO_2S
  //    WDTO_4S
  //    WDTO_8S
  wdt_enable(WDTO_1S);// selection du temps alloué  pour  ne pas faire  reset

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  Serial.println(" Suiveur-solaire  bricbric  ");

  pinMode(reaction, INPUT_PULLUP);
  pinMode(gauche, OUTPUT);
  pinMode(droite, OUTPUT);
  pinMode(haut, OUTPUT);
  pinMode(bas, OUTPUT);


  retardhb = delai;
  retardgd = delai;

  wdt_reset();

}

// the loop routine runs over and over again forever:
void loop() {

  currentTime = millis();
  //Serial.println(currentTime - lastTime);

  if ((currentTime - lastTime) >= loopTime) {  //start timed loop
    lastTime = currentTime ;

    if (!digitalRead(reaction)) delai =  delai_theorique * 10;
    else delai = delai_theorique ;

    Serial.print("delai ");
    Serial.print( delai );


    if ( retardgd  < 0) retardgd  = 0;
    if ( retardhb  < 0) retardhb  = 0;


    // read the input on analog pin 0:  capteur   gauche droite
    int sensorvalue = analogRead(A0);
    // read the input on analog pin 1:   capteur   haut bas
    int sensorvalue1 = analogRead(A1);

    // print out the value you read:

    Serial.print(" capteur   gauche droite ");
    Serial.print(sensorvalue);

    Serial.print(" capteur   haut bas ");
    Serial.print (sensorvalue1);

    // test  du gauche droite


    Serial.print(" retardgd ");
    Serial.print( retardgd );

    if ((sensorvalue > centre - hysteresis) and (sensorvalue < centre + hysteresis)) {
      digitalWrite(gauche, Relaisnonactif);
      digitalWrite(droite, Relaisnonactif);
      Serial.print(" neutre gauche droite");
      retardgd = delai;

    } else if   (sensorvalue < centre - hysteresis) {
      if (retardgd--  < 1) {
        digitalWrite(gauche, Relaisactif);
        digitalWrite(droite, Relaisnonactif);
        Serial.print(" gauche");
      } else        Serial.print(" retard gauche");

    } else if   (sensorvalue > centre + hysteresis) {
      if (retardgd--  < 1) {
        digitalWrite(droite, Relaisactif);
        digitalWrite(gauche, Relaisnonactif);
        Serial.print(" droite");
      } else        Serial.print(" retard droite");
    }

    // test  du haut bas


    Serial.print(" retardhb ");
    Serial.print( retardhb);
    if ((sensorvalue1 > centre - hysteresis) and (sensorvalue1 < centre + hysteresis)) {
      digitalWrite(haut, Relaisnonactif);
      digitalWrite(bas, Relaisnonactif);
      Serial.println(" neutre  haut  bas ");

      retardhb = delai;
    } else if   (sensorvalue1 < centre - hysteresis) {
      if (retardhb --  < 1) {

        digitalWrite(haut, Relaisactif);
        digitalWrite(bas, Relaisnonactif);
        Serial.println(" haut");
      } else        Serial.println(" retard haut");
    } else if   (sensorvalue1 > centre + hysteresis) {
      if (retardhb--  < 1) {

        digitalWrite(bas, Relaisactif);
        digitalWrite(haut, Relaisnonactif);
        Serial.println(" bas");
      } else        Serial.println(" retard bas");
    }


    wdt_reset(); //  on nourrit le chien de garde !
  }


}
