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
/*#include <Servo.h>
Servo Zservo;
bool zstate = false;*/

void setup(){ 
  SERIAL_P.begin(BAUDRATE); //init printer com dont delete
  SERIAL_U.begin(BAUDRATE); // init usb to printer
  SERIAL_BT.begin(BAUDRATE); //init bt to printer
  
  myFlash.begin();  //image stored in lcd flash ic
  //Zservo.attach(9);
  
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  TCHB.Coords(20,60,70,110);
  BTB.Coords(90,60,140,110);
  USBB.Coords(20,130,70,180);
  SETB.Coords(90,130,140,180);
  TCHB.Colors(GREEN,WHITE,NOFILL,SQUARED);
  BTB.Colors(GREEN,WHITE,NOFILL,SQUARED);
  USBB.Colors(GREEN,WHITE,NOFILL,SQUARED);
  SETB.Colors(GREEN,WHITE,NOFILL,SQUARED);
  BACK.Coords(0,219,20,239);
  BACK.Colors(GREEN,WHITE,NOFILL,SQUARED);
}

void loop() { 
  /*if (initcom){
  checkPrinter();
  }*/

  switch(disp_pg){
    case 0:
      disp_HOM();
    break;
    case 1:
      disp_TCH();
    break;
    case 2:
      disp_BT();
    break;
    case 3:
      disp_USB();
    break;
    case 4:
      disp_SET();
    break;
    case 5:
      disp_SD();
    break;
  }

}  

