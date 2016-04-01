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

#include <UTFT.h>
#include <UTouch.h>
#include <TFT_ExtensionV2.h>
#include <SPI.h>
#include <SPIflash.h>
#include <UTFT_SPIflash.h>
#include "Arduino.h"
#include "variables.h" //loading variable setting

//this is for servo z-probe
#include <Servo.h>
Servo Zservo;
bool zstate = false;

void setup(){ 
  SERIAL_P.begin(BAUDRATE); //init printer com dont delete
  SERIAL_U.begin(BAUDRATE); // init usb to printer
  SERIAL_BT.begin(BAUDRATE); //init bt to printer
  
  myFlash.begin();  //image stored in lcd flash ic
  Zservo.attach(9);
  
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  #include "interface.h" //loading customised interface var and function

}

void loop() { 
  if (initcom){
  checkPrinter();
  }

  if (SERIAL_P.readString()== F("zservo")|| Bmc.Touch()){
    if(zstate==false){
      //Zservo.attach(9);
      Zservo.write(90);  //down pos
      delay(500);
      //Zservo.detach();
      zstate = true;
    } else if (zstate==true)  {
      //Zservo.attach(9);
      Zservo.write(150);  //up pos
      delay(500);
      //Zservo.detach();
      zstate=false;
    }
  }
    
  if (ComSet==0){
    Bpr.Colors(RED,RED,NOFILL);
    InitTouchInterface();
  } else if(ComSet==1){
    Bpr.Colors(BLUE,BLUE,NOFILL);
      serialEcho(ComSet);
  } else if(ComSet==2){
    Bpr.Colors(WHITE,WHITE,NOFILL);
      serialEcho(ComSet);
  }

  if(Bpr.Toggle()) { 
      Bpr.SetState(false);
      if( ComSet==0){ ComSet=1; myFiles.loadBitmap(2,0,0);} //activate usb
      else if(ComSet==1){ ComSet=2; myFiles.loadBitmap(3,0,0);} //activate bt
      else if(ComSet==2){ ComSet=0; myGLCD.print(F("          "),0,0);}
      Bpr.Draw();
  } else if (Bst.Touch()){  //arret urgence
     sendGcode(F("M112"));
  }
    if ((millis() - lastUpdate) >= 5000) { //affiche la température toute les 5 secondes
      getTemperatures();
      lastUpdate=millis(); 
    }
}  

