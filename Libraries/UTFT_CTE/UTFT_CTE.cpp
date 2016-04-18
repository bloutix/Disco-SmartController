/*
  UTFT_CTE.cpp - Addon library for Henning Karlsen's UTFT library (2.4.1 or above) to support drawing text and images on the serial flash of the CTE LCD modules (v.1.0 initial release)
  Copyright (C)2013 Coldtears electronics. All right reserved v.1.1
  
  http://stores.ebay.com/coldtears-electronics-store/
  
 Currently support CTE modules:
 1. CTE 5" LCD Module w/Font IC (CTE50)
 2. CTE 7" LCD Module w/Font IC (CTE70)
 3. CTE 3.2" LCD Module w/Font IC 320x240 (CTE32)
 4. CTE 3.2" LCD Module w/Font IC 480x320 (CTE32HR)
 5. CTE 3.2" Wide LCD Module w/Font IC 400x240 (TFT01-32WD)
 6. CTE 4.3" LCD Module w/Font IC (ITDB43)
 
 Demo sketches are avalibable for the above 6 modules (without any sketch modification) 
 for arduino MEGA2560 and arduino DUE using CTE shield
 
 If you are using CTE shield for DUE, you need to uncomment "#define CTE_DUE_SHIELD 1" in the HW_ARM_defines.h in the \hardware\arm folder of the UTFT library
 
 Thanks for Henning Karlsen's help and support in creating this add-on library
  Feel free to modify the code to suite your needs, any improvement or comments,
 Please send email to coldtearselectronics@gmail.com

  UTFT library can be downloaded at:
  http://electronics.henningkarlsen.com/
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.
*/

#include "UTFT_CTE.h"
#include <UTFT.h>

#if defined(__AVR__)											
	#include "hardware/avr/HW_AVR.h"							
#elif defined(__arm__)											
	#include "hardware/arm/HW_ARM.h"							
#endif															

#define swap(a, b) { int16_t t = a; a = b; b = t; }

UTFT_CTE::UTFT_CTE(UTFT *ptrUTFT)								
{
	_UTFT = ptrUTFT;
		
}

