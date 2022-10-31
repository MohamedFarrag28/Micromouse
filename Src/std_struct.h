
#ifndef STD_STRUCT_H_
#define STD_STRUCT_H_


typedef union
 {
	 struct
	 {
		u8 row :4 ;
		u8 col :4 ;
	
	 }loc;
	 
	 u8 location ;	//	xxxx(col) :  xxxx(row)
 }Position ;
 
 
 
 
#endif
