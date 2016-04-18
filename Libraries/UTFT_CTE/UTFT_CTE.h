#ifndef UTFT_CTE_h											
#define UTFT_CTE_h											

#if defined(__AVR__)										
	#include "hardware/avr/HW_AVR_defines.h"				
	#include "SPI.h"										
#elif defined(__arm__)										
	#include "hardware/arm/HW_ARM_defines.h"				
#endif														

#include <UTFT.h>
#if ((!defined(UTFT_VERSION)) || (UTFT_VERSION<241))
	#error : You will need UTFT v2.41 or higher to use this add-on library...
#endif

//BVS= Bitstream vera sans, suffix = font size in pixel (height)
#define BVS_13 10											 
#define BVS_15 12											 
#define BVS_19 14											 
#define BVS_22 18											 
#define BVS_28 22											 
#define BVS_34 28											 
#define BVS_43 38											 
#define BVS_52 53											 
#define BVS_74 78											 
#define BVS_112 122											 

class UTFT_CTE
{
	public:
		UTFT_CTE(UTFT *ptrUTFT);							
		void	SPI_Flash_init(int CSpin);
		void	show_color_bar();
		void	Load_image(int X, int Y, int location);
		void	Send_Flash_information_to_UART();
		void	Put_Text(String st, int x, int y, int font_number);
		void	Put_Text_array(char *st, int x, int y, int font_number);
		void	Set_character_spacing(unsigned char space);

		
	protected:												
		UTFT	*_UTFT;
		regtype *P_F_CS;
		regsize B_F_CS;
		void	setXY_2(word x1, word y1, word x2, word y2);
		void	setXY_4(word x1, word y1, word x2, word y2);
		void	Draw_character(unsigned char character);

		void	_SPIstart();								
		void	_SPIwrite(byte data);						
		byte	_SPIread();									
};

#if defined(__arm__)										
	static	void spiInit(uint8_t spiRate);					
	static	void spiBegin();								
	static	uint8_t spiRec();								
	static	void spiSend(uint8_t b);						
#endif														

#endif														
