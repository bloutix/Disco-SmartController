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
 
//Interface setting 
  myGLCD.setColor(CYAN);
  myGLCD.drawRoundRect(20,224,319,239);
  
  myFiles.loadBitmap(0,0,15);
  myFiles.loadBitmap(1,200,120);

  Eext.Coords(200,119,230,149);
  Eext.Colors(GREEN,WHITE,NOFILL,SQUARED);
  Rext.Coords(235,119,265,149);
  Rext.Colors(GREEN,WHITE,NOFILL,SQUARED);
  Fan.Coords(270,119,300,149);
  Fan.Colors(RED,WHITE,NOFILL,SQUARED);
  Fan.ReDraw();
  Bsd.Coords(200,154,230,184);
  Bsd.Colors(GREEN,WHITE,NOFILL,SQUARED);
  Bmc.Coords(270,154,300,184);
  Bmc.Colors(GREEN,WHITE,NOFILL,SQUARED);
  Bpr.Coords(200,189,230,219);
  Bpr.Colors(CYAN,WHITE,NOFILL,SQUARED);
  Bpr.ReDraw();
  Brk.Coords(235,189,265,219);
  Brk.Colors(RED,WHITE,NOFILL,SQUARED);
  Brk.ReDraw();
  Bst.Coords(270,189,300,219);
  Bst.Colors(RED,WHITE,NOFILL,SQUARED);

 
//Buttons setting
  XH.Coords(2,17,52,67);
  YH.Coords(2,142,52,192);
  ZH.Coords(64,80,114,130);
  UB.Coords(64,17,114,67);
  LB.Coords(2,80,52,130);
  RB.Coords(127,80,177,130);
  DB.Coords(64,142,114,192);
  UZ.Coords(127,17,177,67);
  DZ.Coords(127,142,177,192);
  
  XH.Colors(GREEN,WHITE,NOFILL,SQUARED);
  YH.Colors(GREEN,WHITE,NOFILL,SQUARED);
  ZH.Colors(GREEN,WHITE,NOFILL,SQUARED);
  UB.Colors(BLUE,WHITE,NOFILL,SQUARED);
  LB.Colors(BLUE,WHITE,NOFILL,SQUARED);
  RB.Colors(BLUE,WHITE,NOFILL,SQUARED);
  DB.Colors(BLUE,WHITE,NOFILL,SQUARED);
  UZ.Colors(RED,WHITE,NOFILL,SQUARED);
  DZ.Colors(RED,WHITE,NOFILL,SQUARED);

  //slide control
  moveS.Coords(200,15,220,105);
  moveS.Colors(GREEN,BLACK,ROUNDED,NOFILL);
  moveS.SetDirection(BTT);
  moveS.SetRange(1,4,1);
  moveS.SetStartingValue(1);
  //moveS.Draw();
  
  tempH.Coords(230,15,250,105);
  tempH.Colors(BLUE,BLACK,ROUNDED,NOFILL);
  tempH.SetDirection(BTT);
  tempH.SetRange(1,4,1);
  tempH.SetStartingValue(1);
  //tempH.Draw();
  
  tempB.Coords(260,15,280,105);
  tempB.Colors(RED,BLACK,ROUNDED,NOFILL);
  tempB.SetDirection(BTT);
  tempB.SetRange(1,4,1);
  tempB.SetStartingValue(1);
  //tempB.Draw();

  Print_Bar.Coords(309,0,319,220); // FIRST, must set coordinates before setting Range and optional starting value
  Print_Bar.Attributes(ROUNDED,FILL);  //
  Print_Bar.Colors(CYAN,GREEN,RED,BLACK ); // Optional but good to have some color
  Print_Bar.SetDirection(BTT); // bottom to top   // Optional
  Print_Bar.Percentages(80,50,0);
  Print_Bar.SetRange(0,100);                 // SECOND, Must be set after coordinates
  Print_Bar.SetStartingValue(0);                // THIRD (Optional, but must be set after range)
  //Print_Bar.Draw();
  /*for (int i = 0; i <= 100; i++) // demo voltage
  {
    Print_Bar.SetValue(i);
    Print_Bar.Progress(false);
  }*/
  //myKB.Colors(WHITE,BLACK,BLUE,FILL);


