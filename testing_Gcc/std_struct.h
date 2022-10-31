
#ifndef STD_STRUCT_H_
#define STD_STRUCT_H_


typedef unsigned char			u8;
typedef unsigned short int		u16;
typedef unsigned long int		u32;
typedef unsigned long long		u64;

typedef char					s8;
typedef short int				s16;
typedef long int				s32;
typedef long long				s64;

typedef float					f32;
typedef double					d64;

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