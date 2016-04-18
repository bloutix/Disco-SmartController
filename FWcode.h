/*  Disco-SmartController by Bloutix <bloutix@hotmail.fr>
 *   This program is in developement and it was created for Dagoma Discovery 200
 *   
 *  This program is free software: you can rediMfwibute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is diMfwibuted in the hope that it will be useful,
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
//Marlin 
//sd card
const char Mfw_0[] PROGMEM = "M20"; //list SD card
const char Mfw_1[] PROGMEM = "M21"; //init SD Card
const char Mfw_2[] PROGMEM = "M23 "; //select a file
const char Mfw_3[] PROGMEM = "M24"; //start/resume sd print
const char Mfw_4[] PROGMEM = "M25"; //Pause SD print
const char Mfw_5[] PROGMEM = "M26"; //Set SD position in Byte after M23 then M24
const char Mfw_6[] PROGMEM = "M27"; //print status in byte
const char Mfw_7[] PROGMEM = "M28"; //start SD write
const char Mfw_8[] PROGMEM = "M29"; //stop SD write
const char Mfw_9[] PROGMEM = "M31"; //display elapsed time
//Hardware control
const char Mfw_10[] PROGMEM = "M112"; //Emergency Stop need to reset the printer after this
const char Mfw_11[] PROGMEM = "M410"; //QuickStop abort all, stop the printer
//Temp and Fan
const char Mfw_12[] PROGMEM = "M104 S"; //set extruder temp
const char Mfw_13[] PROGMEM = "M105"; //request all temp to display
const char Mfw_14[] PROGMEM = "M106 S255"; //Set fan speed
const char Mfw_15[] PROGMEM = "M107"; //stop fan default Marlin
const char Mfw_16[] PROGMEM = "M908"; //stop fan by Dagoma
const char Mfw_17[] PROGMEM = "M140 S"; //set Bed Temp
//misc
const char Mfw_18[] PROGMEM = "M114"; //display current position 
const char Mfw_19[] PROGMEM = "G1 X-"; //move to left
const char Mfw_20[] PROGMEM = "G1 X"; //move to right
const char Mfw_21[] PROGMEM = "G28 X"; //home X
const char Mfw_22[] PROGMEM = "G1 Y"; //move to front
const char Mfw_23[] PROGMEM = "G1 Y-"; //move to back
const char Mfw_24[] PROGMEM = "G28 Y"; //home Y
const char Mfw_25[] PROGMEM = "G1 Z-"; //move up
const char Mfw_26[] PROGMEM = "G1 Z"; //move down
const char Mfw_27[] PROGMEM = "G28 Z"; //home Z
const char Mfw_28[] PROGMEM = "G1 E-"; //retract filament
const char Mfw_29[] PROGMEM = "G1 E"; //extrude filment
const char Mfw_30[] PROGMEM = "G90"; //absolute coordinate
const char Mfw_31[] PROGMEM = "G91"; //relative coordinate
const char Mfw_32[] PROGMEM = " F15000"; //move speed for axes 250mm/s
const char Mfw_33[] PROGMEM = " F240";  //speed for extrusion

const char* const Mfw_table[] PROGMEM = {
  Mfw_0, Mfw_1, Mfw_2, Mfw_3, Mfw_4, Mfw_5, Mfw_6, Mfw_7, Mfw_8, Mfw_9, Mfw_10,
  Mfw_11, Mfw_12, Mfw_13, Mfw_14, Mfw_15, Mfw_16, Mfw_17, Mfw_18, Mfw_19, Mfw_20,
  Mfw_21, Mfw_22, Mfw_23, Mfw_24, Mfw_25, Mfw_26, Mfw_27, Mfw_28, Mfw_29, Mfw_30,
  Mfw_31, Mfw_32, Mfw_33
};

char Mfw_buf[10];    // make sure this is large enough for the largest string it must hold
