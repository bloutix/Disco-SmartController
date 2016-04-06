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

  TCHB.Coords(20,60,68,108);
  BTB.Coords(90,60,138,108);
  USBB.Coords(20,130,68,178);
  SETB.Coords(90,130,138,178);
  TCHB.Colors(GREEN,WHITE,NOFILL,SQUARED);
  BTB.Colors(GREEN,WHITE,NOFILL,SQUARED);
  USBB.Colors(GREEN,WHITE,NOFILL,SQUARED);
  SETB.Colors(GREEN,WHITE,NOFILL,SQUARED);
  BACK.Coords(0,220,20,239);
  BACK.Colors(GREEN,BLACK,NOFILL,SQUARED);
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
    case 6:
      disp_Mcode();
    break;
  }

}  
//PAGES d'affichage
void disp_Mcode(){  //afichage du clavier pour envoie de ligne de commande
  
}
void disp_HOM(){  //affichage de l'écran d'accueil
  if (loaded == false) {
  myGLCD.setColor(CYAN);
  myGLCD.drawRoundRect(0,224,319,239);
  myGLCD.print(F("Developped by Bloutix"),CENTER,225);
  myGLCD.setColor(BLACK);
  myGLCD.fillRect(0,0,319,10);
  myGLCD.setBackColor(BLACK);
  myGLCD.setColor(CYAN);
  myGLCD.print(F("Disco SmartController"),CENTER,0);
  TCHB.ReDraw();
  BTB.ReDraw();
  USBB.ReDraw();
  SETB.ReDraw();
  myFiles.loadBitmap(4,160,45);
  myFiles.loadBitmap(9,20,60);
  myFiles.loadBitmap(6,90,130);
  myFiles.loadBitmap(7,90,60);
  myFiles.loadBitmap(8,20,130);
  loaded = true;
  }

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
void disp_TCH(){  //affichage de l'interface de controle tactil
  if (loaded == false) {
  #include "interface.h" //loading customised interface var and function
  myFiles.loadBitmap(5,0,219);
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
void disp_SET(){  //affichage de la fenetre de parametrage
  if (loaded == false) {
        myGLCD.setColor(BLACK);
        myGLCD.fillRect(0,10,319,239);
        myGLCD.setBackColor(BLACK);
        myGLCD.setColor(RED);
        myGLCD.print(F("Page de configuration"),CENTER,15);
        SDPTB.Coords(245,220,319,239);
        SDPTB.Colors(BLACK,FILL,SQUARED);
        myFiles.loadBitmap(5,0,219);//back btn
        sub_loaded = false;
        loaded = true;
      }
  switch (sub_pg){
    case 0: //pge lanques, bt, bed, dago, flash ic
      if (sub_loaded == false) {
          rstBtn(0);
          SDPTB.Text("Suivant",WHITE,Small);
          SDPTB.ReDraw();
          myGLCD.setColor(WHITE);
          myGLCD.print(F("Langue:"),20,40);
          myGLCD.print(F("FR"),120,40);
          myGLCD.print(F("EN"),170,40);
          myGLCD.print(F("Dagoma:"),20,70);
          myGLCD.print(F("Bluetooth:"),20,100);
          myGLCD.print(F("Lit Chauffant:"),20,130);
          myGLCD.print(F("Flash IC:"),20,160);
        sub_loaded = true;
      }
        myRB.RadioButtons(Chkbox);
        Bt3.Toggle();
        Bt4.Toggle();
        Bt5.Toggle();
        Bt6.Toggle();
        if(SDPTB.Touch()){
          bitWrite(addr[0],0,((Bt1.getState()) ? 0 : 1)); //langue FR/EN
          bitWrite(addr[0],4,((Bt3.getState()) ? 1 : 0)); //Marlin FW Dagoma / !Dagoma
          bitWrite(addr[0],1,((Bt4.getState()) ? 1 : 0)); //Bluetooth
          bitWrite(addr[0],2,((Bt5.getState()) ? 1 : 0)); //BED
          bitWrite(addr[0],3,((Bt6.getState()) ? 1 : 0)); //Flash IC 
        
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          sub_pg = 1;
          sub_loaded = false;
        } else if (BACK.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,10,319,239);
          disp_pg =0;
          sub_loaded = false;
          loaded = false;
        }
    break;
    case 1: //vitesse com, sd files
      if (sub_loaded == false) {
      rstBtn(1);
      SDPTB.Text("Suivant",WHITE,Small);
      SDPTB.ReDraw();
      myGLCD.setColor(WHITE);
      myGLCD.print(F("Vitesse de communication"),20,40);
      myGLCD.print(F("Nb max fichier SD:"),20,100);
      myGLCD.print(F("TFT SD:"),20,130);
      myGLCD.print(F("Dual Extruder:"),20,160);
      bps = addr[0]>>5;
      par1 = (addr[1]<<3);
      par1 = par1>>3;
      myGLCD.printNumI(par1,170,100, 0);
      String strval = spdVal(bps);
      myGLCD.print(strval,100,70);
      sub_loaded = true;
      }
    if (Bt1.Touch()){
        delay(30);
      if(bps>1)
        bps--;
    } else if(Bt2.Touch()){
      delay(30);
      if(bps<6)
        bps++;
    } else if(Bt3.Touch()){
      delay(30);
      if(par1 > 1)
        par1--;
    } else if(Bt4.Touch()){
        delay(30);
      if(par1 < 25)
        par1++;
    }
        updispval(par1,170,100, 0);
        updispval(bps,100,70, 1);
        Bt5.Toggle();
        Bt6.Toggle();
        if(SDPTB.Touch()){
      speed2byte(bps);// ecrit comspeed dans le byte 0
      sdf2byte(par1); //ecrit nb sd file dans byte 1
      bitWrite(addr[1],5,((Bt5.getState()) ? 1 : 0)); //TFT SD
      bitWrite(addr[1],7,((Bt6.getState()) ? 1 : 0)); //DUAl EXTRUDER
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          sub_pg = 2;
          sub_loaded = false;
        } else if (BACK.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          sub_pg =0;
          sub_loaded = false;
        }
    break;
    case 2: // temp and case sup 255°
      if (sub_loaded == false) {
        rstBtn(2);
        myGLCD.setColor(WHITE);
        myGLCD.print(F("Buse > 255~ ?"),20,40);
        myGLCD.print(F("Temperatures de la Buse"),20,70);
        myGLCD.print(F("T1"),40,80);myGLCD.print(F("T2"),120,80);myGLCD.print(F("T3"),200,80);
        myGLCD.print(F("Temperatures du lit chauffant"),20,140);
        myGLCD.print(F("T1"),40,150);myGLCD.print(F("T2"),120,150);myGLCD.print(F("T3"),200,150);
        SDPTB.Text("Suivant",WHITE,Small);
        SDPTB.ReDraw();
        sub_loaded = true;
      }
        (Bt1.Toggle())? bitWrite(addr[1],6,1):bitWrite(addr[1],6,0);
        
        if(SDPTB.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          sub_pg = 3;
          sub_loaded = false;
        } else if (BACK.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          sub_pg =1;
          sub_loaded = false;
        }
    break;
    case 3: //resume and validate
      if (sub_loaded == false) {
        myGLCD.setColor(BLACK);
        SDPTB.Text("VALIDER",WHITE,Small);
        SDPTB.ReDraw();
        sub_loaded = true;
      }
        if(SDPTB.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          sub_pg = 4;
          sub_loaded = false;
        } else if (BACK.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          SDPTB.Text("Suivant",WHITE,Small);
          SDPTB.ReDraw();
          sub_pg =2;
          sub_loaded = false;
        }
    break;
    case 4: //do eeprom update
      if (sub_loaded == false) {
        SDPTB.Text("ACCUEIL",WHITE,Small);
        SDPTB.ReDraw();
        myGLCD.setColor(WHITE);
        myGLCD.print(F("Les parametres sont bien mis a jour."),CENTER,115);
        myGLCD.setColor(BLACK);
        myGLCD.fillRect(0,220,30,239);
        sub_loaded = true;
      }
      if (SDPTB.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,10,319,239);
          SDPTB.Text("",WHITE,Small);
          sub_pg =0;
          disp_pg = 0;
          sub_loaded = false;
          loaded = false;
        }
    break;
  }
}
void disp_USB(){  //affichage de la fenetre de communication usb
  if (loaded == false) {
  myFiles.loadBitmap(2,0,0);
  myFiles.loadBitmap(5,0,219);
  myGLCD.setColor(RED);
  myGLCD.drawRoundRect(50,105,270,125);
  myGLCD.setBackColor(BLACK);
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
void disp_BT(){   //affichage de la fenetre de communication bluetooth
  if (loaded == false) {
  myFiles.loadBitmap(3,0,0);
  myFiles.loadBitmap(5,0,219);
  myGLCD.setColor(BLUE);
  myGLCD.drawRoundRect(50,105,270,125);
  myGLCD.setBackColor(BLACK);
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
void disp_SD(){   //affichage de la liste des fichiers sur la carte sd
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
  myGLCD.print(F("VALIDER"),230,190);
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

/*Fonctions principales*/
//fonctions liées au SLIDER
void setMvStep(int MvStep){ //fonction de parametrage des mouvements en mm
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
void setBedTemp(int temp){  //fonction de parametrage de la température du lit chauffant
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
void setHdTemp(int temp){   //fonction de parametrage de la température de la buse
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

//fonctions de communication
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
void checkPrinter(){    //fonction de controle de la communication DESACTIVE
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
void sendGcode(String lineOfCode){    //envoie de Gcode avec attente de msg OK
  SERIAL_P.println(lineOfCode);
  awaitingOK = true;  
  setStateDisplay(lineOfCode);
  delay(10);
  checkForOk();
}
void checkForOk() {   //fonction d'attente de MSG_OK
  char c,lastc;
   while (SERIAL_P.available()) {
    c = SERIAL_P.read();    
    if (lastc=='o' && c=='k') {awaitingOK=false; clearText(CENTER,225);}
    lastc=c;
    delay(1);     
    }
}

//fonctions affichage et controle
void InitTouchInterface(){  //variables et fonctions de l'interface tactile
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
   } else if(Bpr.Touch()){
      sendGcode(F("M24"));
   } else if (Bmc.Touch()){
      loaded = false;
      myGLCD.setColor(BLACK);
      myGLCD.fillRect(0,10,319,239);
      disp_pg = 6;
      //myKB.SetupMobileKB(0, 0, 300, 230); //NEEDED TO WORK!
   }
}
void PSdFilelist(){ //initialisation et liste les fichier de la carte sd
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
void setStateDisplay(String dispstring){  //affichage d'information dans le cadre du bas de l'écran
    clearText(CENTER,225);
    myGLCD.print(dispstring,CENTER,225);    
}
void clearText(int col, int line){    //lié à la fonction précédente
    myGLCD.print(F("                                  "),col,line); 
}

//fonctions affichage de valeurs dynamiques/cycliques
void getTemperatures() {    //fonction affichage des température
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
void getPrintState(){ //affiche le pourcentage, le temps estimé de l'impression
  //M31 echo:54 min, 38 sec //récupère le temps écoulé (reply)
  //M27 SD printing byte 2134/235422 récupère le numéro de ligne en cours (reply)
  //M26 Filename S2134 imprime depuis la ligne x du fichier en byte  (cmd with ok reply)
}

//fonctions de pramétrage
//arduino mega eeprom = 4kb or maybe must to use flash ic for storing data
void updispval(int val, int x, int y, byte nset){   //affichage des valeur dynamique de reglage
  
  switch(nset){
    case 0:   //sd files int value is val to print
    int val2;
      val2 = val;
        if(val!=lval[0]){
          lval[0]=val;
          chg[0] = true;
        }
        if(chg[0]){
          myGLCD.setBackColor(WHITE);
          myGLCD.setColor(BLACK);
          myGLCD.print(F("   "),x,y);
          myGLCD.printNumI(val2,x,y);
          chg[0] = false;
        }
    break;
    case 1: //comspeed int value is converted val to print
      if(val!=lval[1]){
          lval[1]=val;
          chg[1] = true;
        }
        if(chg[1]){
          myGLCD.setBackColor(WHITE);
          myGLCD.setColor(BLACK);
          myGLCD.print(F("      "),x,y);
          String strval = spdVal(val);
          myGLCD.print(strval,x,y);
          chg[1] = false;
        }
    break;

  }

}
String spdVal(byte val){
  String valb;
  if(val==1) valb="9600";
    if(val==2) valb="14400";
    if(val==3) valb="19200";
    if(val==4) valb="38400";
    if(val==5) valb="57600";
    if(val==6) valb="115200";
  return valb;
}
void rstBtn(byte pg){   //reset btn  for saving SRAM
  switch(pg){
    case 0:
      Bt1.SetState(bitRead(addr[0],0));Bt2.SetState((bitRead(addr[0],0))?1:0);Bt3.SetState(bitRead(addr[0],4));
      Bt4.SetState(bitRead(addr[0],1));Bt5.SetState(bitRead(addr[0],2));Bt6.SetState(bitRead(addr[0],3));
      Bt1.Coords(140,35,160,55); Bt1.Colors(GREEN,BLACK,SQUARED,FILL); Bt1.Text("", BLACK,Big); Bt1.ReDraw();
      Bt2.Coords(190,35,210,55); Bt2.Colors(GREEN,BLACK,SQUARED,FILL); Bt2.Text("", BLACK,Big); Bt2.ReDraw();
      Bt3.Coords(140,65,160,85); Bt3.Colors(GREEN,BLACK,SQUARED,FILL); Bt3.Text("", BLACK,Big); Bt3.ReDraw();
      Bt4.Coords(140,95,160,115); Bt4.Colors(GREEN,BLACK,SQUARED,FILL); Bt4.Text("", BLACK,Big); Bt4.ReDraw();
      Bt5.Coords(140,125,160,145); Bt5.Colors(GREEN,BLACK,SQUARED,FILL); Bt5.ReDraw();
      Bt6.Coords(140,155,160,175); Bt6.Colors(GREEN,BLACK,SQUARED,FILL); Bt6.ReDraw();
    break;
    case 1:
      Bt5.SetState(bitRead(addr[1],5));Bt6.SetState(bitRead(addr[1],7));
      Bt1.Coords(220,35,240,55); Bt1.Colors(RED,BLACK,SQUARED,FILL); Bt1.SetState(false); Bt1.Text("-", WHITE,Big); Bt1.ReDraw();
      Bt2.Coords(260,35,280,55); Bt2.Colors(BLUE,BLACK,SQUARED,FILL); Bt2.SetState(false); Bt2.Text("+", WHITE,Big); Bt2.ReDraw();
      Bt3.Coords(220,95,240,115); Bt3.Colors(RED,BLACK,SQUARED,FILL); Bt3.SetState(false); Bt3.Text("-", WHITE,Big); Bt3.ReDraw();
      Bt4.Coords(260,95,280,115); Bt4.Colors(BLUE,BLACK,SQUARED,FILL); Bt4.SetState(false); Bt4.Text("+", WHITE,Big); Bt4.ReDraw();
      Bt5.Coords(220,125,240,145); Bt5.Colors(GREEN,BLACK,SQUARED,FILL); Bt5.ReDraw();
      Bt6.Coords(220,155,240,175); Bt6.Colors(GREEN,BLACK,SQUARED,FILL); Bt6.ReDraw();
    break;
    case 2: 
      Bt1.SetState(bitRead(addr[1],6));
      Bt1.Coords(140,35,160,55); Bt1.Colors(GREEN,BLACK,SQUARED,FILL);Bt1.Text("", WHITE,Big); Bt1.ReDraw();
      Bt2.Coords(280,80,300,100); Bt2.Colors(RED,BLACK,SQUARED,FILL); Bt2.SetState(false); Bt2.Text("-", WHITE,Big); Bt2.ReDraw();
      Bt3.Coords(280,140,300,160); Bt3.Colors(BLUE,BLACK,SQUARED,FILL); Bt3.SetState(false); Bt3.Text("+", WHITE,Big);Bt3.ReDraw();
      //Bt4.Coords(140,95,160,115); Bt4.Colors(GREEN,BLACK,SQUARED,FILL); Bt4.ReDraw();
      //Bt5.Coords(140,125,160,145); Bt5.Colors(GREEN,BLACK,SQUARED,FILL); Bt5.ReDraw();
      //Bt6.Coords(140,155,160,175); Bt6.Colors(GREEN,BLACK,SQUARED,FILL); Bt6.ReDraw();
    break;
  }
}
void speed2byte(byte val){      //détermine les valeurs des bits pour le parametre ComSpeed
      bitWrite(addr[0],5,bitRead(val,0)); //comspeed
      bitWrite(addr[0],6,bitRead(val,1)); //comspeed 
      bitWrite(addr[0],7,bitRead(val,2)); //comspeed 
}
void sdf2byte(byte val){        //determine la valeur des bits pour le nb files sd
      bitWrite(addr[1],0,bitRead(val,0)); 
      bitWrite(addr[1],1,bitRead(val,1)); 
      bitWrite(addr[1],2,bitRead(val,2));
      bitWrite(addr[1],3,bitRead(val,3));
      bitWrite(addr[1],4,bitRead(val,4));
      
}

