#include "STD_types.h"
#include "BIT_math.h"
#include "CLOCK_interface.h"
#include "GPIO_int.h"
#include "INTERRUPT.h"
#include "EXTI_int.h"
#include "ADC_int.h"
#include "TIM_int.h"
#include "TIM_config.h"
#include "MOTORS_PID_CONTROL_int.h"
#include "std_struct.h"
#include "doubly.h"
#include <stdlib.h>
#include <util/delay.h>


u8 flag = 0;
u8 flood_map[16][16];
enum orientation{North_R=1,East_R,West_R=8,South_R=4};
u8 cell_detail[16][16]  = { { 9,1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 3 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 12 ,4, 4, 4, 4, 4, 4, 4, 4 ,4, 4, 4, 4, 4, 4, 6 } };
void refill_flood(Node *Header_q1,Node* Trailer_q1 ,Position src ,Position dest, u8 num_dest);
u16 update_wall(Position cell,u8 state_walls, u8 orientation);
void reset_flag(void) ;

int main(void)
{
	MGPIO_void_SetPinDirection(GPIO_PORTD,3,GPIO_OUTPUT);		//s0
	MGPIO_void_SetPinDirection(GPIO_PORTD,4,GPIO_OUTPUT);		//s1
	MGPIO_void_SetPinDirection(GPIO_PORTD,7,GPIO_OUTPUT);		//s2

	MGPIO_void_SetPinDirection(GPIO_PORTC,0,GPIO_INPUT);	// SharpIR
	MGPIO_void_SetPinDirection(GPIO_PORTD,2,GPIO_INPUT_PULLUP);
	
	MGPIO_void_SetPinDirection (GPIO_PORTD , 5 , GPIO_OUTPUT);
	MGPIO_void_SetPinDirection (GPIO_PORTD , 6 , GPIO_OUTPUT);
	
	MGPIO_void_SetPinDirection (GPIO_PORTB , 4 , GPIO_OUTPUT);
	MGPIO_void_SetPinDirection (GPIO_PORTB , 5 , GPIO_OUTPUT);
	MGPIO_void_SetPinDirection (GPIO_PORTB , 2 , GPIO_OUTPUT);
	MGPIO_void_SetPinDirection (GPIO_PORTB , 3 , GPIO_OUTPUT);
	MADC_void_Initialize (  ADC_MODE_SINGLE_CONVERSION , ADC_AVCC , ACD_128_PRE , ADC_10BIT );

	HMotorPidControl_void_PIDinitialize();
	MEXTI_void_EnableInterrupt (EXTI_0);
	MEXTI_void_SetTrigger(EXTI_0, TRIGGER_RISING_EDGE);
	MEXTI_void_SetCallBack (EXTI_0, &reset_flag);
	
	MTIM8_void_initialize(TIM0 ,TIM_FAST_PWM, TIM_FAST_PWM_NON_INVERTING , TIM_FAST_PWM_NON_INVERTING ,TIM_CS_64_PRESCALER );
	MTIM8_void_initialize(TIM2 ,TIM_NORMAL_MODE, 0 , 0 ,TIM_CS_64_PRESCALER );
	HMotorPidControl_void_StopRobot();


	u8 IR_Sensor_Read =0;
	Node* Header = (Node*)malloc(sizeof(Node)) ;
	Node* Trailer =(Node*)malloc(sizeof(Node)) ;

	Header->Next = Trailer;
	Trailer->Prev = Header;
	Position src;
	Position dest;
	Position cur_cell;

	u16 min_and_dir = 0;
	u8 arr_mux[3] = {2,0,5} ;   // front_r - "front_l" - right - left

	while (1)
    {
		if(flag)
		{
			
				MGPIO_void_SetPinValue (GPIO_PORTD, 3, GET_BIT(arr_mux[0],0) );
				MGPIO_void_SetPinValue (GPIO_PORTD, 4, GET_BIT(arr_mux[0],1) );
				MGPIO_void_SetPinValue (GPIO_PORTD, 7, GET_BIT(arr_mux[0],2) );
				if (MADC_u16_ReadChannelSynch ( ADC_6  ) < 825 )
				{
					MGPIO_void_SetPinValue (GPIO_PORTB , 4 , GPIO_HIGH);
					MGPIO_void_SetPinValue (GPIO_PORTB , 5 , GPIO_LOW);
					MGPIO_void_SetPinValue (GPIO_PORTB , 2 , GPIO_HIGH);
					MGPIO_void_SetPinValue (GPIO_PORTB , 3 , GPIO_LOW);
				}
				else
				{
					
						MGPIO_void_SetPinValue (GPIO_PORTB , 4 , GPIO_LOW);
						MGPIO_void_SetPinValue (GPIO_PORTB , 5 , GPIO_LOW);
						MGPIO_void_SetPinValue (GPIO_PORTB , 2 , GPIO_LOW);
						MGPIO_void_SetPinValue (GPIO_PORTB , 3 , GPIO_LOW);
				}
			

		}

    }

	return 0 ;
}

