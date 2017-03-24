/* general control */

/*****************************************************************************/
/*  Module     : Main                                           Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : This module starts the whole application                    */
/*                                                                           */
/*  Procedures : main                   Entrypoint of the whole application  */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 24.10.2009  IO Created while porting to QT                  */
/*                                                                           */
/*  File       : main.cpp                                                    */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
#include "gui_main.h"

#include <QtGui>
#include <QApplication>

/* module constant declaration */

/* module type declaration */

/* module data declaration */

/* module procedure declaration */

/*****************************************************************************/
/*  Procedure   : main                                                       */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Starts the whole Qt framework, also parses some            */
/*                commandline options                                        */
/*                                                                           */
/*  Type        : global                                                     */
/*                                                                           */
/*  Input Para  : argc, argv     Standard arguments                          */
/*                                                                           */
/*  Output Para : Errorcode to operating system                              */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 24.10.2009  IO  Created (During port to QT)                */
/*                                                                           */
/*                01.01.2010  IO Added debugging support                     */
/*                                                                           */
/*****************************************************************************/
int main(int argc, char *argv[])
{
   /* procedure data */

   /* procedure code */

   /* Check for comandline Arguments */
    if (argc > 1) {

       /* Check for network-debug */
       /* -dn Filename */
       /* Attention, flags wil not be removed from argumentlist */
       /* before passing to Qt                                  */
       if (strcmp(argv[1], "-dn") == 0) {
          extern FILE *DebugOut;
          if( argc > 2) {
             DebugOut = fopen(argv[2], "w");

          }
          if (DebugOut == NULL) {
             DebugOut = stdout;
          }
       }
    }

    /* Create Qt core... */
    QApplication a(argc, argv);

    /* ...and our window... */
    MainGui w;
    //w.show();
    /* ...and start the whole thing */
    return a.exec();
}
/*****************************************************************************/
/*  End         : main                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*  End Module  : Main                                                       */
/*****************************************************************************/

