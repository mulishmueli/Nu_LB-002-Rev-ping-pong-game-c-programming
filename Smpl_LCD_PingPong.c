//
// Smpl_LCD_PingPong : rendering a circle bouncing around between two bars
//
#include <stdio.h>	
#include <string.h>
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "NUC1xx-LB_002\LCD_Driver.h"
#include "2D_Graphic_Driver.h"


#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvADC.h"
#include <stdlib.h>		
#include "scankey.h"
#include "Seven_Segment.h"





#define X0 15       // Circle initial X 
#define Y0 32       // Circle initial Y
#define R_BAR_X 120 // Right-Bar X-position
#define L_BAR_X 7   // Left-Bar  X-position

unsigned char DisplayBuf [128*8];

// display an integer on four 7-segment LEDs
void seg_display(int16_t value)
{
  int8_t digit;
	digit = value / 1000;
	close_seven_segment();
	show_seven_segment(3,digit);
	DrvSYS_Delay(5000);
			
	value = value - digit * 1000;
	digit = value / 100;
	close_seven_segment();
	show_seven_segment(2,digit);
	DrvSYS_Delay(5000);

	value = value - digit * 100;
	digit = value / 10;
	close_seven_segment();
	show_seven_segment(1,digit);
	DrvSYS_Delay(5000);

	value = value - digit * 10;
	digit = value;
	close_seven_segment();
	show_seven_segment(0,digit);
	DrvSYS_Delay(5000);
}

int32_t main (void)
{
	int dirX, dirY;
	int RightScore=0, LeftScore=0, movX, movY,j,i,LeftLineUpperEdge,LeftLineLowerEdge,RightLineUpperEdge,RightLineLowerEdge;
	uint8_t r;
	uint8_t xc, yc,yl,yr,number;
	UNLOCKREG();
	DrvSYS_Open(48000000);
	LOCKREG();
	
	Initial_panel();  
	clr_all_panel();	                        
	xc = X0;    // circle center x
	yc = Y0;    // circle center y
	r = 5;    // circle radius
	yl = Y0;
	yr = Y0;
	movX = 3;  // x movement
	movY = 3;  // y movement
	dirX = 1;  // x direction
	dirY = 1;  // y direction
	
	
	LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,1);
	LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,1);
	
	
	while(1) 
		{
			
			number = Scankey();		
	
		if (number!=0)		
		{
			if (number==3)
			{
				LineBresenham(R_BAR_X,yr-7,R_BAR_X,yr+7,0);
				yr--;
				LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,1);
			}

			if (number==9)
			{
				LineBresenham(R_BAR_X,yr-7,R_BAR_X,yr+7,0);
				yr++;
				LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,1);
			}
			if (number==1)
			{
				LineBresenham(L_BAR_X,yl-7,L_BAR_X,yl+7,0);
				yl--;
				LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,1);
			}

			if (number==7)
			{
				LineBresenham(L_BAR_X,yl-7,L_BAR_X,yl+7,0);
				yl++;
				LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,1);
			}			
		
		}				


     CircleBresenham(xc, yc, r, 1); // draw a circle into display buffer
		 LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,1);   // draw a line
		 LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,1);   // draw a line			
	   draw_LCD(DisplayBuf);		     // from display buffer to LCD
//		close_seven_segment();
//		show_seven_segment(3,LeftScore);
//		DrvSYS_Delay(500);
//    close_seven_segment();
//		show_seven_segment(0,RightScore);
		
