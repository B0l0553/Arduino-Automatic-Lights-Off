#include <Wire.h>
#include <BH1750.h>
#define MOVE_SENSOR 2
#define LED 3

BH1750 lightMeter;
bool L_ON = 0;
short CTR = 0;

void setup() {
  
  // On definie le mode des pins utilise
  pinMode(MOVE_SENSOR, INPUT);
  pinMode(LED, OUTPUT);

  // Initialisation des librairies requise
  Wire.begin();
  lightMeter.begin(); 
}

void loop() {
  // On vas cherche les valeurs des deux detecteurs
  auto sensorValue = digitalRead(MOVE_SENSOR);
  float lux = lightMeter.readLightLevel();

  // Si on detecte une personne
  if( sensorValue == HIGH ) {
    // On regarde si la piece est bien eclairer
    if( lux <= 400 && !(lux >= 5000) ) {
      // Si non on allume
        digitalWrite(LED, HIGH);
    } else {

      // Sinon, on verifie que l'eclairage n'est deja pas assurer par le soleil
      // Dehors le soleil peut delivrer de 50k a 100k lux,
      // Donc jai decide de regarder si la valeur en lux ne depasser pas 5k lux
      // si oui c'est surement car le soleil eclaire assee la piece
      if(L_ON && lux >= 5000) digitalWrite(LED, LOW); 
    }
    CTR = 0;

  } else {

    // Si on ne detecte aucun mouvement dans les 15
    // prochaines iterations (secondes) on eteint les lumieres
    // Cela permet d'eviter que si quelqu'un ne bouge pas
    // durant quelque seconde la lumiere s'eteigne alors qu'il est encore present dans la salle
    // NOTE : 15 sec semble raisonnable, sinon 20 sec est beaucoup trop long

    if(L_ON) CTR++;

    if( CTR >= 15 && L_ON ) {
      digitalWrite(LED, LOW);
      CTR = 0;
    } 
  }
  delay(1000);
}