void setMvStep(int MvStep){
  if (MvStep == 1){
    mvsval = 0.1;
  } else if (MvStep == 2){
    mvsval = 1;
  } else if (MvStep == 3){
    mvsval = 10;
  } else {
    mvsval = 100;
  }
  //  strMvc = dtostrf(mvsval,3,1,strcMv);
    myGLCD.setBackColor(BLACK);
    myGLCD.print(F("       "),140,210);
    myGLCD.setColor(GREEN);
    myGLCD.print(F("Pas"),140,195);
    myGLCD.print(strMvc+"mm",140,208);
}
void setBedTemp(int temp){
  if (temp == 1){
    sendGcode(F("M140 S0"));
  } else if (temp == 2){
    sendGcode(F("M140 S60"));
  } else if (temp == 3){
    sendGcode(F("M140 S90"));
  } else {
    sendGcode(F("M140 S110"));
  }
}
void setHdTemp(int temp){
    if (temp == 1){
    sendGcode(F("M109 S0"));
  } else if (temp == 2){
    sendGcode(F("M109 S185"));
  } else if (temp == 3){
    sendGcode(F("M109 S210"));
  } else {
    sendGcode(F("M109 S230"));
  }
}
//need to be improved to external file
void checkPrinter(){
  int r = 0;
  while (initcom){ //initialise la communication avec l'imprimante
    SERIAL_P.println(F("M105"));
    if (SERIAL_P.available()) {
      setStateDisplay(F("Initialisation en cours"));
      for (int i = 0; i <= 100; i++) 
      {
        Print_Bar.SetValue(i);
        Print_Bar.Progress(false);
      }
      Print_Bar.SetValue(0);
      Print_Bar.Progress(false);
      setStateDisplay(F("Pret"));
      initcom = false;
    } else {
      if (r<10) {
        setStateDisplay(F("Communication en cours"));
        delay(100);
            r++;
      } else {
        myGLCD.setColor(RED);
        setStateDisplay(F("Communication Impossible"));
        delay(100);
      }
    }
  }
}
void InitTouchInterface(){
    unsigned int Mvslide = moveS.Touch();
    unsigned int Tbslide = tempB.Touch();
    unsigned int Thslide = tempH.Touch();
  if(lastTb != Tbslide) {
    setBedTemp(Tbslide);
    lastTb = Tbslide;
  }
  if(lastHb != Thslide) {
    setHdTemp(Thslide);
    lastHb = Thslide;
  }
  if(lastMv != Mvslide) {
    setMvStep(Mvslide);
    lastMv = Mvslide;
  }
    myGLCD.setColor(WHITE);
    myGLCD.setBackColor(BLACK);
   if (XH.Touch()){           //home X
     sendGcode(F("G28 X"));
   } else if(YH.Touch()) {    //home Y
     sendGcode(F("G28 Y"));
   } else if(ZH.Touch()) {    //home Z
     sendGcode(F("G28 Z"));
   } else if(UB.Touch()) {    //deplacer plateau vers arriere
     sendGcode(F("G91"));
     sendGcode("G1 Y"+strMvc+" F1800");
     sendGcode(F("G90"));
   } else if(DB.Touch()) {    //deplacer plateau vers avant
     sendGcode(F("G91"));
     sendGcode("G1 Y-"+strMvc+" F1800");
     sendGcode(F("G90"));
   } else if(LB.Touch()) {    //deplacer buse vers la gauche
     sendGcode(F("G91"));
     sendGcode("G1 X-"+strMvc+" F1800");
     sendGcode(F("G90"));
   } else if(RB.Touch()) {    //deplacer buse vers la droite
     sendGcode(F("G91"));
     sendGcode("G1 X+"+strMvc+" F1800");
     sendGcode(F("G90"));
   } else if(UZ.Touch()) {    //monter buse
     sendGcode(F("G91"));
     sendGcode("G1 Z"+strMvc+" F1800");
     sendGcode(F("G90"));
   } else if(DZ.Touch()) {    //descendre buse
     sendGcode(F("G91"));
     sendGcode("G1 Z-"+strMvc+" F1800");
     sendGcode(F("G90"));     
   } else if(Eext.Touch()) {  //extrusion filament
     sendGcode(F("G91"));
     sendGcode("G1 E"+strMvc+" F240");
     sendGcode(F("G90"));  
   } else if(Rext.Touch()){   //retracter filament
     sendGcode(F("G91"));
     sendGcode("G1 E-"+strMvc+" F240");
     sendGcode(F("G90"));  
   } else if (Bsd.Touch()) {  // lister les fichiers sur la carte sd
     PSdFilelist();
   } else if ( Fan.Toggle()){
       Fan.SetState(false);
       if (fanS) {
        sendGcode(F("M908"));
        Fan.Colors(RED,RED,NOFILL);
        Fan.Draw();
        fanS = false;
       } else {
        sendGcode(F("M106 S255"));
        Fan.Colors(WHITE,WHITE,NOFILL);
        Fan.Draw();
        fanS = true;
       }
   } else if(Brk.Toggle()){
       Brk.SetState(false);
       if (!Brked) {
        sendGcode(F("M25"));
        Brk.Colors(RED,RED,NOFILL);
        Brk.Draw();
        Brked = true;
       } else {
        sendGcode(F("M24"));
        Brk.Colors(WHITE,WHITE,NOFILL);
        Brk.Draw();
        Brked = false;
       }
   }
   //else if (Bmc.Touch()){
     //myKB.SetupMobileKB(0, 0, 300, 230); //NEEDED TO WORK!
   //}
}
void serialEcho(int scom){ //bypass serial com
  if (scom==1){
    if (SERIAL_U.available() > 0) {
      SERIAL_P.write(SERIAL_U.read());
    }
    if (SERIAL_P.available() > 0) {
      SERIAL_U.write(SERIAL_P.read());
    }
  } else if (scom==2){
    if (SERIAL_BT.available() > 0) {
      SERIAL_P.write(SERIAL_BT.read());
    }
    if (SERIAL_P.available() > 0) {
      SERIAL_BT.write(SERIAL_P.read());
    }
  }
}
void boxSd(){
  myGLCD.setColor(GREY);
  myGLCD.drawRoundRect(20,30,300,210);
}
void PSdFilelist(){
    bool startsd = false;
    String filename = "";
    int file_cnt = 0;
    char f_0[10] ="";
    char f_1[10] ="";
    char f_2[10] ="";
    char f_3[10] ="";
    char f_4[10]=""; 
    char *file[] = {f_0,f_1,f_2,f_3,f_4};
    
     //SERIAL_P.println(F("M21"));
     SERIAL_P.println(F("M20"));
     while (SERIAL_P.available()==0) { }
      myGLCD.setColor(GREY);
      myGLCD.fillRoundRect(20,30,300,210);
     while (SERIAL_P.available()>0){
        filename = SERIAL_P.readStringUntil('\n');
        if (filename != F("End file list") && startsd==true) {  
          if(file_cnt < 5){ //if is GCODE file //(filename.endsWith(".G") || filename.endsWith(".g")) && 
             filename.toCharArray(file[file_cnt],filename.length()+1);
             myGLCD.setBackColor(GREY);
             myGLCD.setColor(BLACK);
             myGLCD.print(file[file_cnt],50,file_cnt*10);
             file_cnt++;
           }
        } else if(filename == F("End file list")){
          startsd =false;
          setStateDisplay(filename);
        } else if (filename == F("Begin file list")) {
          startsd = true;
          setStateDisplay(filename);
        }
     }
     SERIAL_P.read(); //on vide serial buffer 
}
void setStateDisplay(String dispstring){
    clearText(CENTER,225);
    myGLCD.print(dispstring,CENTER,225);    
}
void clearText(int col, int line){
    myGLCD.print(F("                                       "),col,line); 
}
void sendGcode(String lineOfCode){
  SERIAL_P.println(lineOfCode);
  awaitingOK = true;  
  setStateDisplay(lineOfCode);
  delay(10);
  checkForOk();
}
void checkForOk() {
  char c,lastc;
   while (SERIAL_P.available()) {
    c = SERIAL_P.read();    
    if (lastc=='o' && c=='k') {awaitingOK=false; clearText(CENTER,225);}
    lastc=c;
    delay(1);     
    }
}
void getTemperatures() {
      SERIAL_P.println(F("M105"));
      while (SERIAL_P.available()==0) { }
     if( String strTemps = SERIAL_P.readString()){
            // ok T:21.9 /0.0 B:22.1 /0.0 T0:21.9 /0.0 @:0 B@:0 ok T:22.0 /0;
            // Get extruder current temperature
            String a = strTemps.substring(strTemps.indexOf(F("T:"))+2, strTemps.indexOf(F("/")));
            a.trim();
            // Get extruder target temperature
            String b = strTemps.substring(strTemps.indexOf(F("/"))+1, strTemps.indexOf(F("B:")));
            b.trim();
            // Get bed current temperature
            String c = strTemps.substring(strTemps.indexOf(F("B:"))+2, strTemps.indexOf(F("/"), strTemps.indexOf(F("B:"))+2));;
            c.trim();
            // Get bed target temperature
            String d = strTemps.substring(strTemps.indexOf(F("/"), strTemps.indexOf(F("B:")))+1, strTemps.indexOf(F("T0:")));
            d.trim();
            //display temp
            myGLCD.setBackColor(BLACK);
            myGLCD.print(F("               "),0,195);
            myGLCD.print(F("                 "),0,210);
            myGLCD.setColor(RED);
            myGLCD.print("Lit "+c+"/"+d+"~C",0,195);
            myGLCD.setColor(BLUE);
            myGLCD.print("Buse "+a+"/"+b+"~C",0,210);
            //on libère la ram
            a= "";
            b=a;
            c=b;
            d=c;
            }
}
