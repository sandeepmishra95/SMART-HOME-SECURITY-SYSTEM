#include "Nokia5110.h"
#include "PLL.h"
#include "Display.h"
#include "SysTick.h"
#include "UART.h"
#include "tm4c123gh6pm.h"


//define section
#define HIGH 1
#define LOW 0

// Global Variables
unsigned long sensor0_status = HIGH;	// Status of sensor 0				(HIGH: Door Closed):(LOW: Door Open)
unsigned long sensor0_arm = HIGH;			// Armed status of sensor 0	(HIGH: Sensor Enabled):(LOW: Sensor Disabled)			
unsigned long sensor1_status = HIGH;	// Status of sensor 1				(HIGH: Door Closed):(LOW: Door Open)
unsigned long sensor1_arm = HIGH;			// Armed status of sensor 1	(HIGH: Sensor Enabled):(LOW: Sensor Disabled)	
unsigned long msg_received = 0;

/*	Screen = 0 -> Main screen
		Screen = 1 -> Menu screen
		Screen = 2 -> Alert Screen
*/
unsigned long screen = 0; 

/*	Mode = 0 means option one is highlighted
		Mode = 1 means option two is highlighted
*/
unsigned long mode = 0;

/* Option = 0 means no button is pressed
	 Option = 1 means back button is pressed
	 Option = 2 means select button is pressed
	 option = 3 means ok button is pressed
*/
unsigned long last_option = 0;
unsigned long alarm = LOW;


void PortB_Init(void); // Configure PORT B PB0-2 as inputs
unsigned long read_input(void);	//Read Inputs
void process_input(unsigned long input); // Processes input
void EnableInterrupts(void);
void DisableInterrupts(void);
void UART1_Handler(void);
void Timer2_Init(unsigned long period);
void Timer2A_Handler(void);
void turn_on_buzzer(void);
void turn_off_buzzer(void);

char Rx_Buf[20]; //Buffer to store incoming UART Data

int main(void)
{
	//unsigned long i,j,k;
  PLL_Init();                           // set system clock to 80 MHz
	UART_Init();
	SysTick_Init();												// Initialise Systick module
	PortB_Init();
  Nokia5110_Init();			// Initialise Nokia LCD Module
	Delay_10ms(5);
  Nokia5110_Clear();										// Clear LCD Screen
	display_splash_screen();
	Delay_10ms(200);
	Nokia5110_Clear();	
	Timer2_Init(8000000);
	
	while(1)
	{
		 if(screen == 0)
			 display_main_screen(sensor0_arm, sensor1_arm);
		 else if (screen == 1)
			 display_menu(sensor0_arm,sensor1_arm, mode);
		 else if (screen == 2)
			 display_alert();
		 
		 Delay_10ms(5);
		 process_input( read_input() );
	}

}

void PortB_Init(void)
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000002;     // 1) B clock
  delay = SYSCTL_RCGC2_R;           // 2) delay for clock to stabilise    
  GPIO_PORTB_AMSEL_R &= ~0x0F;      // 3) disable analog function
  GPIO_PORTB_PCTL_R &= ~0x0F;   		// 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R &= ~0x07 ;       // 5) PB 0-2 input
	GPIO_PORTB_DIR_R |= 0x08;
  GPIO_PORTB_AFSEL_R &= ~0x0F;      // 6) no alternate function
  GPIO_PORTB_PUR_R |= 0x07;         // 7) enable pullup resistors on PB2-0       
  GPIO_PORTB_DEN_R |= 0x0F;         // 8) enable digital pins PB3-0      
}

unsigned long read_input(void)
{
	unsigned long input = GPIO_PORTB_DATA_R & 0x07;
	Delay_10ms(2);
	if(input == last_option)
		return 0;
	else
		last_option = input;
	switch(input)
	{
		case 0x07:
			return 0;
		case 0x06:
			return 1;
		case 0x05:
			return 2;
		case 0x04:
			return 2;
		default:
			return 3;			
	}
}

void process_input(unsigned long input)
{
	
	if(screen==0) // If me are on Main screen
	{
		if(input == 3) // If Input is OK
		{
			screen=1; // Go to Menu screen
			mode = 0;	// Set mode to 0
		}
	}
	
	else if(screen==1) // If we are on Menu screen
	{
		if(input==1) // If input is back
		{
			if(alarm == LOW) // If alarm is off
			{
				screen=0; // Go to main screen
				mode=0;	// Set mode to 0
			}
			else // If alarm is on
			{
				screen = 2; //Go to Alert screen
				mode = 0; // Set mode to 0
			}
		}
		else 
		{
			if(input==2) // If input is select
			{
				mode= 1-mode; //change options
			}
			
			if(input==3) // If input is OK
			{
				if(mode==0) // If we are on option 1
				{
					if(sensor0_arm == HIGH)
						sensor0_arm = LOW;
					else
						sensor0_arm = HIGH;
				}
				else if(mode==1) // If we are on option 2
				{
					if(sensor1_arm == HIGH)
						sensor1_arm = LOW;
					else
						sensor1_arm = HIGH;
				}
				
			}
			
		}
	}
	else if(screen == 2)
	{
		if(input==3)
		{
			screen = 1;
			mode = 0;
		}
	}
}


void UART1_Handler(void)
{
	UART1_ICR_R &= ~0x50; //Set 1 to interrupt clear register
	//DisableInterrupts();
	msg_received = UART_InCharNonBlocking();
	switch(msg_received)
	{
		case 0:
			sensor0_status = HIGH;
			sensor1_status = HIGH;
			break;
		case 1:
			sensor0_status = LOW;
			sensor1_status = HIGH;
			break;
		case 2:
			sensor0_status = HIGH;
			sensor1_status = LOW;
			break;
		case 3:
			sensor0_status = LOW;
			sensor1_status = LOW;
			break;	
	}
	
}

// You can use this timer only if you learn how it works
void Timer2_Init(unsigned long period)
{ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}

void Timer2A_Handler(void)
{ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
	// Do other Task
	if(sensor0_arm==HIGH && sensor1_arm==HIGH)
		UART_OutChar(0x00);
	else if(sensor0_arm==LOW && sensor1_arm==HIGH)
		UART_OutChar(0x01);
	else if(sensor0_arm==HIGH && sensor1_arm==LOW)
		UART_OutChar(0x02);
	else 
		UART_OutChar(0x03);
	
	if(alarm==0)
	{
		if(sensor0_arm == HIGH && sensor0_status==LOW)
		{
			alarm=1;
			screen=2;
			turn_on_buzzer();
		}
		if(sensor1_arm == HIGH && sensor1_status==LOW)
		{
			alarm=1;
			screen=2;
			turn_on_buzzer();
		}
	}
	else
	{
		if(sensor0_arm==LOW && sensor1_arm==LOW)
		{
			alarm=0;
			turn_off_buzzer();
		}
	}
}

void turn_on_buzzer(void)
{
	GPIO_PORTB_DATA_R |= 0x08;
}

void turn_off_buzzer(void)
{
	GPIO_PORTB_DATA_R &= ~0x08;
}