void UTFT_CTE::show_color_bar()
{
	unsigned long i,j;
		
	
	cbi(_UTFT->P_CS, _UTFT->B_CS);								
	
	switch(_UTFT->display_model)								
	{
	case CTE32HR:												
		for (i=0; i<(_UTFT->disp_y_size+1); i++)				
		{
			for (j=0; j<(_UTFT->disp_x_size+1); j++)			
			{			
				if(i>420)_UTFT->LCD_Write_DATA(0xFF,0xFF);
				else if(i>360)_UTFT->LCD_Write_DATA(0x00,0x1F);
				else if(i>300)_UTFT->LCD_Write_DATA(0x07,0xE0);
				else if(i>240)_UTFT->LCD_Write_DATA(0x7F,0xFF);
				else if(i>180)_UTFT->LCD_Write_DATA(0xF8,0x00);
				else if(i>120)_UTFT->LCD_Write_DATA(0xF8,0x1F);
				else if(i>60)_UTFT->LCD_Write_DATA(0xFF,0xE0);
				else _UTFT->LCD_Write_DATA(213,156);
			}	
		}					
		break;
	case CTE50:													
	case CTE70:													
		for (i=0; i<(_UTFT->disp_y_size+1); i++)				
		{
			for (j=0; j<(_UTFT->disp_x_size+1); j++)			
			{			
	 			if(i>700)_UTFT->LCD_Write_DATA(0xFF,0xFF);
 	 			else if(i>600)_UTFT->LCD_Write_DATA(0x00,0x1F);
 	 			else if(i>500)_UTFT->LCD_Write_DATA(0x07,0xE0);
 	 			else if(i>400)_UTFT->LCD_Write_DATA(0x7F,0xFF);
 	 			else if(i>300)_UTFT->LCD_Write_DATA(0xF8,0x00);
 	 			else if(i>200)_UTFT->LCD_Write_DATA(0xF8,0x1F);
 	 			else if(i>100)_UTFT->LCD_Write_DATA(0xFF,0xE0);
				else _UTFT->LCD_Write_DATA(213,156);
			}	
		}		
		break;	
		case CTE32:													
		for (i=0; i<(_UTFT->disp_y_size+1); i++)				
		{
			for (j=0; j<(_UTFT->disp_x_size+1); j++)			
			{			
	 			if(i>280)_UTFT->LCD_Write_DATA(0xFF,0xFF);
 	 			else if(i>240)_UTFT->LCD_Write_DATA(0x00,0x1F);
 	 			else if(i>200)_UTFT->LCD_Write_DATA(0x07,0xE0);
 	 			else if(i>160)_UTFT->LCD_Write_DATA(0x7F,0xFF);
 	 			else if(i>120)_UTFT->LCD_Write_DATA(0xF8,0x00);
 	 			else if(i>80)_UTFT->LCD_Write_DATA(0xF8,0x1F);
 	 			else if(i>40)_UTFT->LCD_Write_DATA(0xFF,0xE0);
				else _UTFT->LCD_Write_DATA(213,156);
			}	
		}					
		break;	
		case ITDB43:
		setXY_4(0,0,_UTFT->disp_y_size-1,_UTFT->disp_x_size-1);																
		for (i=0; i<(_UTFT->disp_y_size+1); i++)				
		{
			for (j=0; j<(_UTFT->disp_x_size+1); j++)			
			{			
	 			if(i>350)_UTFT->LCD_Write_DATA(0xFF,0xFF);
 	 			else if(i>300)_UTFT->LCD_Write_DATA(0x00,0x1F);
 	 			else if(i>250)_UTFT->LCD_Write_DATA(0x07,0xE0);
 	 			else if(i>200)_UTFT->LCD_Write_DATA(0x7F,0xFF);
 	 			else if(i>150)_UTFT->LCD_Write_DATA(0xF8,0x00);
 	 			else if(i>100)_UTFT->LCD_Write_DATA(0xF8,0x1F);
 	 			else if(i>50)_UTFT->LCD_Write_DATA(0xFF,0xE0);
				else _UTFT->LCD_Write_DATA(213,156); 

			}	
		}					
		break;	
		case TFT01_32WD:												
		for (i=0; i<(_UTFT->disp_y_size+1); i++)				
		{
			for (j=0; j<(_UTFT->disp_x_size+1); j++)			
			{			
	 			if(i>350)_UTFT->LCD_Write_DATA(0xFF,0xFF);
 	 			else if(i>300)_UTFT->LCD_Write_DATA(0x00,0x1F);
 	 			else if(i>250)_UTFT->LCD_Write_DATA(0x07,0xE0);
 	 			else if(i>200)_UTFT->LCD_Write_DATA(0x7F,0xFF);
 	 			else if(i>150)_UTFT->LCD_Write_DATA(0xF8,0x00);
 	 			else if(i>100)_UTFT->LCD_Write_DATA(0xF8,0x1F);
 	 			else if(i>50)_UTFT->LCD_Write_DATA(0xFF,0xE0);
				else _UTFT->LCD_Write_DATA(213,156); 

			}	
		}					
		break;		
	}
	sbi(_UTFT->P_CS, _UTFT->B_CS);								
}


void UTFT_CTE::SPI_Flash_init(int CSpin)
{
	P_F_CS	= portOutputRegister(digitalPinToPort(CSpin));
	B_F_CS	= digitalPinToBitMask(CSpin);
	pinMode(CSpin,OUTPUT);
	_SPIstart();												
}


void UTFT_CTE::Send_Flash_information_to_UART()
{
	cbi(P_F_CS, B_F_CS);
	_SPIwrite(0x03);											
	_SPIwrite(0);												
	_SPIwrite(0);												
	_SPIwrite(0);												
	for (int i=0; i<1000; i++)
		Serial1.write(_SPIread());								
	sbi(P_F_CS, B_F_CS);
}

