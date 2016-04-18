/*  Disco-SmartController by Bloutix <bloutix@hotmail.fr>
 *   This program is in developement and it was created for Dagoma Discovery 200
 *   
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  Version: 1.0b
 *  Hardware: 
 *  - Arduino MEGA2560, TFT 320QVT or other 3.2 tft touch screen 320x240
 *  - TFT LCD Mega Shield V1.0 moded (added SPI flash memory wire) (will be optionnal)
 *  - Winbond SPI flash IC for storing 
 *  - Bt device: HC-06
 *  - some wire
 */

const char str_0[] PROGMEM = "Page de configuration"; 
const char str_1[] PROGMEM = "Disco SmartController";
const char str_2[] PROGMEM = "Developped by Bloutix";
const char str_3[] PROGMEM = "PROBLEME DE CONNEXION";
const char str_4[] PROGMEM = "Langue:";
const char str_5[] PROGMEM = "FR";
const char str_6[] PROGMEM = "EN";
const char str_7[] PROGMEM = "Dagoma:";
const char str_8[] PROGMEM = "Bluetooth:";
const char str_9[] PROGMEM = "Lit Chauffant:";
const char str_10[] PROGMEM = "Flash IC:";
const char str_11[] PROGMEM = "Vitesse de communication";
const char str_12[] PROGMEM = "Nb max fichier SD:";
const char str_13[] PROGMEM = "TFT SD:";
const char str_14[] PROGMEM = "Double Buse:";
const char str_15[] PROGMEM = "Buse > 255~C ?";
const char str_16[] PROGMEM = "Temperatures de la Buse";
const char str_17[] PROGMEM = "Temperatures du lit chauffant";
const char str_18[] PROGMEM = "VALIDER";
const char str_19[] PROGMEM = "Suivant";
const char str_20[] PROGMEM = "ACCUEIL";
const char str_21[] PROGMEM = "Les parametres sont bien mis a jour.";
const char str_22[] PROGMEM = "Controle USB en cours";
const char str_23[] PROGMEM = "Controle Bluetooth en cours";
const char str_24[] PROGMEM = "Liste des fichiers sur la Carte SD";
const char str_25[] PROGMEM = "ok";
const char str_26[] PROGMEM = "Lit ";
const char str_27[] PROGMEM = "Buse ";
const char str_28[] PROGMEM = "Impression Terminee";
const char str_29[] PROGMEM = "Imprimante: ";
const char str_30[] PROGMEM = "Vitesse: ";
const char str_31[] PROGMEM = "Maxi SD: ";
const char str_32[] PROGMEM = "IMPORTANT: Verifiez bien vos parametres";
const char str_33[] PROGMEM = "avant de les valider ! ! !";
const char str_34[] PROGMEM = "Oui";
const char str_35[] PROGMEM = "Non";
const char str_36[] PROGMEM = "zservo";
const char str_37[] PROGMEM = "Pas";
const char str_38[] PROGMEM = "mm";

/*const char str_40[] PROGMEM = "Setting Page";
const char str_41[] PROGMEM = "Disco SmartController";
const char str_42[] PROGMEM = "Developped by Bloutix";
const char str_43[] PROGMEM = "COMMUNICATING ERROR";
const char str_44[] PROGMEM = "Language:";
const char str_45[] PROGMEM = "FR";
const char str_46[] PROGMEM = "EN";
const char str_47[] PROGMEM = "Dagoma:";
const char str_48[] PROGMEM = "Bluetooth:";
const char str_49[] PROGMEM = "Hotbed:";
const char str_50[] PROGMEM = "Flash IC:";
const char str_51[] PROGMEM = "COM Speed";
const char str_52[] PROGMEM = "Max SD File:";
const char str_53[] PROGMEM = "TFT SD:";
const char str_54[] PROGMEM = "Dual Extruder:";
const char str_55[] PROGMEM = "Hotend > 255~C ?";
const char str_56[] PROGMEM = "Hotend Temp";
const char str_57[] PROGMEM = "Hotbed Temp";
const char str_58[] PROGMEM = "Apply";
const char str_59[] PROGMEM = "Next";
const char str_60[] PROGMEM = "HOME";
const char str_61[] PROGMEM = "Settings are updated.";
const char str_62[] PROGMEM = "USB controlling";
const char str_63[] PROGMEM = "Bluetooth controlling";
const char str_64[] PROGMEM = "SD card Files List";
const char str_65[] PROGMEM = "ok";
const char str_66[] PROGMEM = "Bed ";
const char str_67[] PROGMEM = "Hotend ";
const char str_68[] PROGMEM = "Print finished";
const char str_69[] PROGMEM = "Printer: ";
const char str_70[] PROGMEM = "Speed: ";
const char str_71[] PROGMEM = "Max SD: ";
const char str_72[] PROGMEM = "CAUTION: Check your settings";
const char str_73[] PROGMEM = "before applying ! ! !";
const char str_74[] PROGMEM = "Yes";
const char str_75[] PROGMEM = "No";
const char str_76[] PROGMEM = "zservo";
const char str_77[] PROGMEM = "Step";
const char str_78[] PROGMEM = "mm";*/

// Then set up a table to refer to your strings.
const char* const strFR_table[] PROGMEM = {
  str_0, str_1, str_2, str_3, str_4, str_5, str_6, str_7, str_8, str_9, str_10,
  str_11, str_12, str_13, str_14, str_15, str_16, str_17, str_18, str_19, str_20,
  str_21, str_22, str_23, str_24, str_25, str_26, str_27, str_28, str_29, str_30,
  str_31, str_32, str_33, str_34, str_35, str_36, str_37, str_38
};
/*const char* const strEN_table[] PROGMEM = {
  str_40, str_41, str_42, str_43, str_44, str_45, str_46, str47, str_48, str_49, str_50,
  str_51, str_52, str_53, str_54, str_55, str_56, str57, str_58, str_59, str_60,
  str_61, str_62, str_63, str_64, str_65, str_66, str67, str_68, str_69, str_70,
  str_71, str_72, str_73, str_74, str_75, str_76, str77, str_78
};*/
char str_buf[50];    // make sure this is large enough for the largest string it must hold
char strc_buf[10];    // make sure this is large enough for the largest string it must hold
