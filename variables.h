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
 
//all pre-setup variables
//#define imagedatatype  unsigned short
extern uint8_t SmallFont[];
UTFT myGLCD(ITDB32S,38,39,40,41);
UTouch  myTouch(6,5,4,3,2);
SPIflash      myFlash(45);//45
UTFT_SPIflash myFiles(&myGLCD,&myFlash);

// Not touch is for TFT_extensionV2 button lib
Base B(&myGLCD,&myTouch); // Base class, NEEDED!!!

//MobileKeyboard myKB(&B);

//initialize slider
Slider moveS(&B,VERTICAL);
Slider tempH(&B,VERTICAL);
Slider tempB(&B,VERTICAL);

Meter Print_Bar(&B,VERTICAL);

#define SERIAL_U Serial
#define SERIAL_P Serial1   //port serie smartcontroller -> printer
#define SERIAL_BT Serial2   //port serie smartcontroller -> bt module
#define BAUDRATE 115200

unsigned int disp_pg =0;
long lastUpdate = 0;
bool awaitingOK = false;
bool initcom = true;
bool fanS = true;
bool Brked = false;
bool loaded = false;

//this is for SD file list
int file_cnt = 0;
String file[] = {"","","","",""};
int selected = 0;

char strcMv[15];
float mvsval = 0.1; //desired mv step
String strMvc = "0.1";

//var of slide control
unsigned int lastTb,lastHb,lastMv;

//define home button
Box TCHB(&B), BTB(&B), USBB(&B), SETB(&B), BACK(&B), SDPTB(&B);
//define all custom button
Box XH(&B), YH(&B), ZH(&B), UB(&B), DB(&B), LB(&B), RB(&B), UZ(&B), DZ(&B);
Box Eext(&B), Rext(&B), Fan(&B), Bsd(&B), Bpr(&B), Brk(&B), Bst(&B);
Box Bmc(&B),UPB(&B),DNB(&B);