void  UTFT_CTE::Load_image(int X, int Y, int location)
{
	unsigned long address;
	unsigned char pixelH, pixelL,t1;
	unsigned int w, h;
	unsigned int x_1,y_1;
				

	switch(_UTFT->display_model)								
  	{
	case CTE32HR:												
		X=_UTFT->disp_y_size-X;									
		break;  
	case CTE32:													
		X=_UTFT->disp_y_size-X;									
		break;  
	case CTE50:													
	case CTE70:
	case ITDB43:													
		X=_UTFT->disp_y_size-X;								
		break;  
	case TFT01_32WD:													
		X=X;								
		break;  									
	}	
				
	location = location + 224;
	address=(unsigned long)location*4096;
	char H = address>>16;
	char M = address>>8;

	cbi(P_F_CS, B_F_CS);
	_SPIwrite(0x03);											
	_SPIwrite(H);												
	_SPIwrite(M);												
	_SPIwrite(0x00);											
	t1 = _SPIread();											
	t1 = _SPIread();											
	w = _SPIread();												
	w = (w << 8) | _SPIread();									
	h = _SPIread();												
	h = (h << 8) | _SPIread();									
	sbi(P_F_CS, B_F_CS);
			   
	if(t1!=16)	return;	
	if((h>801)||(w>801)) return;	
			   
 	cbi(_UTFT->P_CS, _UTFT->B_CS);								

	switch(_UTFT->display_model)								
  	{
	case CTE32:													
		_UTFT->LCD_Write_COM_DATA(0x11,0x6018);					
		setXY_4(Y,X,Y+h-1,X-w+1);
 		break;
	case CTE32HR:												
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0xAE);
  		setXY_4(Y,X,Y+h-1,X-w+1);
		break;
	case CTE50:													
	case CTE70:										
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0x42);
		setXY_4(X,Y,X-w+1,Y+h-1);	
		break;
	case ITDB43:	
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0x42);
		setXY_4(X-w+1,Y,X,Y+h-1);																
		break;
	case TFT01_32WD:												
		_UTFT->LCD_Write_COM(0x16); 
		_UTFT->LCD_Write_DATA(0xBC);
		setXY_4(X,Y,X+w-1,Y+h-1);	
		
		break;		
 	}
 					
	sbi(_UTFT->P_RS, _UTFT->B_RS);								
	cbi(P_F_CS, B_F_CS);
    _SPIwrite(0x03);											
	_SPIwrite(H);												
	_SPIwrite(M);												
	_SPIwrite(0x08);											
	for (x_1 = 0; x_1<w ; x_1++)
		for (y_1 = 0; y_1<h ; y_1++)
			_UTFT->LCD_Writ_Bus(_SPIread(),_SPIread(),16);		

	switch(_UTFT->display_model)								
  	{
  		case CTE32:												
		_UTFT->LCD_Write_COM_DATA(0x11,0x6070);
 		break;
		case CTE32HR: 											
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0x0A);
  		break;
		case CTE50:												
		case CTE70:	
		case ITDB43:											
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0x22);	
		break;
		case TFT01_32WD:												
		_UTFT->LCD_Write_COM(0x16); 
		_UTFT->LCD_Write_DATA(0x1C);
		break;				
 	} 					
	sbi(_UTFT->P_CS, _UTFT->B_CS);								
	sbi(P_F_CS, B_F_CS);
}