void refill_flood(Node *Header_q1,Node* Trailer_q1,Position src ,Position dest ,u8 num_dest )
{
	Position cur_position , temp  ;

	for (u8 i =0 ; i<16;i++)
	{
		for (u8 j =0 ; j<16;j++)
		{
			flood_map[i][j] = 255 ;
		}

	}
	if (num_dest == 1)
	{
		flood_map[dest.loc.row][dest.loc.col] = 0 ;
		push( Header_q1 , Trailer_q1 ,dest) ;
	}
	else
	{
		flood_map[dest.loc.row][dest.loc.col] = 0 ;
		push( Header_q1 , Trailer_q1 ,dest) ;

		flood_map[dest.loc.row][dest.loc.col+1] = 0 ;
		temp.loc.row=dest.loc.row;
		temp.loc.col=dest.loc.col+1 ;
		push( Header_q1 , Trailer_q1 ,temp) ;

		flood_map[dest.loc.row+1][dest.loc.col] = 0 ;
		temp.loc.row=dest.loc.row+1;
		temp.loc.col=dest.loc.col ;
		push( Header_q1 , Trailer_q1 ,temp) ;

		flood_map[dest.loc.row+1][dest.loc.col+1] = 0 ;
		temp.loc.row=dest.loc.row+1;
		temp.loc.col=dest.loc.col+1 ;
		push( Header_q1 , Trailer_q1 ,temp) ;
	}




	while ( Doubly_Is_NOT_Empty(Header_q1 , Trailer_q1))
	{
		//x'w'  x'S'  x'E'  x'N'
		cur_position =Pop (Header_q1 , Trailer_q1);
		if ( !(GET_BIT(cell_detail[cur_position.loc.row][cur_position.loc.col],0))
		&& cur_position.loc.row-1 >= 0 ) // for North
		{
			if( flood_map[cur_position.loc.row-1][cur_position.loc.col] == 255)
			{
				flood_map[cur_position.loc.row-1][cur_position.loc.col] = flood_map[cur_position.loc.row][cur_position.loc.col] +1 ;
				temp.loc.row=cur_position.loc.row-1;
				temp.loc.col=cur_position.loc.col ;
				push( Header_q1 , Trailer_q1 ,temp) ;
			}

		}
		if ( (!GET_BIT(cell_detail[cur_position.loc.row][cur_position.loc.col],1) )
		&&cur_position.loc.col+1 <= 15  ) // for East
		{
			if(flood_map[cur_position.loc.row][cur_position.loc.col+1] == 255)
			{
				flood_map[cur_position.loc.row][cur_position.loc.col+1] = flood_map[cur_position.loc.row][cur_position.loc.col] +1 ;
				temp.loc.row=cur_position.loc.row;
				temp.loc.col=cur_position.loc.col+1 ;
				push( Header_q1 , Trailer_q1 ,temp) ;
			}

		}
		if ( !(GET_BIT(cell_detail[cur_position.loc.row][cur_position.loc.col],2))
		&& cur_position.loc.row+1 <= 15 ) // for South
		{
			if(flood_map[cur_position.loc.row+1][cur_position.loc.col] == 255)
			{
				flood_map[cur_position.loc.row+1][cur_position.loc.col] = flood_map[cur_position.loc.row][cur_position.loc.col] +1 ;
				temp.loc.row=cur_position.loc.row+1;
				temp.loc.col=cur_position.loc.col ;
				push( Header_q1 , Trailer_q1 ,temp) ;
			}
		}
		if ( !(GET_BIT(cell_detail[cur_position.loc.row][cur_position.loc.col],3))
		&& cur_position.loc.col-1>= 0 ) // for West
		{
			if(flood_map[cur_position.loc.row][cur_position.loc.col-1] == 255)
			flood_map[cur_position.loc.row][cur_position.loc.col-1] = flood_map[cur_position.loc.row][cur_position.loc.col] +1 ;
			temp.loc.row=cur_position.loc.row;
			temp.loc.col=cur_position.loc.col-1 ;
			push ( Header_q1 , Trailer_q1 ,temp) ;
		}

	}
}


