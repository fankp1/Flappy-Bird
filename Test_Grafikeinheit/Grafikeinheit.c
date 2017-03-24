/*****************************************************************************/
/*  Module     : Grafikeinheit                                  Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : This is a short dmeo for the usage of the graphics module   */
/*                                                                           */
/*  Procedures : main   Mainprocedure for the demo                           */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 03.05.2002  IO Created                                      */
/*                                                                           */
/*  File       : wormmain.c                                                  */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
#include "error.h"
#include "window.h"
#include "communication.h"
//#include "ConsoleFunctions.h"


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/* module constant declaration */
#define PI 3.1416

/* module type declaration */
typedef struct ImageMap {
   int Height;
   int Width;
   int MapSizeInBytes;
   ColorType *BasePtr;
   ColorType **ImageMap;
} ImageMapType;


/* module data declaration */
static volatile int Tick = 0;

int x_Kord = 0;
int y_Kord = 0;
int y_Kord_alt = 0;

int x_1 = 200;
int x_1alt;

int x_2 = 300;
int x_2alt;

int x_3 = 400;
int x_3alt;

int x_4 = 500;
int x_4alt;

int aenderung = 0;

/* module procedure declaration */
static void TimerHandler (void *Param);
void DrawTransformedImage(int x, int y, float Angle, float ScaleX, float ScaleY, int Image);
ImageMapType *CreateImageMap(int MyImage);
void DestroyImageMap(ImageMapType * ImageMap);


void DoSomething(void *Paras);
struct ThreadInfo {
   volatile int DoStop;
   volatile int Stopped;
            int x, y;
            int DelayTime;
};

static struct ThreadInfo Thread_1 = {0, 0, 222, 222, 100};
static struct ThreadInfo Thread_2 = {0, 0, 277, 277, 50};


