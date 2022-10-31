

//pseudo code "MODIFIED FLOOD FILL"
/*
0 - initiallize "create" a flood_map from our source to destination"four cells" .

1- push current cell "first cell" to our stack 'stack1' "singly or douply linked list or array using malloc (depend on our implementation)" // use it if we put "update function"
2- make a variable (cur_cell) of struct which has x&y of current cell

2- while(cur_cell != destination)
	*  make wall update for cur_cell  (and adjacent cells "if possible"->wwwe handle this in wall_update function)  -> update wall map  .
	*  get the lowest distance value  of neighboring cells .
		* change cur_cell to next target which is the neighbor which has lowest distance value ;  //************************************************ w8
		--------------------------------
		If distance value in this cell not equal to	1 + minimum value of its open neighbor and cell	is not destination cell ? 
		TRUE ->  call updade function
						---->  refill function or / update fun - >
						while (stack1.embty()!= TRUE )
										* take then pop cell from the stack1
										If distance value in this cell not equal to	1 + minimum value of its open neighbor and cell	is not destination cell ?
										TRUE -> Change the cell to (1 + the minimum value of its open neighbors) and
												push all of the cell's open neighbors onto the stack1 to be checked
								
		FALSE -> pop/Do nothing
				
	* push cur_cell to stack1 to continue 
	* move to cur_cell

*/

#include<stdio.h>
#include <stdlib.h>
#include "std_struct.h"
 #include "doubly.h"

 
#define GET_BIT(VAR,BIT)		 ((VAR>>BIT) & 1)
#define SET_BIT(VAR,BIT)		 (VAR |= (1<<BIT))

														//8		4		2	1
														//x'w'  x'S'  x'E'  x'N' 
u8 flood_map[5][5] ;
enum orientation{North=1,East,West=8,South=4};

u8 cell_detail[5][5]  = { { 9,1, 1, 1, 3  },
						  { 8 ,0, 0, 0, 2 },
						  { 8 ,0, 0, 0, 2 },
						  { 8 ,0, 0, 0, 2 },
						  { 12 ,4, 4, 4, 6}} ;


		
/*
u8 cell_detail[16][16]  = { { 9, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 3 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, x, x ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, x, x ,0, 0, 0, 0, 0, 0, 2 }, 
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 8 ,0, 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 2 },
							{ 12 ,4, 4, 4, 4, 4, 4, 4, 4 ,4, 4, 4, 4, 4, 4, 6 } };
*/
 

void refill_flood(Node *Header_q1,Node* Trailer_q1 ,Position src ,Position dest, u8 num_dest); 
u16 update_wall(Position cell,u8 state_walls, u8 orientation);


