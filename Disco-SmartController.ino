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

#include "Arduino.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>

#include <UTFT.h>
#include <UTouch.h>
#include <TFT_ExtensionV2.h>
#include <SPI.h>
#include <SPIflash.h>
#include <UTFT_SPIflash.h>

#include "variables.h" //loading variable setting
#include "FWcode.h"   //gcode command
#include "language.h" //language array

//this is for servo z-probe
/*#include <Servo.h>
Servo Zservo;*/
//bool zstate = false;
bool Pcon=true;
void setup(){ 
    //Dont Touch this
  for(int i=0;i<8;i++){  //it take config from EEPROM
    addr[i]=EEPROM.read(i);
    delay(5);
  }
  //Dont touch this
  SERIAL_P.begin(spdVal(addr[0]>>5)); //init printer com dont delete
  SERIAL_U.begin(spdVal(addr[0]>>5));
  SERIAL_P.setTimeout(100);
  SERIAL_U.setTimeout(100);
  
  myFlash.begin();  //images stored in lcd flash ic
 // Zservo.attach(9);
  
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
  /*   SERIAL_P.write("?");
   while (SERIAL_P.available()==0 && millis()<3000) {
    myGLCD.setColor(WHITE);
    myGLCD.print(F("Test de connexion"),CENTER,110);
  }
  if (SERIAL_P.available()>0){
    SERIAL_P.read();
    Pcon =true;
  }*/
  myGLCD.fillScr(BLACK);
}