//		DrvSYS_Delay(500);
		
		
	   DrvSYS_Delay(20000);         // adjustable delay for vision			
	   CircleBresenham(xc, yc, r, 0); // erase a circle from display buffer
     LineBresenham(R_BAR_X,yr-7,R_BAR_X,yr+7,0);   // draw a line
     LineBresenham(L_BAR_X,yl-7,L_BAR_X,yl+7,0);   // draw a line			
			
	   xc = xc + dirX * movX;         // change x of circle center
     yc = yc + dirY * movY;         // change y of circle center
     LeftLineUpperEdge=yl-7;
		 LeftLineLowerEdge=yl+7;
		 RightLineUpperEdge=yr-7;
		 RightLineLowerEdge=yr+7;

	 	 if ((xc-r-movX) < L_BAR_X)
		 {
			 //if(movY>0 || movY<63)
			 //{
					if((yc+2) > LeftLineUpperEdge && (yc-2) < LeftLineLowerEdge)
					{				
						dirX=1;  // boundary check for changing direction if on left bar									
					}
					else
				{
				 RightScore++;
				CircleBresenham(xc, yc, r, 0);
				LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,0);
			 LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,0);
				
				 yl=32;
				 xc=13;
				 yc=32;
				 yr=32;
			  	dirX=1;
				
				 
			 CircleBresenham(xc, yc, r, 1);
				LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,1);
				 LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,1);
					 clr_all_panel();
					 
			 draw_LCD(DisplayBuf);
					while(number!=5)
			 {
		close_seven_segment();
		show_seven_segment(0,RightScore);
		DrvSYS_Delay(500);
    close_seven_segment();
		show_seven_segment(3,LeftScore);
		DrvSYS_Delay(500);
				 
				number=Scankey();
				 
				 
			 }
			 	   CircleBresenham(xc, yc, r, 0); // erase a circle from display buffer

			}
		   //}
		 
			
		}				
		    		
	   
		 else if ((xc+r+movX) > R_BAR_X)
		 {
//			 if((yc) > RightLineUpperEdge)
//			 {
//				if ((yc) < RightLineLowerEdge)
//				{
					dirX=-1;  // boundary check for changing direction if on right bar
//				}					
//			 }
		 }
	   else if ((yc-r)<0)  dirY=1;  
	   else if ((yc+r)>63) dirY=-1;					
		
			
//			
//	   CircleBresenham(xc, yc, r, 1); // draw a circle into display buffer
//		 			
//	   draw_LCD(DisplayBuf);		     // from display buffer to LCD
//	   DrvSYS_Delay(50000);         // adjustable delay for vision			
//	   CircleBresenham(xc, yc, r, 0); // erase a circle from display buffer
//     
//		
		
		
//		if      ((xc-r-movX) < L_BAR_X)
//		 {
//			 if(yc-yl>2 || yl-yc>2)
//			 {
//				 RightScore++;
//				 CircleBresenham(xc, yc, r, 0);
//      LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,0);
//	     LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,0);
//			
//			 yl=32;
//			 xc=13;
//			 yc=32;
//				 yr=32;
//				 
//		 CircleBresenham(xc, yc, r, 1);
//      LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,1);
//	     LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,1);
//				 clr_all_panel();
//				 
//			 draw_LCD(DisplayBuf);
//			 
//			 while(number!=5)
//			 {
//				number=Scankey();
//			 }
//			 	   CircleBresenham(xc, yc, r, 0); // erase a circle from display buffer

//		 }
//	 }
//			 else
//				 {
//			     dirX=1;  // boundary check for changing direction
//		 }
//	   if ((xc+r+movX) > R_BAR_X)
//		 {
//			 if(yc-yr>2 || yr-yc>2)
//			 {
//			 
//			 LeftScore++;
//				 	 CircleBresenham(xc, yc, r, 0);
//      LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,0);
//	     LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,0);
//			
//			 yr=32;
//			 yl=32;
//			 xc=115;
//			 yc=32;
//			
//			 CircleBresenham(xc, yc, r, 1);
//				 LineBresenham(R_BAR_X,yr-7, R_BAR_X,yr+7,1);
//	LineBresenham(L_BAR_X,yl-7, L_BAR_X,yl+7,1);
//	
//			 draw_LCD(DisplayBuf);
//			 while(number!=5)
//			 {
//				number=Scankey();
//			 }
//			 	   CircleBresenham(xc, yc, r, 0); // erase a circle from display buffer

//		   }
//		 }
//			 else
//			 {
//				 dirX=-1;
//			 }
//  		 
//		
//		
//	   xc = xc + dirX * movX;         // change x of circle center
//     yc = yc + dirY * movY;         // change y of circle center
//		
//		
//     if((xc-r-movX )<7) dirX=1;
//			 else if(xc+r+movX>120) dirX=-1;
//	   if ((yc-r-movY)<0)  dirY=1;  
//	   else if ((yc+r+movY)>63) dirY=-1;
//		 
//     
//		 
//		 
//	     
		
//		for(j=0;j<7;j++)
//		{
  	  close_seven_segment();
	 		show_seven_segment(3,LeftScore);
	    DrvSYS_Delay(10000);
			close_seven_segment();
	 		show_seven_segment(0,RightScore);
	    DrvSYS_Delay(5000);
//		}	   
		
		 if ((yl-7)<0)  yl=7;  
	   else if ((yl+7)>63) yl=55;
		 if ((yr-7)<0)  yr=7;  
	   else if ((yr+7)>63) yr=55;		
   }
}
		
