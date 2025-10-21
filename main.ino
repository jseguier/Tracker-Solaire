// Projet Tracker-Solaire | BUT2 MP 2024/2025
// terme "SRA" = Servomoteur à Rotation Angulaire
// terme "LDR" = Photorésistance

#include <Servo.h>

int LDR1 = A1; // Photorésistance 1 en HAUT/GAUCHE
int LDR2 = A2; // Photorésistance 2 en HAUT/DROITE
int LDR3 = A3; // Photorésistance 3 en BAS/DROITE
int LDR4 = A4; // Photorésistance 4 en BAS/GAUCHE


int LDR1_R = 0; // définit la lecture de la LDR1 à 0
int LDR2_R = 0; // définit la lecture de la LDR2 à 0
int LDR3_R = 0; // définit la lecture de la LDR3 à 0
int LDR4_R = 0; // définit la lecture de la LDR4 à 0

Servo SRA1; // définit le moteur à la base comme "SRA1"
int sra1=0; // définit la position du "SRA1" à 0°
Servo SRA2; // définit le moteur responsable de la rotation du panneau comme "SRA2"
int sra2 = 0; // définit la position du "SRA2" à 0°

int seuil = 50; // seuil de détection que l'on impose aux LDRS. En changeant ce seuil, on peut faire suivre une lumière de téléphone dans un environnement lumineux.
int pas = 1; // déclaration d'un pas pour la rotation des SRAS (servomoteurs à rotation angulaire)

int angle_min_SRA2 = 0;  // limite d'angle min du SRA2, /!\ changer cette limite peut endommager les soudures du tracker
int angle_max_SRA2 = 120; // limite d'angle max du SRA2, /!\ changer cette limite peut endommager les soudures du tracker

void setup() {

  Serial.begin(25000); // Port de communication arduino-ordinateur, /!\ + les bauds seront élévés + le tracker sera réactif (testé expérimentalement)

  SRA1.attach(10); // Attribue SRA1 au pin 10
  SRA2.attach(11); // Attribue SRA2 au pin 11
  delay(500);

}

void loop() {
  
// moyenne de 3 valeurs de chaques LDRS pour limiter le bruit détecté lors de nos essais (3 est arbitraire)
  LDR1_R = (analogRead(LDR1) + analogRead(LDR1) + analogRead(LDR1) ) /3; // moyenne de 3 lectures de la LDR1
  LDR2_R = (analogRead(LDR2) + analogRead(LDR2) + analogRead(LDR2) ) /3; // moyenne de 3 lectures de la LDR2
  LDR3_R = (analogRead(LDR3) + analogRead(LDR3) + analogRead(LDR3) ) /3; // moyenne de 3 lectures de la LDR3
  LDR4_R = (analogRead(LDR4) + analogRead(LDR4) + analogRead(LDR4) ) /3; // moyenne de 3 lectures de la LDR4

// Formatage de l'affichage des valeurs des LDRS dans la console Arduino. Sert également au traitement des données depuis LabVIEW.
  Serial.print(LDR1_R);
  Serial.print(";");
  Serial.print(LDR2_R);
  Serial.print(";");
  Serial.print(LDR3_R);
  Serial.print(";");
  Serial.print(LDR4_R);
  Serial.println();
// Renvoie dans la console Arduino : <LDR1_R>;<LDR2_R>;<LDR3_R>;<LDR4_R>;

  int c_gauche = (LDR1_R + LDR4_R); // somme des capteurs côté gauche du panneau
  int c_droite = (LDR2_R + LDR3_R); // somme des capteurs côté droit du panneau

  int c_haut = (LDR1_R + LDR2_R); // somme des capteurs côté haut du panneau
  int c_bas = (LDR3_R + LDR4_R); // somme des capteurs côté bas du panneau

  int delta_x = ( c_gauche - c_droite ); // delta gauche/droite
  int delta_y = ( c_haut - c_bas ); // delta haut/bas 

  // s'il y a plus de lumière du côté gauche du panneau
  if (delta_x < -seuil && sra1 < 180) { 
    sra1 += pas;  // on tourne vers la gauche avec un pas de 1
    SRA1.write(sra1); // on envoie l'ordre au SRA1 (moteur à la base)
    delay(1);
  }

  // s'il y a plus de lumière du côté droit du panneau
  if (delta_x > seuil && sra1 > 0) {
    sra1 -= pas; // on tourne vers la droite avec un pas de 1
    SRA1.write(sra1); // on envoie l'ordre au SRA1 (moteur à la base)
    delay(1);
  }

  // s'il y a plus de lumière côté haut du panneau
  if (delta_y < -seuil && sra2 < angle_max_SRA2) {
  sra2 += pas; // on s'oriente vers le haut avec un pas de 1
  SRA2.write(sra2); // on envoie l'ordre au SRA2 (moteur responsable de la rotation du panneau)
  delay(1);
  }

  // s'il y a plus de lumière côté bas du panneau
  if (delta_y > seuil && sra2 > angle_min_SRA2) {
    sra2 -= pas; // on s'oriente vers le bas avec un pas de 1
    SRA2.write(sra2); // on envoie l'ordre au SRA2 (moteur responsable de la rotation du panneau)
    delay(1);
  }

  /* /!\ changer ce bloc peut endommager les soudures du tracker /!\ */
  sra2 = constrain(sra2, angle_min_SRA2, angle_max_SRA2); // on impose des limites (max/min) au SRA2
  SRA2.write(sra2); // on renvoie l'ordre au SRA2 (moteur responsable de la rotation du panneau) avec les valeurs des limites
  delay(1);
  
}








