/* general control */

/*****************************************************************************/
/*  Module     : ErrorHandler                                   Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : This Module provides some mocros and functionality for      */
/*               errorhandling                                               */
/*                                                                           */
/*  Procedures : HandleError            Handles the given Error              */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 09.04.2002  IO Created                                      */
/*                                                                           */
/*  File       : error.c                                                     */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

/* module constant declaration */

/* module type declaration */

/* module data declaration */

/* module procedure declaration */

/*****************************************************************************/
/*  Procedure   : ErrorHandler                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Handles all internal Errors                                */
/*                Aborts program on Fatal errors                             */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Message      Message to Display                            */
/*                FileName     Name of File producing the error              */
/*                LineNumber   Number of line causing the error              */
/*                FunctionName Name of function causing the error            */
/*                Level        Severity of the error                         */
/*                                                                           */
/*  Output Para : Pinter to created worm                                     */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 09.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void ErrorHandler(const char *Message, const char *FileName, int LineNumber, const char *FunctionName, int Level)
{
   /* procedure data */
   char *ErrorType = "Error";

   /* procedure code */

   switch (Level) {
      case ERR_FATAL:     ErrorType = "Fatal error";   break;
      case ERR_NON_FATAL: ErrorType = "Error";         break;
      case ERR_WARNING:   ErrorType = "Warning";       break;
      default:            ErrorType = "Unknown error"; break;
   }

   printf("\n%s: %s,\n", ErrorType, Message);
   printf("in file %s in function %s at line %d)\n", FileName, FunctionName, LineNumber);
   system("Pause");
   /* quit program on fatal errors */
   if (Level == ERR_FATAL) {
      exit(0);
   }
}
/*****************************************************************************/
/*  End         : ErrorHandler                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  End Module  : ErrorHandler                                               */
/*****************************************************************************/














