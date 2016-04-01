# Disco-SmartController
Arduino based Smart Controller for Dagoma Discovery 200 or Melzi printer with Marlin fw

Version: 1.0b

Prérequis:
	- Arduino MEGA 2560
	- 3.2 " TFT touch
	- TFT LCD MEGA Shield V1.0
	- Un adaptateur BT HC-06 (optionnel)
	- Flash IC (sera une option) mais ne peux fonctionner sans si aucune modification dans le code
	- des cables Dupont

Le contenu du fichier Libraries.zip est à copier dans le dossier librarie de l'ide ARDUINO.

Il vous faut changer le port série dans le fichier configuration du firmware Marlin en SERIAL1.
Attention: le fait de changer le port série empêche de controller l'imprimante depuis l'USB de la Melzi,
mais le controle est possible via l'USB de l'Arduino (115200 bps).

Connection du Smart Controller vers la MELZI:
	- RX (melzi) -> TX1 (Arduino)
	- TX (melzi) -> RX1 (Arduino)
	- GND (melzi) -> GND (Arduino)
	
Connextion du module Bluetooth:
	- RX (BT) -> TX2 (Arduino)
	- TX (BT) -> RX2 (Arduino)
	- GND (BT) -> GND (Arduino)
	- VCC (BT) -> 5V

Les fonctionnalités:
	- Affichage de la température (Hotend + Bed)
	- Réglages température, pas de déplacement (0.1, 1, 10, 100mm)
	- Mise en Pause de l'impression
	- Arret d'urgence
	- Connextion Switch (Controller->Melzi, BT->Melzi, USB->Melzi)
	- Déplacement manuel des axes, Homing, filament
	- Marche / Arrêt du ventilateur
	- Control d'un ou plusieurs SERVO (à améliorer)
	
  Fonctionnalités à venir:
	- Affichage et sélection d'un fichier à imprimer sur la SD
	- Affichage du temps restant, et numéro de couche en cours
	- Envoi manuel de G-code, M-code
	- Réglage du SmartController depuis l'écran (variables, diverses et activation/désactivation de fonctionnalités)
	
Attention!!!
	Le SmartController est en cours de développement et je ne pourrai en aucun cas être tenu responsable d'une mauvaise utilisation,
	ou de la présence de bug indésirable.
	