void UTFT_CTE::setXY_4(word x1, word y1, word x2, word y2)
{

	switch(_UTFT->display_model)								
	{
	case CTE32:													
		swap(y1, y2)
		_UTFT->LCD_Write_COM_DATA(0x44,(x2<<8)+x1);
		_UTFT->LCD_Write_COM_DATA(0x45,y1);
		_UTFT->LCD_Write_COM_DATA(0x46,y2);
		_UTFT->LCD_Write_COM_DATA(0x4e,x1);
		_UTFT->LCD_Write_COM_DATA(0x4f,y2);
		_UTFT->LCD_Write_COM(0x22); 
		break;		
	case CTE32HR:												
		swap(y1, y2);
		_UTFT->LCD_Write_COM(0x2b); 
  		_UTFT->LCD_Write_DATA(x1>>8);
  		_UTFT->LCD_Write_DATA(x1);
  		_UTFT->LCD_Write_DATA(x2>>8);
  		_UTFT->LCD_Write_DATA(x2);
		_UTFT->LCD_Write_COM(0x2a); 
  		_UTFT->LCD_Write_DATA(y1>>8);
  		_UTFT->LCD_Write_DATA(y1);
  		_UTFT->LCD_Write_DATA(y2>>8);
  		_UTFT->LCD_Write_DATA(y2);
		_UTFT->LCD_Write_COM(0x2c); 
		break;
	case CTE50:													
	case CTE70:													
 		swap(x1, x2);
		_UTFT->LCD_Write_COM(0x2a); 
  		_UTFT->LCD_Write_DATA(x1>>8);
  		_UTFT->LCD_Write_DATA(x1);
  		_UTFT->LCD_Write_DATA(x2>>8);
  		_UTFT->LCD_Write_DATA(x2);
		_UTFT->LCD_Write_COM(0x2b); 
  		_UTFT->LCD_Write_DATA(y1>>8);
  		_UTFT->LCD_Write_DATA(y1);
  		_UTFT->LCD_Write_DATA(y2>>8);
  		_UTFT->LCD_Write_DATA(y2);
		_UTFT->LCD_Write_COM(0x2c); 
		break;
	case ITDB43:													
		_UTFT->LCD_Write_COM(0x2a); 
  		_UTFT->LCD_Write_DATA(x1>>8);
  		_UTFT->LCD_Write_DATA(x1);
  		_UTFT->LCD_Write_DATA(x2>>8);
  		_UTFT->LCD_Write_DATA(x2);
		_UTFT->LCD_Write_COM(0x2b); 
  		_UTFT->LCD_Write_DATA(y1>>8);
  		_UTFT->LCD_Write_DATA(y1);
  		_UTFT->LCD_Write_DATA(y2>>8);
  		_UTFT->LCD_Write_DATA(y2);
		_UTFT->LCD_Write_COM(0x2c); 
		break;
	case TFT01_32WD:
		_UTFT->LCD_Write_COM_DATA(0x02,x1>>8);
		_UTFT->LCD_Write_COM_DATA(0x03,x1);
		_UTFT->LCD_Write_COM_DATA(0x04,x2>>8);
		_UTFT->LCD_Write_COM_DATA(0x05,x2);
		_UTFT->LCD_Write_COM_DATA(0x06,y1>>8);
		_UTFT->LCD_Write_COM_DATA(0x07,y1);
		_UTFT->LCD_Write_COM_DATA(0x08,y2>>8);
		_UTFT->LCD_Write_COM_DATA(0x09,y2);
		_UTFT->LCD_Write_COM(0x22);      
		break;
		
	}	
}

void UTFT_CTE::setXY_2(word x1, word y1, word x2, word y2)
{
	swap( x1, y1);
	swap( x2, y2);
	swap( y1, y2);

	switch(_UTFT->display_model)								
	{
	case CTE32:													
		_UTFT->LCD_Write_COM_DATA(0x44,(x2<<8)+x1);
		_UTFT->LCD_Write_COM_DATA(0x45,y1);
		_UTFT->LCD_Write_COM_DATA(0x46,y2);
		_UTFT->LCD_Write_COM_DATA(0x4e,x2);
		_UTFT->LCD_Write_COM_DATA(0x4f,y2);
		_UTFT->LCD_Write_COM(0x22); 
		break;	
	case CTE32HR:												
		_UTFT->LCD_Write_COM(0x2a); 
  		_UTFT->LCD_Write_DATA(x1>>8);
  		_UTFT->LCD_Write_DATA(x1);
  		_UTFT->LCD_Write_DATA(x2>>8);
  		_UTFT->LCD_Write_DATA(x2);
		_UTFT->LCD_Write_COM(0x2b); 
  		_UTFT->LCD_Write_DATA(y1>>8);
  		_UTFT->LCD_Write_DATA(y1);
  		_UTFT->LCD_Write_DATA(y2>>8);
  		_UTFT->LCD_Write_DATA(y2);
		_UTFT->LCD_Write_COM(0x2c); 
		break;
	case CTE50:													
	case CTE70:													
		swap( x1, y1);
		swap( x2, y2);
		_UTFT->LCD_Write_COM(0x2a); 
  		_UTFT->LCD_Write_DATA(x1>>8);
  		_UTFT->LCD_Write_DATA(x1);
  		_UTFT->LCD_Write_DATA(x2>>8);
  		_UTFT->LCD_Write_DATA(x2);
		_UTFT->LCD_Write_COM(0x2b); 
  		_UTFT->LCD_Write_DATA(y1>>8);
  		_UTFT->LCD_Write_DATA(y1);
  		_UTFT->LCD_Write_DATA(y2>>8);
  		_UTFT->LCD_Write_DATA(y2);
		_UTFT->LCD_Write_COM(0x2c); 
		break;
	case TFT01_32WD:
		swap( x1, y1);
		swap( x2, y2);
		swap( y1, y2);
		_UTFT->LCD_Write_COM_DATA(0x02,x1>>8);
		_UTFT->LCD_Write_COM_DATA(0x03,x1);
		_UTFT->LCD_Write_COM_DATA(0x04,x2>>8);
		_UTFT->LCD_Write_COM_DATA(0x05,x2);
		_UTFT->LCD_Write_COM_DATA(0x06,y1>>8);
		_UTFT->LCD_Write_COM_DATA(0x07,y1);
		_UTFT->LCD_Write_COM_DATA(0x08,y2>>8);
		_UTFT->LCD_Write_COM_DATA(0x09,y2);
		_UTFT->LCD_Write_COM(0x22);      
		break;
	}		
}

