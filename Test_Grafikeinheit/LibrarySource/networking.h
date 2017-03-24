#ifndef NETWORKING_H
#define NETWORKING_H
/*****************************************************************************/
/*  Header     : Networking                                     Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : This module manages the lowlevel network communication      */
/*                                                                           */
/*  Procedures : InitializeNetwork      Initializes the Networklayer         */
/*               CleanupNetwork         Shuts down the networking            */
/*               CreateConnection       Creates a connection                 */
/*               FreeConnection         Deletes a connection                 */
/*               SendMessageTo          Sends datas to a connection          */
/*               SendBroadcastMessage   Sends datas to everybody             */
/*               ReadMessage            Receives Data, if availlable         */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 03.04.2002  IO Created                                      */
/*               28.03.2004  IO Added some typecasts in SendMessageTo() and  */
/*                              ReadMessage() for cpp compatibility          */
/*                              Added cpp compatibility in header            */
/*                                                                           */
/*               28.04.2004  IO Added support for arbitrary Telegrammlengths */
/*                              (MaxTelegramLength)                          */
/*               24.10.2009  IO Ported to QT, added support for Broadcast    */
/*                                                                           */
/*  File       : Networking.h                                                */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */

/* module constant declaration  */
#define IGNORE_MAXIMAL_RECOMENDED_TELEGRAMLENGTH 0x55000000u
#define IGNORE_MAXIMAL_RECOMENDED_TELEGRAMLENGTH_MASK 0xFF000000u
#define TELEGRAMLENGTH_MASK 0x00FFFFFFu

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


/* module type declaration      */

/* module data declaration      */

/* module procedure declaration */
#ifdef __cplusplus
extern "C" {
#endif
extern char ** InitializeNetwork(char *MyIpAddress, unsigned int MaximumTelegramLength, unsigned int NewLocalPort, unsigned int NewRemotePort);
extern void CleanupNetwork(void);
extern int  CreateConnection( char *InetAddress);
extern void FreeConnection( int Index );
extern int SendMessageTo(int Connection, void *Message, unsigned int Length);
extern int SendBroadcastMessage(void *Message, unsigned int Length);
extern int ReadMessage(int *Index, void *Message, int MaxLength, int *ReceivedLength);
extern int GetHostAddress(int Index, char *IPAddress, int MaxLength);
#ifdef __cplusplus
}
#endif


/*****************************************************************************/
/*  End Header  : Networking                                                 */
/*****************************************************************************/
#endif