/*****************************************************************************/
/*  Procedure   : main                                                       */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Just a short demonstration for the usage of the graphic    */
/*                module                                                     */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : argc           Number of commandline arguments from system */
/*                argv           Vectro with commandline arguments           */
/*                                                                           */
/*  Output Para : Errorcode to system, 0 on no error                         */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int gfxmain(int argc, char* argv[], const char *ApplicationPath)
{
   /* procedure data */
   int x = 0;
   ColorType Col;
   ColorType ColBlack;
   ColorType ColRed;
   ColorType ColGreen;
   ColorType ColYellow;
   ColorType TempCol;

   TextDimensionType TextExtension;

   int LoadedImage;
   int MyImage;
   int SavedImage;
   int LastX, LastY;
   int Key = 0;
   float ColorAngle = 0;

   char TextBuffer[100];


   /* procedure code */


   /* Set the color information */
   Col.Red   = 0xff;
   Col.Green = 0;
   Col.Blue  = 0;
   Col.Alpha = 0xff;

   ColRed.Red   = 0xff;
   ColRed.Green = 0;
   ColRed.Blue  = 0;
   ColRed.Alpha = 0xff;

   ColGreen.Red   = 0;
   ColGreen.Green = 0xff;
   ColGreen.Blue  = 0;
   ColGreen.Alpha = 0xff;

   ColYellow.Red   = 0xff;
   ColYellow.Green = 0xff;
   ColYellow.Blue  = 0;
   ColYellow.Alpha = 0xff;

   ColBlack.Red   = 0;
   ColBlack.Green = 0;
   ColBlack.Blue  = 0;
   ColBlack.Alpha = 0xff;

   /* Grafikeinheit initalisieren */
   InitGraphic(500, 500);  /* Negativer Wert -> Vollbildschirm */
   DrawFilledRectangle(0,0,500,500,COL_LIGHTBLUE,1);

   while(1)
   {

       /* double x = 0.1; /*
       for(double i=0.1; i<(2*3.14); i+x)
       {
           DrawEmptyRectangle(i,sin(i),20,20,COL_BLUE,1);
           DrawFilledCircle(i+50,i+50,1,1,COL_BLUE,10);
           WaitMs(50);
       }
       //DrawFilledRectangle(0, 0, 20, 20, COL_RED, 1) ;
       DrawFilledCircle(50,50,1,1,COL_BLUE,1);
       */

       /*
       while(IsKeyPressReady()) {
          Key = GetKeyPress();
          if ((Key == W_KEY_CURSOR_UP))
          {
             y_Kord = y_Kord - 5;
          }
          if ((Key == W_KEY_CURSOR_DOWN))
          {
             y_Kord = y_Kord + 5;
          }
          if ((Key == W_KEY_CURSOR_LEFT))
          {
             x_Kord = x_Kord -5;
          }
          if ((Key == W_KEY_CURSOR_RIGHT))
          {
             x_Kord = x_Kord + 5;
          }

          if(x_Kord > 30)
          {
               DrawFilledCircle(x_Kord,y_Kord,50,50,COL_GREEN,1);
          }
          else
          {
              if(x_Kord > 20)
              {
                   DrawFilledCircle(x_Kord,y_Kord,50,50,COL_BLUE,1);
              }
              else
              {
                  DrawFilledCircle(x_Kord,y_Kord,50,50,COL_RED,1);
              }

          }
       }
       */

       WaitMs(250);

        PlaySoundOnce("\\sound2.wav");

       while(IsKeyPressReady())
       {
          Key = GetKeyPress();

          if ((Key == W_KEY_CURSOR_UP))
          {
             y_Kord_alt = y_Kord;
             y_Kord = y_Kord - 20;
             aenderung = 1;
          }   

          if((Key == W_KEY_ESCAPE))
          {
              CloseGraphic();
          }
       }

       x_Kord = 20;

       if(aenderung == 0)
       {
           y_Kord_alt = y_Kord;
           y_Kord = y_Kord + 20;
       }

       aenderung = 0;

       if(x_1 < 10)
       {
           DrawFilledRectangle(x_1,0,50,200,COL_LIGHTBLUE,1);
           DrawFilledRectangle(x_1,300,50,200,COL_LIGHTBLUE,1);
           x_1 = 500;
       }
       if(x_2 < 10)
       {
           DrawFilledRectangle(x_2,0,50,100,COL_LIGHTBLUE,1);
           DrawFilledRectangle(x_2,400,50,100,COL_LIGHTBLUE,1);
           x_2 = 600;
       }
       if(x_3 < 10)
       {
           DrawFilledRectangle(x_3,0,50,300,COL_LIGHTBLUE,1);
           DrawFilledRectangle(x_3,200,50,300,COL_LIGHTBLUE,1);
           x_3 = 800;
       }
       if(x_4 < 10)
       {
           DrawFilledRectangle(x_4,0,50,250,COL_LIGHTBLUE,1);
           DrawFilledRectangle(x_4,250,50,250,COL_LIGHTBLUE,1);
           x_4 = 900;
       }

       x_1alt = x_1;
       x_1 = x_1 - 10;

       x_2alt = x_2;
       x_2 = x_2 - 10;

       x_3alt = x_3;
       x_3 = x_3 - 10;

       x_4alt = x_4;
       x_4 = x_4 - 10;



       DrawFilledCircle(x_Kord,y_Kord,19,19,COL_BLACK,1);
       DrawFilledCircle(x_Kord,y_Kord_alt,19,19,COL_LIGHTBLUE,1);

       DrawFilledRectangle(x_1,0,50,200,COL_GREEN,1);
       DrawFilledRectangle(x_1alt,0,50,200,COL_LIGHTBLUE,1);
       DrawFilledRectangle(x_1,300,50,200,COL_GREEN,1);
       DrawFilledRectangle(x_1alt,300,50,200,COL_LIGHTBLUE,1);

       DrawFilledRectangle(x_2,0,50,100,COL_RED,1);
       DrawFilledRectangle(x_2alt,0,50,100,COL_LIGHTBLUE,1);
       DrawFilledRectangle(x_2,150,50,350,COL_RED,1);
       DrawFilledRectangle(x_2alt,150,50,350,COL_LIGHTBLUE,1);

       DrawFilledRectangle(x_3,0,50,300,COL_DARKYELLOW,1);
       DrawFilledRectangle(x_3alt,0,50,300,COL_LIGHTBLUE,1);
       DrawFilledRectangle(x_3,350,50,150,COL_DARKYELLOW,1);
       DrawFilledRectangle(x_3alt,350,50,150,COL_LIGHTBLUE,1);

       DrawFilledRectangle(x_4,0,50,250,COL_ORANGE,1);
       DrawFilledRectangle(x_4alt,0,50,250,COL_LIGHTBLUE,1);
       DrawFilledRectangle(x_4,350,50,150,COL_ORANGE,1);
       DrawFilledRectangle(x_4alt,350,50,150,COL_LIGHTBLUE,1);
   }
}






/*****************************************************************************/
/*  End Module  : Mainmodule                                                 */
/*****************************************************************************/



