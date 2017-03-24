/******************************************************************************/
/*  Module:       Test_Grafikeinheit                            Version 1.0   */
/******************************************************************************/
/*                                                                            */
/*  Beschreibung: Das Ziel von diesem Programm ist das Erlernen des Umgangs   */
/*                mit den zur Verfügung gestellten Grafikfunktionen.          */
/*                Zu Beginn wird die Einheit initialiersiert. Anschliessend   */
/*                werden zum Test verschiedene Elemente in unterschiedlichen  */
/*                Grössen, Formen und Positionen dargestellt.                 */
/*                                                                            */
/*  Funktion:     main (Testzwecke)                                           */
/*                                                                            */
/*  Autor:        Philippe Fankhauser                                         */
/*                                                                            */
/*  Verlauf:      24.03.2017  Erstellen der Vorlage                           */
/*                                                                            */
/*  Datei:        Test_Grafikeinheit.c                                        */
/*                                                                            */
/******************************************************************************/

/* Importieren der externen Header-Dateien */
#include "error.h"
#include "window.h"
#include "communication.h"

/* Importieren von den Bibliothekfunktionen */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/* Deklaration der lokalen Varibeln */


/************************************************************************************/
/*  Funktion:       main                                                            */
/************************************************************************************/
/*                                                                                  */
/*  Beschreibung:      Testen der Grafikfunktionen                                  */
/*                                                                                  */
/*  Type:              Global                                                       */
/*                                                                                  */
/*  Eingabeparameter:  argc   Anzahl von Argumente in der Kommandozeile des Systems */
/*                     argv   Vektor mit den Argumenten                             */
/*                                                                                  */
/*  Ausgabeparameter:  Fehlercodes (falls kein Fehler null)                         */
/*                                                                                  */
/*  Autor:             Philippe Fankhauser                                          */
/*                                                                                  */
/*  Verlauf:           24.03.2017  Implementieren der Funktionen                    */
/*                                                                                  */
/************************************************************************************/

int gfxmain(int argc, char* argv[], const char *ApplicationPath)
{
   /* Funktionsdaten */
   ColorType Col;
   ColorType ColBlack;
   ColorType ColRed;
   ColorType ColGreen;
   ColorType ColYellow;
   ColorType TempCol;

   /* Definition der Farben */
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
   InitGraphic(-1,-1);  /* Negativer Wert -> Vollbildschirm */
   printf("Startbildschirm im Vollbild intialiseren\n");
   printf("Hoehe: %d");
   printf("Breite: %d");


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

       /*WaitMs(250);

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

       */
   }
}

/*****************************************************************************/
/*  End Module  : Mainmodule                                                 */
/*****************************************************************************/