unsigned char font_height;
unsigned int font_size;
unsigned long font_address;
unsigned int PositionX, PositionY;
unsigned char Charspace=1;


void UTFT_CTE::Set_character_spacing(unsigned char space)
{
	Charspace=space;
}

 
void UTFT_CTE::Put_Text_array(char *st, int x, int y, int font_number)
{
	switch(_UTFT->display_model)								
	{				
	case CTE32HR:												
		PositionX=_UTFT->disp_y_size-x;							
		break;
	case CTE32:													
		PositionX=_UTFT->disp_y_size-x;							
		break;
	case CTE50:													
	case CTE70:	
	case ITDB43:												
		PositionX=_UTFT->disp_y_size-x;							
		break;	
	case TFT01_32WD:													
		PositionX=x;							
		break;						
	}

	PositionY=y;
	cbi(_UTFT->P_CS, _UTFT->B_CS);								

	switch(_UTFT->display_model)								
	{
	case CTE32:													
		_UTFT->LCD_Write_COM_DATA(0x11,0x6010);
		break;
	case CTE32HR:												
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0x8E);	
		break;						
	case CTE50:													
	case CTE70:
	case ITDB43:													
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0x62);	
		break;	
	case TFT01_32WD:													
		_UTFT->LCD_Write_COM(0x16); 
		_UTFT->LCD_Write_DATA(0x9C);	
			
		break;			
			
	}

	sbi(_UTFT->P_CS, _UTFT->B_CS);								
				
	int length,i;
	length = strlen(st);
	font_address=(unsigned long) font_number * 4096;
				
	cbi(P_F_CS, B_F_CS);
 	_SPIwrite(0x03);											
 	_SPIwrite(font_address>>16);								
 	_SPIwrite(font_address>>8);									
 	_SPIwrite(font_address);									
 	font_height=(_SPIread());									
						
  	sbi(P_F_CS, B_F_CS);
					
	cbi(P_F_CS, B_F_CS);
 	_SPIwrite(0x03);											 
 	_SPIwrite(font_address>>16);								 
 	_SPIwrite(font_address>>8);									 
	if (font_height>70)
		_SPIwrite(font_address+150);							 
	else 
	 	_SPIwrite(font_address+35);								 
 	font_size=(_SPIread());										 
						
  	sbi(P_F_CS, B_F_CS);	
	font_size=font_size*18;
	for (i=0; i<length; i++)
		Draw_character(st[i]);
	cbi(_UTFT->P_CS, _UTFT->B_CS);								 

	switch(_UTFT->display_model)								 
	{
	case CTE32:													 
		_UTFT->LCD_Write_COM_DATA(0x11,0x6070);
		break;
	case CTE32HR:												 
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0x0A);
		break;	
	case CTE50:													 
	case CTE70:	
	case ITDB43:															 
		_UTFT->LCD_Write_COM(0x36); 
		_UTFT->LCD_Write_DATA(0x22);	
		break;
	case TFT01_32WD:													
		_UTFT->LCD_Write_COM(0x16); 
		_UTFT->LCD_Write_DATA(0x1C);	
		break;							
	}

	sbi(_UTFT->P_CS, _UTFT->B_CS);								 
}

