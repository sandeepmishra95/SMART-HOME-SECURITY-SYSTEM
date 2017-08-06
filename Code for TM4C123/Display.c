#include "Display.h"
#include "Nokia5110.h"
#include "SysTick.h"

#define HIGH 1
#define LOW 0


// Image of Alert sign

const char alert[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xE0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC,
0xFC, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xE0, 0xF0,
0x80, 0x00, 0x00, 0x00, 0xC3, 0xFF, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F,
0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0xFF, 0xC3, 0x00, 0x00, 0x00, 0x80, 0xF0, 0xE0, 0x60, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x04, 0x07, 0x07, 0x07, 0x0F, 0x0E, 0x0C, 0x09, 0x0F, 0x9C, 0x9C, 0x3C, 0xFC, 0x7E, 0x7E,
0xE7, 0xE1, 0xE7, 0x7E, 0x7E, 0xFC, 0x3C, 0x9C, 0x9C, 0x0F, 0x09, 0x0C, 0x0E, 0x0F, 0x07, 0x07,
0x07, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x1E, 0x3F, 0x0F, 0x07, 0x07,
0x02, 0x00, 0x01, 0x02, 0x03, 0x00, 0x03, 0x02, 0x01, 0x00, 0x02, 0x07, 0x07, 0x3F, 0x3F, 0x1E,
0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};


// Displays Splash screen
void display_splash_screen(void)
{
	Nokia5110_Clear();
	Nokia5110_OutString("************", LIGHT);
	Nokia5110_OutString("*Smart Home*", LIGHT);
	Nokia5110_OutString("* Security *", LIGHT);
	Nokia5110_OutString("*  System  *", LIGHT);
	Nokia5110_OutString("*   v1.0   *", LIGHT);
	Nokia5110_OutString("************", LIGHT);
}

// Displays Main screen
void display_main_screen(unsigned long sensor0_arm, unsigned long sensor1_arm)
{
	unsigned status = 0;
	
	if( sensor0_arm == HIGH && sensor1_arm == HIGH )
		status = 0;
	
	else if(sensor0_arm == HIGH && sensor1_arm != HIGH)
		status = 1;
	
	else if(sensor0_arm != HIGH && sensor1_arm == HIGH)
		status = 2;
	
	else 
		status = 3;
	
	Nokia5110_Clear();
	switch(status)
	{
		case 0:
			Nokia5110_OutString(" All Sensors", LIGHT);	
			Nokia5110_OutString(" Are  Armed ", LIGHT);
			break;
		case 1:
			Nokia5110_OutString("Only Sensor0", LIGHT);	
			Nokia5110_OutString("  Is  Armed ", LIGHT);
			break;
		case 2:
			Nokia5110_OutString("Only Sensor1", LIGHT);	
			Nokia5110_OutString("  Is  Armed ", LIGHT);
			break;
		case 3:
			Nokia5110_OutString(" All Sensors", LIGHT);	
			Nokia5110_OutString("Are Disarmed", LIGHT);
			break;
	}
	
	Nokia5110_OutString("------------",LIGHT);
	//Nokia5110_SetCursor(0,3);
	
	if(status!=3)
		Nokia5110_OutString("System : ON ", LIGHT);
	else
		Nokia5110_OutString("System : OFF", LIGHT);
	
	Nokia5110_OutString("Press OK for", LIGHT);
	Nokia5110_OutString("More Options", LIGHT);
}

void display_alert(void)
{
	Nokia5110_Clear();
	Nokia5110_DrawFullImage(alert,LIGHT);	
	Nokia5110_SetCursor(0,4);
	Nokia5110_OutString("** Alert ** ", LIGHT);
	Nokia5110_OutString("  Press OK  ", LIGHT);
}

void display_menu(unsigned long sensor0_arm,unsigned long sensor1_arm, unsigned char mode)
{
	Nokia5110_Clear();
	Nokia5110_SetCursor(0,0);
	Nokia5110_OutString("*** MENU ***", LIGHT);
	Nokia5110_OutString("------------", LIGHT);
	
	if(mode==0) // Selection at 1st option 
	{
		if(sensor0_arm==HIGH)
		{
			Nokia5110_OutChar(0x80,DARK);
			Nokia5110_OutString(" DISARM    ",DARK);
			Nokia5110_OutString("  SENSOR 0  ",DARK);			
		}
		else
			{
			Nokia5110_OutChar(0x80,DARK);
			Nokia5110_OutString(" ARM       ",DARK);
			Nokia5110_OutString("  SENSOR 0  ",DARK);			
		}
		
		if(sensor1_arm==HIGH)
			{
			Nokia5110_OutString("  DISARM    ",LIGHT);
			Nokia5110_OutString("  SENSOR 1  ",LIGHT);			
			}
		else
			{
			Nokia5110_OutString("  ARM       ",LIGHT);
			Nokia5110_OutString("  SENSOR 1  ",LIGHT);			
			}
	}
	
	else
	{
		if(sensor0_arm==HIGH)
		{
			Nokia5110_OutString("  DISARM    ",LIGHT);
			Nokia5110_OutString("  SENSOR 0  ",LIGHT);			
		}
		else
			{
			Nokia5110_OutString("  ARM       ",LIGHT);
			Nokia5110_OutString("  SENSOR 0  ",LIGHT);			
		}
		
		if(sensor1_arm==HIGH)
			{
			Nokia5110_OutChar(0x80,DARK);
			Nokia5110_OutString(" DISARM    ",DARK);
			Nokia5110_OutString("  SENSOR 1  ",DARK);			
			}
		else
			{
			Nokia5110_OutChar(0x80,DARK);
			Nokia5110_OutString(" ARM       ",DARK);
			Nokia5110_OutString("  SENSOR 1  ",DARK);					
			}
	}
}