void main(void)
{
	Node* Header = (Node*)malloc(sizeof(Node)) ;
	Node* Trailer =(Node*)malloc(sizeof(Node)) ;
	
	Header->Next = Trailer ;
	Trailer->Prev = Header ;
	
	Position src ;
	Position dest ;
	Position cur_cell ;

	u16 min_and_dir = 0; 
	
	
	enum orientation orient = North ;
	
	u8 sensors_reads[5][5] = { { 0  ,0  , 0  , 0  , 0 },
						       { 9  ,8  , 2  , 8  , 0 },
							   { 10 ,0  , 0  , 9  , 0 },
							   { 8  ,0  , 0  , 0  , 0 },
							   { 14 ,0  , 0  , 0  , 0 }} ; 
	
	u8 sensors_reads2[5][5] = { { 0  ,0  , 0  , 0  , 0 },
							    { 0  ,0  , 0  , 0  , 0 },
							    { 0 ,0  , 3  , 0  , 0 },
							    { 0  ,0  , 2  , 0  , 0 },
							    { 0 ,0  , 0  , 0  , 0 }} ;
	
	src.loc.row = 4;
	src.loc.col = 4;
	dest.loc.row = 2 ;
	dest.loc.col = 2 ;
	
	cur_cell.loc.row =  src.loc.row ;
	cur_cell.loc.col = src.loc.col ;
	
	refill_flood(Header,Trailer, src , dest,1 ) ;
	
	printf("first Fill :\n") ;
	
	for (u8 i =0 ; i<5;i++)
	{
		for (u8 j =0 ; j<5;j++)
		{
			printf("%d " ,flood_map[i][j] );
		}
			printf("\n" );
	}
	printf("\n" );
	printf("starting at %d ---- %d \n" , cur_cell.loc.row ,cur_cell.loc.col);
	printf("ur dest at %d ---- %d \n" , dest.loc.row ,dest.loc.col);
	
	for (u8 i=0 ; i<2 ; i ++)
	{
	while(!((cur_cell.loc.row == dest.loc.row) && (cur_cell.loc.col == dest.loc.col)))				 
	{
		// read sensors here ^_^
		
		if(i==0)
		{
		min_and_dir = update_wall( cur_cell, sensors_reads[cur_cell.loc.row][cur_cell.loc.col],orient) ;
		}
		else
		{
			min_and_dir = update_wall( cur_cell, sensors_reads2[cur_cell.loc.row][cur_cell.loc.col],orient) ;
		}
		// print wall and min & direction
		printf("MIN &DIR ->> %x ---- %d  \n" , min_and_dir>>8&0xFF  ,min_and_dir&0x0F)   ;   //(min_and_dir)>>8)&(0xFF) , (min_and_dir&0x0F)
		printf("print Flood of this cell ---- > %d \n",  flood_map[cur_cell.loc.row][cur_cell.loc.col]) ;
		printf("walls : \n") ;
		for (u8 i =0 ; i<5;i++)
		{
			for (u8 j =0 ; j<5;j++)
			{
				printf("%d " ,cell_detail[i][j] );
			}
				printf("\n" );

		}
		printf("\n") ;

		
		
		if ( flood_map[cur_cell.loc.row][cur_cell.loc.col] !=  (1 +((min_and_dir>>8)&(0xFF)))  ) 
		{
			printf("\n ENTERED  REFILL \n" );
			 
				refill_flood(Header,Trailer, src ,dest,1 ) ;
			
			
		}
		
		switch((min_and_dir&0x0F))
		{
			case 1 :				// Front

				if(orient == North)
				{
					cur_cell.loc.row -= 1 ;
				}
				
				else if (orient == East)
				{
					cur_cell.loc.col += 1 ;
				}
				
				else if (orient == South)
				{
					cur_cell.loc.row += 1 ;
				}
					
				else if (orient == West)
				{
					cur_cell.loc.col -= 1 ;
				}
				// ++++++++++++ move front

			break;
			
			case 2 :				// Right
				if(orient == North)
				{
					cur_cell.loc.col += 1 ;
					orient = East ;
				}
				
				else if (orient == East)
				{
					cur_cell.loc.row += 1 ;
					orient = South ;
				}
				
				else if (orient == South)
				{
					cur_cell.loc.col -= 1 ;
					orient = West ;
				}
					
				else if (orient == West)
				{
					cur_cell.loc.row -= 1 ;
					orient = North ;
				}
				
				// ++++++++++++ move Right
				
			break ;
			
			
			case 4 :				// Back
				if(orient == North)
				{
					cur_cell.loc.row += 1 ;
					orient = South ;
				}
				
				else if (orient == East)
				{
					cur_cell.loc.col -= 1 ;
					orient = West ;
				}
				
				else if (orient == South)
				{
					cur_cell.loc.row -= 1 ;
					orient = North ;
				}
					
				else if (orient == West)
				{
					cur_cell.loc.col += 1 ;
					orient = East ;
				}
				
				// ++++++++++++ move Back
			break ;
			
			case 8 :				// Left
				if(orient == North)
				{
					cur_cell.loc.col -= 1 ;
					orient = West ;
				}
				
				else if (orient == East)
				{
					cur_cell.loc.row -= 1 ;
					orient = North ;
				}
				
				else if (orient == South)
				{
					cur_cell.loc.col += 1 ;
					orient = East ;
				}
					
				else if (orient == West)
				{
					cur_cell.loc.row += 1 ;
					orient = South ;
				}
				
				// ++++++++++++ move Left
			
			
			break;
		}
		
		
		printf("\n NEW ORTIENT - > %d \n",orient) ;
		printf("\n NEW CELL - > %d  ,,,, %d \n",cur_cell.loc.row,cur_cell.loc.col) ;
		printf("\n FLOOOOODE : \n");
		for (u8 i =0 ; i<5;i++)
	{
		for (u8 j =0 ; j<5;j++)
		{
			printf("%d " ,flood_map[i][j] );
		}
			printf("\n" );

	}
		
		printf("\n**********************************\n");
				
	}
	
	

	Position temp ;
	temp = src ;
	src = dest ;
	dest =temp ;
	refill_flood(Header,Trailer, src ,dest,1 ) ;
	
	printf("\n REACHEN CHANGEEEE : \n");
		for (u8 i =0 ; i<5;i++)
	{
		for (u8 j =0 ; j<5;j++)
		{
			printf("%d " ,flood_map[i][j] );
		}
			printf("\n" );

	}
		
		printf("\n**********************************\n");
	

	
	}
	
	
	
	/*
	u16 xx ;
	
	Position tesst_cell ;
	Node* Header = (Node*)malloc(sizeof(Node)) ;
	Node* Trailer =(Node*)malloc(sizeof(Node)) ;
	
	Header->Next = Trailer ;
	Trailer->Prev = Header ;
	
	Position src ;
	Position dest ;
	
	src.loc.row = 4;
	src.loc.col = 0;
	dest.loc.row = 2 ;
	dest.loc.col = 2 ;
	
	
	refill_flood(Header,Trailer, src , dest,1 ) ;
	for (u8 i =0 ; i<5;i++)
	{
		for (u8 j =0 ; j<5;j++)
		{
			printf("%d " ,flood_map[i][j] );
		}
			printf("\n" );

	}
	printf("Again \n" );
	
	tesst_cell.loc.row =4 ;
	tesst_cell.loc.col =0 ;
	xx=update_wall( tesst_cell, 14,North) ;
	//printf("min/Rob,direction -> %x \n",xx);
	
	
    tesst_cell.loc.row =3 ;
	tesst_cell.loc.col =0 ;
	xx=update_wall( tesst_cell, 8,North) ;
	//printf("min/Rob,direction -> %x \n",xx);

	
	
	tesst_cell.loc.row =2 ;
	tesst_cell.loc.col =0 ;
	xx=update_wall( tesst_cell, 10,North) ;
	
	
	//printf("min/Rob,direction -> %x \n",xx);

	
	
	tesst_cell.loc.row =1 ;
	tesst_cell.loc.col =0 ;
	xx=update_wall( tesst_cell, 9,North) ;
	
	//printf("min/Rob,direction -> %x \n",xx);


	
	tesst_cell.loc.row =1 ;
	tesst_cell.loc.col =1 ;
	xx=update_wall( tesst_cell, 8,East) ;
		//printf("min/Rob,direction -> %x \n",xx);

	
	
	tesst_cell.loc.row =1 ;
	tesst_cell.loc.col =2 ;
	xx=update_wall( tesst_cell,2 ,East) ;
		//printf("min/Rob,direction -> %x \n",xx);

	
	
	tesst_cell.loc.row =1 ;
	tesst_cell.loc.col =3;
	xx=update_wall( tesst_cell, 8,East) ;
	//printf("min/Rob,direction -> %x \n",xx);
	
	
	tesst_cell.loc.row =2 ;
	tesst_cell.loc.col =3 ;
	xx=update_wall( tesst_cell, 9,South) ;
	
		//printf("min/Rob,direction -> %x \n",xx);

	refill_flood(Header,Trailer, dest , src,1 ) ;
	
	tesst_cell.loc.row =2 ;
	tesst_cell.loc.col =2 ;
	xx=update_wall( tesst_cell, 3,West) ;
	printf("min/Rob,direction -> %x \n",xx);

	
	//8		4		2	1
	//x'w'  x'S'  x'E'  x'N'
	
	refill_flood(Header,Trailer, dest , src,1 ) ;
	for (u8 i =0 ; i<5;i++)
	{
		for (u8 j =0 ; j<5;j++)
		{
			printf("%d " ,flood_map[i][j] );
		}
			printf("\n" );

	}
	printf("walls : \n") ;
	for (u8 i =0 ; i<5;i++)
	{
		for (u8 j =0 ; j<5;j++)
		{
			printf("%d " ,cell_detail[i][j] );
		}
			printf("\n" );

	}
	*/

}



void refill_flood(Node *Header_q1,Node* Trailer_q1,Position src ,Position dest ,u8 num_dest )
{
	Position cur_position , temp  ;
	
	for (u8 i =0 ; i<5;i++)
	{
		for (u8 j =0 ; j<5;j++)
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
		cur_position =Pop(Header_q1 , Trailer_q1);
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
							&&cur_position.loc.col+1 <= 4  ) // for East
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
							&& cur_position.loc.row+1 <= 4 ) // for South
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
				push( Header_q1 , Trailer_q1 ,temp) ;
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
		case North:
		cell_detail[cell.loc.row][cell.loc.col] |= state_walls ;
		
		if ( cell.loc.row+1 <= 4)
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
				
				
				
		if (cell.loc.col+1 <= 4)	
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
	
			
		case East:						
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
		
		if ( cell.loc.row+1 <= 4)
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
		
		if (cell.loc.col+1 <= 4)	
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
		
		
		case West:
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
		
			if ( cell.loc.row+1 <= 4)
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
		
		
		case South:
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
		
		if (cell.loc.col+1 <= 4)	
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
			if ( cell.loc.row+1 <= 4)
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