void UTFT_CTE::Put_Text(String st, int x, int y, int font_number)
{
	char buf[st.length()+1];

	st.toCharArray(buf, st.length()+1);
	Put_Text_array(buf, x, y, font_number);
}

void UTFT_CTE::Draw_character(unsigned char character)
{
	char ch = _UTFT->fch, cl = _UTFT->fcl;						
	char ch2 = _UTFT->bch, cl2 = _UTFT->bcl;					
	unsigned long location;
	unsigned char font_header[2];

	if ((character>=0x20)&&(character<=0xFF))
	{
		location = (unsigned long)(character-0x20)*(unsigned long)font_size + (unsigned long)font_address;
		cbi(P_F_CS, B_F_CS);
		_SPIwrite(0x03);										 
		_SPIwrite(location>>16);								 
		_SPIwrite(location>>8);									 
		_SPIwrite(location);									 
		font_header[0] = _SPIread();							 
		font_header[1] = _SPIread();							 
		sbi(P_F_CS, B_F_CS);
		cbi(_UTFT->P_CS, _UTFT->B_CS);							 

		switch(_UTFT->display_model)							 
		{
		case CTE32:												 
			setXY_2(PositionX,PositionY,PositionX-font_header[1]-Charspace,PositionY+font_height-1);
			break;
		case CTE32HR:											 
			setXY_2(PositionX,PositionY,PositionX-font_header[1]-Charspace,PositionY+font_height-1);
			break;	
		case CTE50:												 
		case CTE70:												
			setXY_2(PositionX,PositionY,PositionX+font_header[1]+Charspace,PositionY+font_height-1);
			break;	
		case TFT01_32WD:												
			setXY_4(PositionY,PositionX,PositionY+font_height-1,PositionX+font_header[1]+Charspace);
			break;	
		case ITDB43:
			setXY_4(PositionX-font_header[1]-Charspace,PositionY,PositionX,PositionY+font_height-1);
		
			break;				
		}	
		unsigned int i,j;
		unsigned int temp = font_header[1]*font_height;
		unsigned char bitsleft = temp%8;
		temp=temp/8+2;
		location=location+2;
		sbi(_UTFT->P_RS, _UTFT->B_RS);							
		cbi(P_F_CS, B_F_CS);
		_SPIwrite(0x03);										 
		_SPIwrite(location>>16);								 
		_SPIwrite(location>>8);									 
		_SPIwrite(location);									 

		for(i=2;i<temp;i++) 
		{
			volatile unsigned char m=_SPIread();				 
			for(j=0;j<8;j++) 
			{
				if((m&0x01)==0x01) 	_UTFT->LCD_Writ_Bus(ch,cl,16); 
				else _UTFT->LCD_Writ_Bus(ch2,cl2,16);
				//delay(1);
				m>>=1; 
			}
		}
				
		volatile unsigned char m=_SPIread();					 
		for(j=0;j<bitsleft;j++) 
		{
			if((m&0x01)==0x01) 	_UTFT->LCD_Writ_Bus(ch,cl,16);
			else _UTFT->LCD_Writ_Bus(ch2,cl2,16);
			m>>=1;
		}					
		for(i=0;i<Charspace;i++) 
		{
			for(unsigned char a=0;a<font_height;a++)
				_UTFT->LCD_Writ_Bus(ch2,cl2,16);
		}

		switch(_UTFT->display_model)							
		{				
		case CTE32HR:											
			PositionX =PositionX- font_header[1]-Charspace;
			break;
		case CTE32:												
			PositionX =PositionX- font_header[1]-Charspace;
			break;			
		case CTE50:												
		case CTE70:	
			case ITDB43:											
			PositionX =PositionX- font_header[1]-Charspace;
			break;
		case TFT01_32WD:												
			PositionX =PositionX+ font_header[1]+Charspace;
			break;			
							
		}
		sbi(P_F_CS, B_F_CS);
		sbi(_UTFT->P_CS, _UTFT->B_CS);							
	}
	else return;
}