u16 update_wall(Position cell,u8 state_walls,u8 orientation)
{
	//8		4		2	1
	//x'w'  x'S'  x'E'  x'N'

	//cell_detail[cell.loc.row][cell.loc.col] |= state_walls ;
	u16 loc_and_min = 0xFF00 ;  							//1111 1111 0000 0001 0xFF00
	//xxxx xxxx 0000 0001
	switch(orientation)
	{
		case North_R:
		cell_detail[cell.loc.row][cell.loc.col] |= state_walls ;

		if ( cell.loc.row+1 <= 15)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],2)) // for South
			{
				SET_BIT(cell_detail[cell.loc.row+1][cell.loc.col],0 );

			}
			else if ( flood_map[cell.loc.row+1][cell.loc.col] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row+1][cell.loc.col]) <<8 | 1<<2 ;		  // Robot's South
			}

		}
		if (cell.loc.col-1>= 0)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],3)) // for West
			{
				SET_BIT(cell_detail[cell.loc.row][cell.loc.col-1],1 );

			}
			else if ( flood_map[cell.loc.row][cell.loc.col-1] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row][cell.loc.col-1]) <<8 | 1<<3 ;
			}
		}



		if (cell.loc.col+1 <= 15)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],1)) // for East
			{
				SET_BIT(cell_detail[cell.loc.row][cell.loc.col+1],3 );
			}
			else if ( flood_map[cell.loc.row][cell.loc.col+1] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row][cell.loc.col+1]) <<8 | 1<<1 ;
			}
		}

		if (cell.loc.row-1 >= 0 )
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],0) ) // for North
			{
				SET_BIT(cell_detail[cell.loc.row-1][cell.loc.col],2 );
			}
			else if ( flood_map[cell.loc.row-1][cell.loc.col] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row-1][cell.loc.col]) <<8 | 1<<0 ;
			}
		}
		break;


		case East_R:
		state_walls = ((state_walls<<1)&0x0E)| GET_BIT(state_walls,3) ;  // for o -> east
		cell_detail[cell.loc.row][cell.loc.col] |= state_walls;
		if (cell.loc.col-1>= 0)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],3)) // for West
			{
				SET_BIT(cell_detail[cell.loc.row][cell.loc.col-1],1 );

			}
			else if ( flood_map[cell.loc.row][cell.loc.col-1] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row][cell.loc.col-1]) <<8 | 1<<2 ;    // for Left -- Back -- Right -- Front  ->bits
			}
		}


		if (cell.loc.row-1 >= 0 )
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],0) ) // for North
			{
				SET_BIT(cell_detail[cell.loc.row-1][cell.loc.col],2 );
			}
			else if ( flood_map[cell.loc.row-1][cell.loc.col] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row-1][cell.loc.col]) <<8 | 1<<3 ;
			}
		}

		if ( cell.loc.row+1 <= 15)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],2)) // for South
			{
				SET_BIT(cell_detail[cell.loc.row+1][cell.loc.col],0 );

			}
			else if ( flood_map[cell.loc.row+1][cell.loc.col] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row+1][cell.loc.col]) <<8 | 1<<1 ;
			}

		}

		if (cell.loc.col+1 <= 15)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],1)) // for East
			{
				SET_BIT(cell_detail[cell.loc.row][cell.loc.col+1],3 );
			}
			else if ( flood_map[cell.loc.row][cell.loc.col+1] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row][cell.loc.col+1]) <<8 | 1<<0 ;
			}
		}
		break;


		case West_R:
		state_walls = ((state_walls>>1)&0x07)| (GET_BIT(state_walls,0)<<3) ; 	 // for o -> west
		cell_detail[cell.loc.row][cell.loc.col] |= state_walls;
		if (cell.loc.col+1 <= 4)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],1)) // for East
			{
				SET_BIT(cell_detail[cell.loc.row][cell.loc.col+1],3 );
			}
			else if ( flood_map[cell.loc.row][cell.loc.col+1] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row][cell.loc.col+1]) <<8 | 1<<2 ;
			}
		}

		if ( cell.loc.row+1 <= 15)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],2)) // for South
			{
				SET_BIT(cell_detail[cell.loc.row+1][cell.loc.col],0 );

			}
			else if ( flood_map[cell.loc.row+1][cell.loc.col] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row+1][cell.loc.col]) <<8 | 1<<3 ;
			}

		}

		if (cell.loc.row-1 >= 0 )
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],0) ) // for North
			{
				SET_BIT(cell_detail[cell.loc.row-1][cell.loc.col],2 );
			}
			else if ( flood_map[cell.loc.row-1][cell.loc.col] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row-1][cell.loc.col]) <<8 | 1<<1 ;
			}
		}
		if (cell.loc.col-1>= 0)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],3)) // for West
			{
				SET_BIT(cell_detail[cell.loc.row][cell.loc.col-1],1 );

			}
			else if ( flood_map[cell.loc.row][cell.loc.col-1] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row][cell.loc.col-1]) <<8 | 1<<0 ;
			}
		}
		break;


		case South_R:
		state_walls = ((state_walls&0x03)<<2)| ((state_walls&0x0C)>>2) ;  // for o ->South
		cell_detail[cell.loc.row][cell.loc.col] |= state_walls ;
		if (cell.loc.row-1 >= 0 )
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],0) ) // for North
			{
				SET_BIT(cell_detail[cell.loc.row-1][cell.loc.col],2 );
			}
			else if ( flood_map[cell.loc.row-1][cell.loc.col] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row-1][cell.loc.col]) <<8 | 1<<2 ;
			}
		}

		if (cell.loc.col+1 <= 15)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],1)) // for East
			{
				SET_BIT(cell_detail[cell.loc.row][cell.loc.col+1],3 );
			}
			else if ( flood_map[cell.loc.row][cell.loc.col+1] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row][cell.loc.col+1]) <<8 | 1<<3 ;
			}
		}

		if (cell.loc.col-1>= 0)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],3)) // for West
			{
				SET_BIT(cell_detail[cell.loc.row][cell.loc.col-1],1 );

			}
			else if ( flood_map[cell.loc.row][cell.loc.col-1] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row][cell.loc.col-1]) <<8 | 1<<1 ;
			}
		}
		if ( cell.loc.row+1 <= 15)
		{
			if ( GET_BIT(cell_detail[cell.loc.row][cell.loc.col],2)) // for South
			{
				SET_BIT(cell_detail[cell.loc.row+1][cell.loc.col],0 );

			}
			else if ( flood_map[cell.loc.row+1][cell.loc.col] <=  ((loc_and_min>>8)&0xFF) )
			{
				loc_and_min =  ( flood_map[cell.loc.row+1][cell.loc.col]) <<8 | 1<<0 ;
			}

		}
		break;
	}
	return loc_and_min ;

	// u16 - >  "8 -> distance  " : "8bit -> direction"
	// now find minimum distance and put it in the highest 8 bits and direction in the lowest 8 bits

}


void reset_flag(void)
{
	flag = 1 ;
}