void disp_HOM(){  
  if (loaded == false) {
  myFiles.loadBitmap(4,160,45);
  myFiles.loadBitmap(9,20,60);
  myFiles.loadBitmap(6,90,130);
  myFiles.loadBitmap(7,90,60);
  myFiles.loadBitmap(8,20,130);
  myGLCD.setColor(CYAN);
  myGLCD.drawRoundRect(0,224,319,239);
  myGLCD.print(F("Developped by Bloutix"),CENTER,225);
  myGLCD.setColor(BLACK);
  myGLCD.fillRect(0,0,319,10);
  myGLCD.setBackColor(BLACK);
  myGLCD.setColor(CYAN);
  myGLCD.print(F("Disco SmartController"),CENTER,0);
  loaded = true;
  }
  TCHB.ReDraw();
  BTB.ReDraw();
  USBB.ReDraw();
  SETB.ReDraw();
  if (TCHB.Touch()){
    loaded = false;
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    disp_pg = 1;
  } else if(BTB.Touch()){
    loaded = false;
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    disp_pg = 2;
  } else if(USBB.Touch()){
    loaded = false;
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    disp_pg = 3;
  } else if(SETB.Touch()){
    loaded = false;
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    disp_pg = 4;
  }
}
void disp_TCH(){
  if (loaded == false) {
  myFiles.loadBitmap(5,0,219);
  #include "interface.h" //loading customised interface var and function
  moveS.Unlock();
  tempB.Unlock();
  tempH.Unlock();
  Print_Bar.Unlock();
  moveS.SetStartingValue(lastMv);
  tempB.SetStartingValue(lastTb);
  tempH.SetStartingValue(lastHb);
  moveS.Draw();
  tempB.Draw();
  tempH.Draw();
  Print_Bar.Draw();
  loaded = true;
  }
  /*  if (SERIAL_P.readString()== F("zservo")|| Bmc.Touch()){
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
  }*/
    
  InitTouchInterface();

  if (Bst.Touch()){  //arret urgence
     sendGcode(F("M112"));
  }
    /*if ((millis() - lastUpdate) >= 5000) { //affiche la température toute les 5 secondes
      getTemperatures();
      lastUpdate=millis(); 
    }*/
  if (BACK.Touch()){
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.print(F("        "),0,0);
    disp_pg =0;
    loaded = false;
  }
}
void disp_SET(){
  if (loaded == false) {
  myFiles.loadBitmap(5,0,219);//back btn
  myGLCD.setBackColor(GREY);
  myGLCD.setColor(BLUE);
  myGLCD.print(F("Parametres"),CENTER,15);
  loaded = true;
  }
  if (BACK.Touch()){
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.print(F("        "),0,0);
    disp_pg =0;
    loaded = false;
  }
}
void disp_USB(){
  if (loaded == false) {
  myFiles.loadBitmap(2,0,0);
  myFiles.loadBitmap(5,0,219);
  myGLCD.setColor(GREY);
  myGLCD.fillRect(50,105,270,125);
  myGLCD.setColor(RED);
  myGLCD.drawRoundRect(50,105,270,125);
  myGLCD.setBackColor(GREY);
  myGLCD.print(F("Controle USB en cours"),CENTER,110);
  loaded =true;
  }
  
  if (BACK.Touch()){
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.print(F("        "),0,0);
    disp_pg =0;
    loaded = false;
  } else {
    serialEcho(1);
  }
}
void disp_BT(){
  if (loaded == false) {
  myFiles.loadBitmap(3,0,0);
  myFiles.loadBitmap(5,0,219);
  myGLCD.setColor(GREY);
  myGLCD.fillRect(50,105,270,125);
  myGLCD.setColor(BLUE);
  myGLCD.drawRoundRect(50,105,270,125);
  myGLCD.setBackColor(GREY);
  myGLCD.print(F("Controle Bluetooth en cours"),CENTER,110);
  loaded =true;
  }
  
  if (BACK.Touch()){
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.print(F("        "),0,0);
    disp_pg =0;
    loaded = false;
  } else {
    serialEcho(2);
  }
}
void disp_SD(){
  if (loaded == false) {
  myFiles.loadBitmap(5,0,219);
  myGLCD.setBackColor(GREY);
  myGLCD.setColor(BLUE);
  myGLCD.print(F("Liste des fichiers sur la Carte SD"),CENTER,15);
  PSdFilelist();
  SDPTB.Coords(220,180,300,210);
  SDPTB.Colors(CYAN,GREEN,FILL,SQUARED);
  SDPTB.ReDraw();
  UPB.Coords(260,50,300,90);
  UPB.Colors(GREEN,RED,FILL,SQUARED);
  UPB.ReDraw();
  DNB.Coords(260,110,300,150);
  DNB.Colors(GREEN,BLUE,FILL,SQUARED);
  DNB.ReDraw();
  myGLCD.setColor(BLUE);
  myGLCD.print(F("IMPRIMER"),230,190);
  for (byte i=0; i<file_cnt; i++){
    if(i==0){
      myGLCD.setColor(GREEN);
      myGLCD.drawRect(45,(i*20)+38,190,(i*20)+52);
    }
    myGLCD.setBackColor(GREY);
    myGLCD.setColor(WHITE);
    myGLCD.print(file[i],50,(i*20)+40);
  }
  loaded =true;
  }
  if(DNB.Touch()){
    myGLCD.setColor(GREY);
    myGLCD.drawRect(45,(selected*20)+38,190,(selected*20)+52);
    selected++;
    if(selected>file_cnt){selected=file_cnt;}
    myGLCD.setColor(GREEN);
    myGLCD.drawRect(45,(selected*20)+38,190,(selected*20)+52);
    delay(50);
  }else if(UPB.Touch()) {
    myGLCD.setColor(GREY);
    myGLCD.drawRect(45,(selected*20)+38,190,(selected*20)+52);
    selected--;
    if(selected<0){selected=0;}
    myGLCD.setColor(GREEN);
    myGLCD.drawRect(45,(selected*20)+38,190,(selected*20)+52);
    delay(50);
  }else if(SDPTB.Touch()){
    sendGcode("M23 "+file[selected]);
    memset(file,0,sizeof(file));
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.print(F("        "),0,0);
    disp_pg=1;
    loaded = false;
  }
  if (BACK.Touch()){
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.print(F("        "),0,0);
    disp_pg =1;
    loaded = false;
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
    strMvc = dtostrf(mvsval,3,1,strcMv);
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
      myGLCD.setColor(BLACK);
      myGLCD.fillRect(0,10,319,239);
      loaded= false;
      disp_pg = 5;
   } else if ( Fan.Toggle()){
       Fan.SetState(false);
       if (fanS) {
        sendGcode(F("M908"));
        Fan.Colors(RED,RED,NOFILL);
        Fan.ReDraw();
        fanS = false;
       } else {
        sendGcode(F("M106 S255"));
        Fan.Colors(WHITE,WHITE,NOFILL);
        Fan.ReDraw();
        fanS = true;
       }
   } else if(Brk.Toggle()){
       Brk.SetState(false);
       if (!Brked) {
        sendGcode(F("M25"));
        Brk.Colors(RED,RED,NOFILL);
        Brk.ReDraw();
        Brked = true;
       } else {
        sendGcode(F("M24"));
        Brk.Colors(WHITE,WHITE,NOFILL);
        Brk.ReDraw();
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
void PSdFilelist(){
    bool startsd = false;
    file_cnt = 0;
    String filename = "";
     myGLCD.setColor(GREY);
     myGLCD.fillRoundRect(20,30,300,210);
      
     SERIAL_P.read(); //on vide serial buffer
     SERIAL_P.println(F("M21"));
     SERIAL_P.read(); //on vide serial buffer
     SERIAL_P.println(F("M20"));
     while (SERIAL_P.available()==0) { }
     while (SERIAL_P.available()>0){
        delay(10);
        filename = SERIAL_P.readStringUntil('\n');
        if (filename != F("End file list") && startsd==true) {  
          if(file_cnt < 5){ //if is GCODE file //(filename.endsWith(".G") || filename.endsWith(".g")) && 
             //filename.toCharArray(file[file_cnt],filename.length()+1);
             file[file_cnt] = filename;
             file_cnt++;
           }
        } else if(filename == F("End file list")){
          myGLCD.setBackColor(GREY);
          myGLCD.setColor(CYAN);
          setStateDisplay(filename);
          startsd =false;
          filename="";
        } else if (filename == F("Begin file list")) {
          myGLCD.setBackColor(GREY);
          myGLCD.setColor(CYAN);
          setStateDisplay(filename);
          startsd = true;
        }
     }
     SERIAL_P.read(); //on vide serial buffer 
}
void setStateDisplay(String dispstring){
    clearText(CENTER,225);
    myGLCD.print(dispstring,CENTER,225);    
}
void clearText(int col, int line){
    myGLCD.print(F("                                  "),col,line); 
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