void loop() { 
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
void serialEvent(){
  if(!autoUSB && Pcon){
    loaded = false;
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    autoUSB = true;
    disp_pg=3;
  }
}
/*void serialEvent1(){
    chkServo();
}*/
void serialEvent2(){
 if(!autoBT && bitRead(addr[0],1) && Pcon){
    loaded = false;
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    autoBT = true;
    disp_pg=2;
 }
}
//PAGES d'affichage
void disp_Mcode(){    //afichage du clavier pour envoie de ligne de commande
  if (loaded == false) {
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,220);
    myFiles.loadBitmap(5,0,219);
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(RED);
    myGLCD.print(str(0),CENTER,15);
    myKB.Colors(WHITE, BLACK, GREY, NOFILL);
    myKB.SetupMobileKB(0,30, 319, 180); //NEEDED TO WORK!
    loaded = true;
  }
  char buf[30] = {'\0'};
  //myKB.ReceiveMsg(0, 0, YELLOW); // X,Y(position on screen), Color
  if (myKB.Mobile_KeyBoard(buf, BLUE))
  {
    setStateDisplay(buf);
    SERIAL_P.println(buf);
    /*awaitingOK=true;
    while(awaitingOK){
      //checkForOk();
    }*/
  }
  if (BACK.Touch()){
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    disp_pg =1;
    loaded = false;
  }
}
void disp_HOM(){  //affichage de l'écran d'accueil
  if (loaded == false) {
  myGLCD.setColor(CYAN);
  myGLCD.drawRoundRect(0,224,319,239);
  myGLCD.setColor(BLACK);
  myGLCD.fillRect(0,0,319,10);
  myGLCD.setBackColor(BLACK);
  myGLCD.setColor(CYAN);
  myGLCD.print(str(1),CENTER,0);
  if (Pcon){
    myGLCD.setColor(CYAN);
    myGLCD.print(str(2),CENTER,225);
  } else {
     myGLCD.setColor(RED);
     myGLCD.print(str(3),CENTER,225);
  }
  BTB.ReDraw();
  TCHB.ReDraw();
  USBB.ReDraw();
  SETB.ReDraw();
  myFiles.loadBitmap(4,160,45);
  myFiles.loadBitmap(6,90,130);
  myFiles.loadBitmap(7,90,60);
  myFiles.loadBitmap(8,20,130);
  myFiles.loadBitmap(9,20,60);
  loaded = true;
  }

  if (TCHB.Touch() && Pcon){
    loaded = false;
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    disp_pg = 1;
  } else if(BTB.Touch() && bitRead(addr[0],1) && Pcon){
    if(bitRead(addr[0],1)){
      loaded = false;
      myGLCD.setColor(BLACK);
      myGLCD.fillRect(0,10,319,239);
      disp_pg = 2;
    }
  } else if(USBB.Touch() && Pcon){
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
    
  InitTouchInterface();
  unsigned long currentMillis = millis();
  if (Bst.Touch()){  //arret urgence
     sendGcode(code(10)); //10->M112 or 11->M410
  }
    if ((currentMillis - lastUpdate) >= 5000) { //affiche la température toute les 5 secondes
      getTemperatures();
      getPrintState();
      lastUpdate=currentMillis; 
    }
  if (BACK.Touch()){
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.fillRect(0,0,15,10);
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
        myGLCD.print(str(0),CENTER,15);
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
          myGLCD.setColor(WHITE);
          myGLCD.print(str(4),20,40);
          myGLCD.print(str(5),120,40);
          myGLCD.print(str(6),170,40);
          myGLCD.print(str(7),20,70);
          myGLCD.print(str(8),20,100);
          myGLCD.print(str(9),20,130);
          myGLCD.print(str(10),20,160);
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
      myGLCD.setColor(WHITE);
      myGLCD.print(str(11),20,40);
      myGLCD.print(str(12),20,100);
      myGLCD.print(str(13),20,130);
      myGLCD.print(str(14),20,160);
      bps = addr[0]>>5;
      par1 = (addr[1]<<3);
      par1 = par1>>3;
      myGLCD.printNumI(par1,170,100, 0);
      myGLCD.print(String(spdVal(bps),DEC),100,70);
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
        myGLCD.setColor(GREEN);myGLCD.drawRect(20,95,70,115);
        myGLCD.setColor(GREY);myGLCD.drawRect(100,95,150,115);myGLCD.drawRect(180,95,230,115);
        myGLCD.drawRect(20,165,70,185);myGLCD.drawRect(100,165,150,185);myGLCD.drawRect(180,165,230,185);
        myGLCD.setColor(WHITE);
        myGLCD.print(F("Buse > 255~C ?"),20,40);
        myGLCD.print(F("Temperatures de la Buse"),20,70);
        myGLCD.print(F("T1"),40,80);myGLCD.print(F("T2"),120,80);myGLCD.print(F("T3"),200,80);
        myGLCD.print(F("Temperatures du lit chauffant"),20,140);
        myGLCD.print(F("T1"),40,150);myGLCD.print(F("T2"),120,150);myGLCD.print(F("T3"),200,150);
        myGLCD.printNumI(addr[2]*mult,30,100);myGLCD.printNumI(addr[3]*mult,110,100);myGLCD.printNumI(addr[4]*mult,190,100);
        myGLCD.printNumI(addr[5],30,170);myGLCD.printNumI(addr[6],110,170);myGLCD.printNumI(addr[7],190,170);
        sub_loaded = true;
      }

        (Bt1.Toggle())? bitWrite(addr[1],6,1):bitWrite(addr[1],6,0);
        mult=(Bt1.getState())? 2:1;
         myGLCD.setColor(WHITE);
         myGLCD.printNumI(addr[2]*mult,30,100);myGLCD.printNumI(addr[3]*mult,110,100);myGLCD.printNumI(addr[4]*mult,190,100);
         
        if(Bt4.Touch()){
          myGLCD.setColor(GREY);
          myGLCD.drawRect(((selected-col)*80)+20,95+ln,((selected-col)*80)+70,115+ln);
          selected--;
          col = (selected>2)? 3: 0;
          ln = (selected>2)? 70:0;
          if(selected<0){selected=0;}
          myGLCD.setColor(GREEN);
          myGLCD.drawRect(((selected-col)*80)+20,95+ln,((selected-col)*80)+70,115+ln);
          delay(70);
        }else if(Bt5.Touch()){ 
          myGLCD.setColor(GREY);
          myGLCD.drawRect(((selected-col)*80)+20,95+ln,((selected-col)*80)+70,115+ln);
          selected++;
          col = (selected>2)? 3: 0;
          ln = (selected>2)? 70:0;
          if(selected>param_cnt){selected=param_cnt;}
          myGLCD.setColor(GREEN);
          myGLCD.drawRect(((selected-col)*80)+20,95+ln,((selected-col)*80)+70,115+ln);
          delay(70);
        }else if(Bt2.Touch()){
          updTemp(selected,0);
        }else if(Bt3.Touch()){
          updTemp(selected,1);
        }
        if(SDPTB.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          selected=0;
          sub_pg = 3;
          sub_loaded = false;
        } else if (BACK.Touch()){
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(0,30,319,219);
          selected=0;
          sub_pg =1;
          sub_loaded = false;
        }
    break;
    case 3: //resume and validate
      if (sub_loaded == false) {
        myGLCD.setColor(BLACK);
        SDPTB.Text(strc(18),RED,Small);
        SDPTB.ReDraw();
        resConf();
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
          SDPTB.Text(strc(19),BLUE,Small);
          SDPTB.ReDraw();
          sub_pg =2;
          sub_loaded = false;
        }
    break;
    case 4: //do eeprom update
      if (sub_loaded == false) {
        SDPTB.Text(strc(20),WHITE,Small);
        SDPTB.ReDraw();
        myGLCD.setColor(WHITE);
        myGLCD.print(str(21),CENTER,115);
        myGLCD.setColor(BLACK);
        myGLCD.fillRect(0,220,30,239);
        //putConf(); //sauvegarde les parametre dans eeprom
        
        
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
  //SERIAL_U.begin(spdVal(addr[0]>>5)); // init usb to printer

  myFiles.loadBitmap(2,0,0);
  myFiles.loadBitmap(5,0,219);
  myGLCD.setColor(RED);
  myGLCD.drawRoundRect(50,105,270,125);
  myGLCD.setBackColor(BLACK);
  myGLCD.print(str(22),CENTER,110);
  loaded =true;
  }
  
  if (BACK.Touch()){
    SERIAL_U.flush();
    delay(2);
    SERIAL_U.read();
    //SERIAL_U.end();
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.fillRect(0,0,15,10);
    disp_pg =0;
    autoUSB=false;
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
  SERIAL_BT.begin(spdVal(addr[0]>>5)); //init bt to printer
  SERIAL_BT.setTimeout(100);

  myGLCD.print(str(23),CENTER,110);
  loaded =true;
  }
  
  if (BACK.Touch()){
    SERIAL_BT.flush();
    delay(2);
    SERIAL_BT.read();
    SERIAL_BT.end();
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.fillRect(0,0,15,10);
    disp_pg =0;
    autoBT=false;
    loaded = false;
  } else {
    if(bitRead(addr[0],1)){
      serialEcho(2);
    }
  }
}
void disp_SD(){   //affichage de la liste des fichiers sur la carte sd
  if (loaded == false) {
  myFiles.loadBitmap(5,0,219);
  myGLCD.setBackColor(GREY);
  myGLCD.setColor(BLUE);
  myGLCD.print(str(24),CENTER,15);
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
  myGLCD.print(str(18),230,190);
  for (int i=0; i<file_cnt; i++){
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
    if(selected>file_cnt-1){selected=file_cnt-1;}
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
    file[selected].toLowerCase();
    SERIAL_P.println(code(2)+file[selected]);
    memset(file,0,sizeof(file));
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.fillRect(0,0,15,10);
    disp_pg=1;
    loaded = false;
    waitPrinting = true;
  }
  if (BACK.Touch()){
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(0,10,319,239);
    myGLCD.fillRect(0,0,15,10);
    disp_pg =1;
    loaded = false;
  }
}

/*Fonctions principales*/
/*void chkServo(){
   if (SERIAL_P.readStringUntil('\n')== F("zservo")){
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
}*/
//fonctions liées au SLIDER
void setMvStep(unsigned int MvStep){ //fonction de parametrage des mouvements en mm
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
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(140,209,195,219);
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(GREEN);
    myGLCD.print(str(37),140,195);
    myGLCD.print(strMvc+str(38),140,208);
}
void setBedTemp(unsigned int temp){  //fonction de parametrage de la température du lit chauffant
  if (temp == 1){
    sendGcode(code(17)+"0");
  } else if (temp == 2){
    sendGcode(code(17)+String(addr[5],DEC));
  } else if (temp == 3){
    sendGcode(code(17)+String(addr[6],DEC));
  } else {
    sendGcode(code(17)+String(addr[7],DEC));
  }
}
void setHdTemp(unsigned int temp){   //fonction de parametrage de la température de la buse
    if (temp == 1){
    sendGcode(code(12)+"0");
  } else if (temp == 2){
    sendGcode(code(12)+String(addr[2],DEC));
  } else if (temp == 3){
    sendGcode(code(12)+String(addr[3],DEC));
  } else {
    sendGcode(code(12)+String(addr[4],DEC));
  }
}

//fonctions de communication
void serialEcho(byte scom){ //bypass serial com
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
void sendGcode(String lineOfCode){    //envoie de Gcode avec attente de msg OK
  SERIAL_P.println(lineOfCode);
  //awaitingOK = true;  
  setStateDisplay(lineOfCode);
  delay(1);
  //checkForOk();
}
/*void checkForOk() {   //fonction d'attente de MSG_OK
  String c ="";
   while (SERIAL_P.available()) {  
    c= SERIAL_P.readString();
    if (c.startsWith(F("ok"))) {awaitingOK=false;}
    else{ awaitingOK=false; }
    c="";
    delay(1);     
    }
}*/

//fonctions affichage et controle
void InitTouchInterface(){  //variables et fonctions de l'interface tactile
    unsigned int Mvslide = moveS.Touch();
    unsigned int Tbslide = tempB.Touch();
    unsigned int Thslide = tempH.Touch();
  if(lastTb != Tbslide && tempB.Touch()) {
    setBedTemp(Tbslide);
    lastTb = Tbslide;
  }
  if(lastHb != Thslide && tempH.Touch()) {
    setHdTemp(Thslide);
    lastHb = Thslide;
  }
  if(lastMv != Mvslide && moveS.Touch()) {
    setMvStep(Mvslide);
    lastMv = Mvslide;
  }
    myGLCD.setColor(WHITE);
    myGLCD.setBackColor(BLACK);
   if (XH.Touch()){           //home X
     sendGcode(code(21));
   } else if(YH.Touch()) {    //home Y
     sendGcode(code(24));
   } else if(ZH.Touch()) {    //home Z
     sendGcode(code(27));
   } else if(UB.Touch()) {    //deplacer plateau vers arriere
     sendGcode(code(31));
     sendGcode(code(23)+strMvc); //for speed add +code(32)
     sendGcode(code(30));
   } else if(DB.Touch()) {    //deplacer plateau vers avant
     sendGcode(code(31));
     sendGcode(code(22)+strMvc); //for speed add +code(32)
     sendGcode(code(30));
   } else if(LB.Touch()) {    //deplacer buse vers la gauche
     sendGcode(code(31));
     sendGcode(code(19)+strMvc); //for speed add +code(32)
     sendGcode(code(30));
   } else if(RB.Touch()) {    //deplacer buse vers la droite
     sendGcode(code(31));
     sendGcode(code(20)+strMvc); //for speed add +code(32)
     sendGcode(code(30));
   } else if(UZ.Touch()) {    //monter buse
     sendGcode(code(31));
     sendGcode(code(26)+strMvc); //for speed add +code(32)
     sendGcode(code(30));
   } else if(DZ.Touch()) {    //descendre buse
     sendGcode(code(31));
     sendGcode(code(25)+strMvc); //for speed add +code(32)
     sendGcode(code(30));     
   } else if(Eext.Touch()) {  //extrusion filament
     sendGcode(code(31));
     sendGcode(code(29)+strMvc); //for speed add +code(33)
     sendGcode(code(30));  
   } else if(Rext.Touch()){   //retracter filament
     sendGcode(code(31));
     sendGcode(code(28)+strMvc); //for speed add +code(33)
     sendGcode(code(30));  
   } else if (Bsd.Touch()) {  // lister les fichiers sur la carte sd
      myGLCD.setColor(BLACK);
      myGLCD.fillRect(0,10,319,239);
      loaded= false;
      disp_pg = 5;
   } else if ( Fan.Toggle()){
       Fan.SetState(false);
       if (fanS) {
        if (bitRead(addr[0],4)){
          sendGcode(code(16));
        } else {
          sendGcode(code(15));
        }
        Fan.Colors(RED,RED,NOFILL);
        Fan.ReDraw();
        fanS = false;
       } else {
        sendGcode(code(14));
        Fan.Colors(WHITE,WHITE,NOFILL);
        Fan.ReDraw();
        fanS = true;
       }
   } else if(Brk.Toggle()){
       Brk.SetState(false);
       if (!Brked) {
        sendGcode(code(4));
        Brk.Colors(RED,RED,NOFILL);
        Brk.ReDraw();
        Brked = true;
       } else {
        sendGcode(code(3));
        Brk.Colors(WHITE,WHITE,NOFILL);
        Brk.ReDraw();
        Brked = false;
       }
   } else if(Bpr.Touch()){
      sendGcode(code(3));
      waitPrinting = false;
      isSDPrinting=true;
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
     SERIAL_P.println(code(1));
     SERIAL_P.read(); //on vide serial buffer
     SERIAL_P.println(code(0));
     while (SERIAL_P.available()==0) { }
     while (SERIAL_P.available()>0){
        delay(10);
        filename = SERIAL_P.readStringUntil('\n');
        if (filename != F("End file list") && startsd==true) {  
          int cnt_sdf= (addr[1]<<3)>>3;
          if(file_cnt < cnt_sdf){ //if is GCODE file //(filename.endsWith(".G") || filename.endsWith(".g")) && 
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
    clearText(20,225);
    myGLCD.setBackColor(BLACK);
    myGLCD.setColor(WHITE);
    myGLCD.print(dispstring,20,225);    
}
void clearText(int col, int line){    //lié à la fonction précédente
    //myGLCD.print(F("                                  "),col,line); 
    myGLCD.setColor(BLACK);
    myGLCD.fillRect(col,line,col+299,line+10);
}

//fonctions affichage de valeurs dynamiques/cycliques
void getTemperatures() {    //fonction affichage des température
  if(waitPrinting == false){
      //SERIAL_P.read(); //on vide le buffer
      SERIAL_P.println(code(13));
      //while (SERIAL_P.available()==0) { }
     if( String strTemps = SERIAL_P.readStringUntil('\n')){
        if(strTemps.startsWith(F("ok T:"))){
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
            myGLCD.setColor(BLACK);
            myGLCD.fillRect(0,190,120,219);
            myGLCD.setColor(RED);
            myGLCD.print("Lit "+c+"/"+d+"~C",0,195);
            myGLCD.setColor(BLUE);
            myGLCD.print("Buse "+a+"/"+b+"~C",0,210);
            //on libère la ram
            a= "";b=a;c=b;d=c;
        } else if (strTemps.startsWith(F("T:"))){
          // T:29.99 E:0 B:88.5
            if (find_text("B", strTemps)>-1){
              String a = strTemps.substring(strTemps.indexOf(F("T:"))+2, strTemps.indexOf(F("E")));
              a.trim();
              // Get bed temperature
              String b = strTemps.substring(strTemps.indexOf(F("B:"))+2);
              b.trim();
              myGLCD.setBackColor(BLACK);
              myGLCD.setColor(BLACK);
              myGLCD.fillRect(0,190,120,219);
              myGLCD.setColor(RED);
              myGLCD.print("Lit "+b+"~C",0,195);
              myGLCD.setColor(BLUE);
              myGLCD.print("Buse "+a+"~C",0,210);
              //on libère la ram
            a="";b=a;
            } else {
              //T:42.4 E:0 W:?
              String a = strTemps.substring(strTemps.indexOf(F("T:"))+2, strTemps.indexOf(F("E")));
              a.trim();
              String b = strTemps.substring(strTemps.indexOf(F("W:"))+2);
              b.trim();
              myGLCD.setBackColor(BLACK);
              myGLCD.setColor(BLACK);
              myGLCD.fillRect(0,190,120,219);
              myGLCD.setColor(BLUE);
              myGLCD.print("Buse "+a+"~C"+" W "+b,0,210);
              //on libère la ram
              a="";b=a;
            }
        }
        strTemps = SERIAL_P.read();strTemps = ""; //on vide le buffer serie et on libere de la ram
     }
  }
}
int find_text(String needle, String haystack) {   //this function search for char in string
  int foundpos = -1;
  for (int i = 0; (i < haystack.length() - needle.length()); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}
String code(byte i){   //get gcode str
  strcpy_P(Mfw_buf, (char*)pgm_read_word(&(Mfw_table[i])));
  return Mfw_buf;
}
String str(byte i){
  strcpy_P(str_buf, (char*)pgm_read_word(&(strFR_table[i])));
  return str_buf;
}
char* strc(byte i){
  strcpy_P(strc_buf, (char*)pgm_read_word(&(strFR_table[i])));
  return strc_buf;
}
void getPrintState(){ //affiche le pourcentage, le temps estimé de l'impression
  //M31 echo:54 min, 38 sec //récupère le temps écoulé (reply)
  //M27 SD printing byte 2134/235422 récupère le numéro de ligne en cours (reply)
  //M26 Filename S2134 imprime depuis la ligne x du fichier en byte  (cmd with ok reply)
  if(isSDPrinting == true){
  SERIAL_P.println(code(6)); //demande le numéro de "ligne" en cours
    while (SERIAL_P.available()==0) { }
  if( String strPbytes = SERIAL_P.readString()){
    if(strPbytes.startsWith(F("SD printing byte")) && !strPbytes.startsWith(F("SD printing byte 0/0"))){
       String c = strPbytes.substring(strPbytes.indexOf(F("byte"))+5, strPbytes.indexOf(F("/")));
            c.trim();
      unsigned long stByte = c.toInt();
      
      String d = strPbytes.substring(strPbytes.indexOf(F("/"))+1);
            d.trim();
      unsigned long endByte = d.toInt();

      //strPbytes =""; //on libère le string de la sram
      SERIAL_P.read();  //vide le buffer
      
      SERIAL_P.println(code(9)); //demande le temps écoulé
      while (SERIAL_P.available()==0) { }
      if( String strTimes = SERIAL_P.readString()){
        if(strTimes.startsWith(F("echo:"))){
          String a = strTimes.substring(strTimes.indexOf(F("echo:"))+5, strTimes.indexOf(F(" min")));
                a.trim();
          unsigned int Tmin = a.toInt();
          
          String b = strTimes.substring(strTimes.indexOf(F(","))+2, strTimes.indexOf(F(" sec")));
                b.trim();
          unsigned int Tsec = b.toInt();
    
          strTimes =""; //on libere la sram
          SERIAL_P.read(); //vide le buffer

          //calcul et affichage des données de temps
          float Ppercent = ((((float)stByte/(float)endByte)*100.0)+0.5);  //calcul pourcentage impression pour bar 
          //estimation du temps total
          unsigned long FTsec = ((Tmin*60)+Tsec); //temps total ecoulé en seconde
          /*long estSec = ((FTsec*100)/Ppercent); //temps total estimé en seconde
          int estH = (estSec/3600);  //temps estimé H
          int estM = ((estSec-(estH*3600))/60); //temps estimé m
          int estS = (estSec-(estH*3600)-(estM*60)); //temps estimé s*/
      
          //temps écoulé en H min
          unsigned int Thour = (int)(Tmin/60);
          Tmin = (int)(Tmin-(Thour*60));
          
          //affichage
          myGLCD.setBackColor(BLACK);
          myGLCD.setColor(BLACK);
          myGLCD.fillRect(20,225,310,235);
          myGLCD.setColor(WHITE);
          myGLCD.printNumI(Thour,20,225);
          myGLCD.print(F("h"),35,225);
          myGLCD.printNumI(Tmin,45,225);
          myGLCD.print(F("m"),55,225);
          myGLCD.printNumI(Tsec,70,225);
          myGLCD.print(F("s"),85,225);
          /*myGLCD.printNumI(estH,105,225);
          myGLCD.print(F("h"),120,225);
          myGLCD.printNumI(estM,130,225);
          myGLCD.print(F("m"),145,225);
          myGLCD.printNumI(estS,155,225);
          myGLCD.print(F("s"),170,225);*/

          if(Ppercent >=99){
            Ppercent = 0;
            myGLCD.setBackColor(BLACK);
            myGLCD.setColor(WHITE);
            myGLCD.print(str(28),20,225);
            isSDPrinting = false;
            waitPrinting = false;
          }
          
          //actualistion barre de chargement
          Print_Bar.SetValue(Ppercent);
          Print_Bar.Progress(false);

          lastByte = stByte; //stroring byte for security not yet implemented
          //libere la SRAM
          a="";b=a;c=a;d=a;
          stByte = 0;endByte=0;
          Ppercent=0;Thour=0;Tmin=0;Tsec=0;
          FTsec=0; //estS=0;estH=0;estM=0;estSec=0;
        }
      }
    }
  }
}}

//fonctions de pramétrage
//arduino mega eeprom = 4kb or maybe must to use flash ic for storing data
void updispval(unsigned int val, int x, int y, byte nset){   //affichage des valeur dynamique de reglage 
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
          myGLCD.setBackColor(BLACK);
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
          myGLCD.print(String(spdVal(val),DEC),x,y);
          myGLCD.setBackColor(BLACK);
          chg[1] = false;
        }
    break;

  }
}
void resConf(){
  byte sdfimx = addr[1]<<3;

  myGLCD.setColor(WHITE);
  myGLCD.print(str(4),10,30);
  myGLCD.print(str(29),150,30);
  
  myGLCD.print(str(8),10,40);
  myGLCD.print(str(9),150,40);

  myGLCD.print(str(10),10,50);
  myGLCD.print(str(30),150,50);

  myGLCD.print(str(31),10,60);
  myGLCD.print(str(13),150,60);

  myGLCD.print(str(14),10,70);
  myGLCD.print(str(15),150,70);

  myGLCD.print(str(16),CENTER,90);
  myGLCD.print(str(17),CENTER,130);
  myGLCD.setColor(GREEN);
  myGLCD.printNumI(addr[5],50,150);
  myGLCD.printNumI((addr[2]*mult),50,110);
  myGLCD.setColor(ORANGE);
  myGLCD.printNumI((addr[3]*mult),120,110);myGLCD.printNumI(addr[6],120,150);
  myGLCD.setColor(RED);
  myGLCD.printNumI(addr[7],200,150);myGLCD.printNumI((addr[4]*mult),200,110);
  myGLCD.print(str(32),CENTER,180);
  myGLCD.print(str(33),CENTER,190);

  myGLCD.setColor(BLUE);
  myGLCD.print(((bitRead(addr[0],0))? F("Anglais") : F("Francais")),70,30);
  myGLCD.print(((bitRead(addr[0],4))? F("Dagoma"): F("Autre")),240,30);
  myGLCD.print(((bitRead(addr[0],1))? str(34): str(35)),100,40);
  myGLCD.print(((bitRead(addr[0],2))? str(34): str(35)),260,40);
  myGLCD.print(((bitRead(addr[0],3))? str(34): str(35)),100,50);
  myGLCD.print(String((spdVal(addr[0]>>5)),DEC),230,50);
  myGLCD.printNumI((sdfimx>>3),100,60);
  myGLCD.print(((bitRead(addr[1],5))? str(34): str(35)),230,60);
  myGLCD.print(((bitRead(addr[1],7))? str(34): str(35)),110,70);
  myGLCD.print(((bitRead(addr[1],6))? str(34): str(35)),260,70);
}
void updTemp(int Tn,bool ud){
  if(Tn<3 && bitRead(addr[1],6) == 1){
    mult =2;
  } else {
    mult=1;
  }
  if(ud){ 
    if(addr[Tn+2]<255)
         addr[Tn+2]++;
  }else {
    if(addr[Tn+2]>0)
         addr[Tn+2]--;
  }
        int x;
        if(Tn==0 || Tn==3){
          x=30;
        }else if(Tn==1 || Tn==4){
          x=110;
        }else if(Tn==2 || Tn==5){
          x=190;
        }
        int y = (Tn>2) ? 170 : 100;
        myGLCD.setColor(BLACK);
        myGLCD.fillRect(x-15,y,x+25,y+10);
        myGLCD.setColor(WHITE);
        myGLCD.printNumI(addr[Tn+2]*mult,x,y);
}
void rstBtn(byte pg){   //reset btn  for saving SRAM
  switch(pg){
    case 0:
      SDPTB.Text(strc(19),BLUE,Small);SDPTB.ReDraw();
      //Bt1.SetState(bitRead(addr[0],0));Bt2.SetState(!bitRead(addr[0],0));
      Bt3.SetState(bitRead(addr[0],4));
      Bt4.SetState(bitRead(addr[0],1));Bt5.SetState(bitRead(addr[0],2));Bt6.SetState(bitRead(addr[0],3));
      Bt1.Coords(140,35,160,55); Bt1.Colors(GREEN,BLACK,SQUARED,FILL); Bt1.Text("", BLACK,Big); Bt1.ReDraw();
      Bt2.Coords(190,35,210,55); Bt2.Colors(GREEN,BLACK,SQUARED,FILL); Bt2.Text("", BLACK,Big); Bt2.ReDraw();
      Bt3.Coords(140,65,160,85); Bt3.Colors(GREEN,BLACK,SQUARED,FILL); Bt3.Text("", BLACK,Big); Bt3.ReDraw();
      Bt4.Coords(140,95,160,115); Bt4.Colors(GREEN,BLACK,SQUARED,FILL); Bt4.Text("", BLACK,Big); Bt4.ReDraw();
      Bt5.Coords(140,125,160,145); Bt5.Colors(GREEN,BLACK,SQUARED,FILL); Bt5.ReDraw();
      Bt6.Coords(140,155,160,175); Bt6.Colors(GREEN,BLACK,SQUARED,FILL); Bt6.ReDraw();
    break;
    case 1:
      SDPTB.Text(strc(19),BLUE,Small);SDPTB.ReDraw();
      Bt5.SetState(bitRead(addr[1],5));Bt6.SetState(bitRead(addr[1],7));
      Bt1.Coords(220,35,240,55); Bt1.Colors(RED,BLACK,SQUARED,FILL); Bt1.SetState(false); Bt1.Text("-", WHITE,Big); Bt1.ReDraw();
      Bt2.Coords(260,35,280,55); Bt2.Colors(BLUE,BLACK,SQUARED,FILL); Bt2.SetState(false); Bt2.Text("+", WHITE,Big); Bt2.ReDraw();
      Bt3.Coords(220,95,240,115); Bt3.Colors(RED,BLACK,SQUARED,FILL); Bt3.SetState(false); Bt3.Text("-", WHITE,Big); Bt3.ReDraw();
      Bt4.Coords(260,95,280,115); Bt4.Colors(BLUE,BLACK,SQUARED,FILL); Bt4.SetState(false); Bt4.Text("+", WHITE,Big); Bt4.ReDraw();
      Bt5.Coords(220,125,240,145); Bt5.Colors(GREEN,BLACK,SQUARED,FILL); Bt5.Text("", WHITE,Small); Bt5.ReDraw();
      Bt6.Coords(220,155,240,175); Bt6.Colors(GREEN,BLACK,SQUARED,FILL); Bt6.ReDraw();
    break;
    case 2: 
      SDPTB.Text(strc(19),BLUE,Small);SDPTB.ReDraw();
      Bt1.SetState(bitRead(addr[1],6));
      Bt1.Coords(140,35,160,55); Bt1.Colors(GREEN,BLACK,SQUARED,FILL);Bt1.Text("", WHITE,Big); Bt1.ReDraw();
      Bt2.Coords(280,80,300,100); Bt2.Colors(RED,BLACK,SQUARED,FILL); Bt2.SetState(false); Bt2.Text("-", WHITE,Big); Bt2.ReDraw();
      Bt3.Coords(280,140,300,160); Bt3.Colors(BLUE,BLACK,SQUARED,FILL); Bt3.SetState(false); Bt3.Text("+", WHITE,Big);Bt3.ReDraw();
      Bt4.Coords(80,195,100,215); Bt4.Colors(GREEN,BLACK,SQUARED,FILL); Bt4.SetState(false); Bt4.Text("<-", WHITE,Small);Bt4.ReDraw();
      Bt5.Coords(140,195,160,215); Bt5.Colors(GREEN,BLACK,SQUARED,FILL); Bt5.SetState(false); Bt5.Text("->", WHITE,Small);Bt5.ReDraw();
      //Bt6.Coords(140,155,160,175); Bt6.Colors(GREEN,BLACK,SQUARED,FILL); Bt6.ReDraw();
    break;
  }
}
long spdVal(byte val){      //this return bps for serial begin
  long valb;
  if(val==1) valb=9600;
    if(val==2) valb=14400;
    if(val==3) valb=19200;
    if(val==4) valb=38400;
    if(val==5) valb=57600;
    if(val==6) valb=115200;
  return valb;
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
void putConf(){     //met à jour les parametres dans EEPROM
  for(int i=0;i<8;i++){
    EEPROM.update(i,addr[i]);
    delay(5);
  }
}

