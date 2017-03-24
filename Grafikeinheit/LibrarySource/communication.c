/* general control */

/*****************************************************************************/
/*  Module     : Communication                                  Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : This module manages the Networkcommunication                */
/*                                                                           */
/*  Procedures : InitializeCommunication  Initialize the communication       */
/*               ShutdownCommunication    Shutdown the communication         */
/*               CreateTelegram           Creates an empty message           */
/*               DeleteTelegram           Deletes a Message                  */
/*               SendTelegramTo           Sends a Message to a host          */
/*               CheckForTelegram         Reveives any Messages              */
/*               FindServer               Finds server by broadcast          */
/*               ConnectTo                Connects to a host as Slave        */
/*               IsConnected              Checks if handle is connected      */
/*               Disconnect               Disconnects from host              */
/*               AcceptConnection         Accept connections as master, is   */
/*                                        bufferoverflow protected           */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 04.04.2002  IO Created                                      */
/*               28.03.2004  IO Converted AcceptConnection() to              */
/*                              SecureAcceptConnection, does no longer       */
/*                              copy a fixed amount of characters with       */
/*                              strncpy, uses now a parameter                */
/*                              Corrected sleep() to Sleep() and included    */
/*                              windows.h                                    */
/*                              Corrected some Spellingerrors in errortexts  */
/*                              Added cpp compatibility in header            */
/*                                                                           */
/*               28.04.2004  IO Added support for arbitrary Telegrammlengths */
/*                              (MaxTelegrammLength)                         */
/*                                                                           */
/*                                                                           */
/*               22.10.2009  IO Ported to QT                                 */
/*                              Added support to find servers by broadcast   */
/*                              InitializeCommunication() now returns        */
/*                              pointer to array of local IP adresses        */
/*                                                                           */
/*  File       : Communication.c                                             */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
#include "communication.h"
#include "networking.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dbgmmgt.h"
#include "window.h"

/* module constant declaration */
#define TELEGRAM_ID ('H' + 'T'*0x100L + 'A'*0x10000L + ':'*0x1000000L)

/* virtual handle for broadcast adresses */
#define H_BROADCAST 19991

/* Define different telegram types */
enum TelegramTypeTag {
   TYPE_NONE       = 0,
   TYPE_MESSAGE    = 1,
   TYPE_CONNECT    = 2,
   TYPE_ACCEPT     = 3,
   TYPE_REJECT     = 4,
   TYPE_DISCONNECT = 5,
   TYPE_PING       = 6,
   TYPE_PONG       = 7,
   TYPE_ACCEPT_DISCONNECT = 8
} TelegramTypeTag;

/* module type declaration */

/* Telegram Header */
typedef struct TelegramHeader {
   unsigned long  ID;
   unsigned short Length;
   unsigned short Type;
} TelegramHeader;

/* Telegram structure */
typedef struct TelegramType {
   TelegramHeader   Header;
   union Data{
      char          Raw[1];   /* Raw data */
      MessageType   Message;  /* structured message */
   } Data;
} TelegramType;

/* Structure to keep track of state of connections */
typedef struct ConnectionInfo {
   int Handle;
   int Valid;
   int Connected;
} ConnectionInfo;

/* module data declaration */
/* to keep track of connections (A handle is an index in this table) */
static ConnectionInfo Connections[20];

static unsigned int MaxTelegrammLength = 0;

/* module procedure declaration */
static void SendHandshake(int Handle, short Command, int Extralength, char *Extradata);

/*
static void MyStrnCpy(char *Dest, const char *Src, int MaxLen) {
   while ((*Src != '\0') && (MaxLen > 0)) {
      *Dest++ = *Src++;
      MaxLen--;
   }
   *Dest++ = '\0';
}
*/
/*****************************************************************************/
/*  Procedure   : InitializeCommunication                                    */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Initializes the communications module                      */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : MaximumTelegramLength   Maximum allowed Telegrammlength    */
/*                Port                    Port to look for incomming messages*/
/*                                        also used als default destination  */
/*                                        port for outgoing messages         */
/*                                                                           */
/*  Output Para : Returns pointer to array of pointer to stings of local IP  */
/*                adresses, end of array is marked by a NULL-Pointer         */
/*                                                                           */
/*                Example:                                                   */
/*                   P = InitializeCommunication(200)                        */
/*                   P[0] points to string with first IP Address             */
/*                   P[1] points to string with second IP Address            */
/*                                                                           */
/*                   P[n] points to string with last IP Address              */
/*                   P[n+1] contains NULL to mark end of array               */
/*                                                                           */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*                23.10.2009  IO  Adapted to return all IP-Addresses         */
/*                                                                           */
/*****************************************************************************/
char **InitializeCommunicationWithPort(char *MyIpAddress, unsigned int MaximumTelegramLength, unsigned int NewLocalPort, unsigned int NewRemotePort)
{
   /* procedure data */
   int i;
   char Buffer[200];

   /* procedure code */
   MaxTelegrammLength = MaximumTelegramLength & TELEGRAMLENGTH_MASK;

   /* Check for correct Arguments */
   if (MaxTelegrammLength < (sizeof(TelegramType)+ 4)) {
      sprintf(Buffer, "Error in InitializeCommunicationWithPort, Telegrammlength must be at least %d Bytes", (sizeof(TelegramType)+ 4));
      CODING_ERROR(Buffer);
   }

   /* Initialize Handle table (No connections) */
   for (i = 0; i < 20; i++) {
      Connections[i].Handle = -1;
      Connections[i].Valid = 0;
      Connections[i].Connected = 0;
   }

   /* startup the network layer and return the IP-Adresses */
   return InitializeNetwork(MyIpAddress, MaxTelegrammLength, NewLocalPort, NewRemotePort);
}
/*****************************************************************************/
/*  End         : InitializeCommunication                                    */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : ShutdownCommunication                                      */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Shuts down the communication and releases all resources    */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void ShutdownCommunication(void)
{
   /* procedure data */
   int i;

   /* procedure code */

   /* free all handles */
   for (i = 0; i < 20; i++) {
      Connections[i].Handle = -1;
      Connections[i].Valid = 0;
      Connections[i].Connected = 0;
   }

   /* stop the network layer */
   CleanupNetwork();
}
/*****************************************************************************/
/*  End         : ShutdownCommunication                                      */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : CreateTelegram                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates an empty message telegram. User gets only a        */
/*                pointer to the Messagefield of a telegram                  */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : AdditionalLength  Additional length required for variable  */
/*                                  sized fields (e.g. Strings or texts)     */
/*                                                                           */
/*  Output Para : Pointer to messagefield or NULL on error                   */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
MessageType *CreateTelegram(unsigned int AdditionalLength)
{
   /* procedure data */
   TelegramType *NewTelegram;
   int           Length = sizeof(TelegramType) + AdditionalLength + 4;

   /* procedure code */

   /* Check for correct Arguments */
   if (AdditionalLength > MaxTelegrammLength - (sizeof(TelegramType)+ 4)) {
      CODING_ERROR("Error in CreateTelegram, Telegram too long");
   }

   /* Allocate space for a new telegramm */
   NewTelegram = (TelegramType *) malloc(Length);
   if (NewTelegram == NULL) {
      CODING_ERROR("Error in CreateTelegram, Out of memory");
   }

   /* initialize the telegram header */
   NewTelegram->Header.ID     = TELEGRAM_ID;
   NewTelegram->Header.Length = Length;
   NewTelegram->Header.Type   = TYPE_MESSAGE;

   /* Protection to detect overwrite of telegramlength */
   ((char *)NewTelegram)[Length-2] = 0x55;
   ((char *)NewTelegram)[Length-3] = 0xaa;

   /* return a pointer to the Messagefield of the Telegram */
   return &(NewTelegram->Data.Message);
}
/*****************************************************************************/
/*  End         : CreateTelegram                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DeleteTelegram                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Deletes a Message telegram                                 */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Message       Pointer to Messagetelegram to delete         */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DeleteTelegram(MessageType *Message)
{
   /* procedure data */
   TelegramType *TelegramToDelete;
   int           Length;

   /* procedure code */

   /* Check for correct Arguments */
   if (Message == NULL) {
      CODING_ERROR("Error in DeleteTelegram, Nullpointer");
   }

   /* find real start of telegram */
   TelegramToDelete = (TelegramType *) (((char *)Message) - sizeof(TelegramHeader));

   /* check for valid telegram */
   if (TelegramToDelete->Header.ID != TELEGRAM_ID) {
      CODING_ERROR("Error in DeleteTelegram, Invalid Message");
   }
   Length = TelegramToDelete->Header.Length;
   if (    (((char *)TelegramToDelete)[Length-2] != 0x55)
        || (((char *)TelegramToDelete)[Length-3] != (char)0xaa)) {
      CODING_ERROR("Error in DeleteTelegram, Invalid Message (Buffer overwriten?)");
   }
   /* release the memory of the telegram */
   free(TelegramToDelete);
}
/*****************************************************************************/
/*  End         : DeleteTelegram                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : SendTelegramTo                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sends a Message to the recipient                           */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Handle        Handle of connection to recipient            */
/*                Message       Message to send                              */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void SendTelegramTo(int Handle, MessageType *Message)
{
   /* procedure data */
   TelegramType *TelegramToSend;
   int           Length;

   /* procedure code */

   /* Check for correct Arguments */
   if (Message == NULL) {
      CODING_ERROR("Error in SendTelegramTo, Nullpointer");
   }
   if ((Handle < 0) || (Handle >= 20) || (Connections[Handle].Valid == 0)) {
      CODING_ERROR("Error in SendTelegramTo, Invalid Handle");
   }

   /* find real start of telegram */
   TelegramToSend = (TelegramType *) (((char *)Message) - sizeof(TelegramHeader));

   /* check for valid telegram */
   if (TelegramToSend->Header.ID != TELEGRAM_ID) {
      CODING_ERROR("Error in SendTelegramTo, Invalid Message");
   }
   Length = TelegramToSend->Header.Length;
   if (    (((char *)TelegramToSend)[Length-2] != 0x55)
        || (((char *)TelegramToSend)[Length-3] != (char)0xaa)) {
      CODING_ERROR("Error in SendTelegramTo, Invalid Message (Buffer overwriten?)");
   }
   /* send the telegramm */
   if (Connections[Handle].Valid != 0) {
      SendMessageTo(Connections[Handle].Handle, TelegramToSend, Length);
   }
   /* and free its memory */
   free(TelegramToSend);
}
/*****************************************************************************/
/*  End         : SendTelegramTo                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : CheckForTelegram                                           */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Checks for any valid telegram for any connection           */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Handle        Pointer to write Handle of sender            */
/*                                                                           */
/*  Output Para : Message received or NULL if none available                 */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
MessageType *CheckForTelegram(int *Handle)
{
   /* procedure data */
   TelegramType *ReceivedTelegram;
   int           Length;
   int           ReceivedLength;
   int           Sender;
   int i;
   int found;

   /* procedure code */

   /* Allocate Memory for Telegram */
   ReceivedTelegram = (TelegramType *) malloc(MaxTelegrammLength + sizeof(TelegramType)+ 4);
   if (ReceivedTelegram == NULL) {
      CODING_ERROR("Error in CheckForTelegram, Out of memory");
   }

   /* Try to receive a telegram as long as datas are availlable */
   do {
      /* Read the next message */
      ReadMessage(&Sender, ReceivedTelegram, MaxTelegrammLength, &ReceivedLength);

      /* Check if this may be a real telegram, ignore otherwise */
      if ((Sender >= 0) && (ReceivedTelegram->Header.ID == TELEGRAM_ID)) {
         Length = ReceivedTelegram->Header.Length;
         if (    (((char *)ReceivedTelegram)[Length-2] == 0x55)
              && (((char *)ReceivedTelegram)[Length-3] == (char)0xaa)) {

            /* Telegramstructure is ok, so analyze it */
            switch (ReceivedTelegram->Header.Type) {
               case TYPE_MESSAGE:
                  /* it is a message, check if we are connected to sender */
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       /* we are connected, everything ok, return telegram */
                       *Handle = Sender;
                       return &(ReceivedTelegram->Data.Message);
                     }
                  }
                  /* Not connected, reject telegram */
                  SendHandshake(Sender, TYPE_REJECT, 0, NULL);
                  FreeConnection(Sender);
                  break;

               case TYPE_DISCONNECT:
                  /* accept disconnect telegram */
                  SendHandshake(Sender, TYPE_ACCEPT_DISCONNECT, 0, NULL);

                  /* disconect corresponding handle */
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       Connections[i].Handle = -1;
                       Connections[i].Connected = 0;
                       break;
                     }
                  }
                  FreeConnection(Sender);
                  break;

               case TYPE_PING:
                  /* answer ping */
                  SendHandshake(Sender, TYPE_PONG, 0, NULL);
                  found = 0;
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       found = 1;
                       break;
                     }
                  }
                  /* only free connection if not connected */
                  if (found == 0) {
                     FreeConnection(Sender);
                  }
                  break;

               case TYPE_CONNECT:
               case TYPE_ACCEPT:
               case TYPE_PONG:
               case TYPE_REJECT:
               case TYPE_ACCEPT_DISCONNECT:
                  /* simply ignore this commands */
                  found = 0;
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       found = 1;
                       break;
                     }
                  }
                  /* only free connection if not connected */
                  if (found == 0) {
                     FreeConnection(Sender);
                  }
                  break;
            }
         }
      }

   } while (Sender >= 0);

   /* release allocated Telegrammemory, since not used */
   free(ReceivedTelegram);
   return NULL;
}
/*****************************************************************************/
/*  End         : CheckForTelegram                                           */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : FindServer                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Looks for hosts by broadcast                               */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Timeout  Timeout to wait for responses in ms               */
/*                                                                           */
/*  Output Para : Pointer to Array of found servers. Last entry marked with  */
/*                empty String in IP-Adress                                  */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 24.10.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
ServerInfoType *FindServer(unsigned int Timeout)
{
   /* procedure data */
   char          Buffer[500];
   TelegramType *ReceivedTelegram = (TelegramType *) Buffer;
   int           Length, ReceivedLength;
   int           Sender;
   int           i;
   int           found;

   static ServerInfoType Servers[21];
   int           ServersFound = 0;


   /* procedure code */

   /* Convert Timeout in 100ms slices */
   Timeout = Timeout / 100 + 1;

   /* Request a connect by the host */
   SendHandshake(H_BROADCAST, TYPE_PING, 0, NULL);

   /* Give servers a chance to react ... */
   WaitMs(100);

   /* Wait for a connect accept */
   do {
      /* read a telegram */
      ReadMessage(&Sender, ReceivedTelegram, 450, &ReceivedLength);

      /* check if it is a valid telegram */
      if ((Sender >= 0) && (ReceivedTelegram->Header.ID == TELEGRAM_ID)) {
         Length = ReceivedTelegram->Header.Length;
         if (    (((char *)ReceivedTelegram)[Length-2] == 0x55)
              && (((char *)ReceivedTelegram)[Length-3] == (char)0xaa)) {

            /* Telegram is OK, analyze further */
            switch (ReceivedTelegram->Header.Type) {

               case TYPE_PING:
                  /* reply ping */
                  SendHandshake(Sender, TYPE_PONG, 4, "ICH");
                  found = 0;
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       found = 1;
                     }
                  }
                  /* only free connection if not connected */
                  if (found == 0) {
                     FreeConnection(Sender);
                  }
                  break;

               case TYPE_PONG:
                  /* Got a reply, add in list of possible servers */
                  GetHostAddress(Sender, Servers[ServersFound].IPAddress, 20);
                  strncpy(Servers[ServersFound].Name, ReceivedTelegram->Data.Raw, 100);
                  if (ServersFound < 19) {
                     ServersFound++;
                  }


                  {
                     /* ignore accept */
                     found = 0;
                     for (i = 0; i < 20; i++) {
                        if (Connections[i].Handle == Sender) {
                          found = 1;
                          break;
                        }
                     }
                     /* only free connection if not connected */
                     if (found == 0) {
                        FreeConnection(Sender);
                     }
                  }
                  break;

               case TYPE_DISCONNECT:
                  /* got a disconnect, honor it */
                  SendHandshake(Sender, TYPE_ACCEPT_DISCONNECT, 0, NULL);
                  /* find corresponding connection */
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       /* and disconnect it */
                       Connections[i].Handle = -1;
                       Connections[i].Connected = 0;
                       break;
                     }
                  }
                  FreeConnection(Sender);
                  break;

               case TYPE_MESSAGE:
               case TYPE_ACCEPT:
               case TYPE_CONNECT:
               case TYPE_REJECT:
               case TYPE_ACCEPT_DISCONNECT:
                  /* ignore this commands */
                  found = 0;
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       found = 1;
                       break;
                     }
                  }
                  /* only free connection if not connected */
                  if (found == 0) {
                     FreeConnection(Sender);
                  }
                  break;
            }
         }
      }
      /* wait a bit and try againg, till timeout runs out */
      if (Sender < 0) {
         WaitMs(100);
         Timeout--;
      }

   } while (Timeout > 0);

   Servers[ServersFound].IPAddress[0] = '\0';
   return Servers;
}
/*****************************************************************************/
/*  End         : FindServer                                                 */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : ConnectTo                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Connects to given Host                                     */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Address       IP Addres  of host to connect to             */
/*                MyName        My name to identyfy myself on connect        */
/*                Timeout       Timeout to wait for responses in ms          */
/*                                                                           */
/*  Output Para : Handle of connection on success, -1 otherwise              */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int ConnectTo(char *Address, char *MyName, unsigned int Timeout)
{
   /* procedure data */
   char          Buffer[500];
   TelegramType *ReceivedTelegram = (TelegramType *) Buffer;
   int           Length, ReceivedLength;
   int           Handle, Sender;
   int i;
   int found;

   /* procedure code */

   /* Convert Timeout in 100ms slices */
   Timeout = Timeout / 100 + 1;

   /* create a connection */
   Handle = CreateConnection(Address);

   /* check if we got a connection */
   if (Handle < 0)  {
      return -1;
   }

   /* Request a connect by the host */
   SendHandshake(Handle, TYPE_CONNECT, strlen(MyName)+1, MyName);

   /* Give servers a chance to react ... */
   WaitMs(100);

   /* Wait for a connect accept */
   do {
      /* read a telegram */
      ReadMessage(&Sender, ReceivedTelegram, 450, &ReceivedLength);

      /* check if it is a valid telegram */
      if ((Sender >= 0) && (ReceivedTelegram->Header.ID == TELEGRAM_ID)) {
         Length = ReceivedTelegram->Header.Length;
         if (    (((char *)ReceivedTelegram)[Length-2] == 0x55)
              && (((char *)ReceivedTelegram)[Length-3] == (char)0xaa)) {

            /* Telegram is OK, analyze further */
            switch (ReceivedTelegram->Header.Type) {

               case TYPE_PING:
                  /* reply ping */
                  SendHandshake(Sender, TYPE_PONG, 0, NULL);
                  found = 0;
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       found = 1;
                     }
                  }
                  /* only free connection if not connected */
                  if (found == 0) {
                     FreeConnection(Sender);
                  }
                  break;

               case TYPE_ACCEPT:
                  /* Got a connect accept, check if from correct host */
                  if (Handle == Sender) {
                     found = 0;
                     /* check for new connection */
                     for (i = 0; i < 20; i++) {
                        if (Connections[i].Handle == Sender) {
                          Connections[i].Connected = 1;
                          found = 1;
                          // allready accepted, what shall we do ??
                          break;
                        }
                     }
                     if (found == 0) {
                        /* find a free handle */
                        for (i = 0; i < 20; i++) {
                           if (Connections[i].Valid == 0) {
                             /* and allocate it for this connection */
                             Connections[i].Handle = Handle;
                             Connections[i].Valid = 1;
                             Connections[i].Connected = 1;
                             return i;
                           }
                        }
                        // no free entry left, ignore
                        FreeConnection(Sender);
                     }
                  } else {
                     /* ignore accept */
                     found = 0;
                     for (i = 0; i < 20; i++) {
                        if (Connections[i].Handle == Sender) {
                          found = 1;
                          break;
                        }
                     }
                     /* only free connection if not connected */
                     if (found == 0) {
                        FreeConnection(Sender);
                     }
                  }
                  break;

               case TYPE_DISCONNECT:
                  /* got a disconnect, honor it */
                  SendHandshake(Sender, TYPE_ACCEPT_DISCONNECT, 0, NULL);
                  /* find corresponding connection */
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       /* and disconnect it */
                       Connections[i].Handle = -1;
                       Connections[i].Connected = 0;
                       break;
                     }
                  }
                  FreeConnection(Sender);
                  break;

               case TYPE_MESSAGE:
               case TYPE_CONNECT:
               case TYPE_PONG:
               case TYPE_REJECT:
               case TYPE_ACCEPT_DISCONNECT:
                  /* ignore this commands */
                  found = 0;
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       found = 1;
                       break;
                     }
                  }
                  /* only free connection if not connected */
                  if (found == 0) {
                     FreeConnection(Sender);
                  }
                  break;
            }
         }
      }
      /* wait a bit and try againg, till timeout runs out */
      if (Sender < 0) {
         WaitMs(100);
         Timeout--;
      }

   } while (Timeout > 0);

   return -1;
}
/*****************************************************************************/
/*  End         : ConnectTo                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : IsConnected                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Checks if a given connection still exists                  */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Handle        Handle of connection to check                */
/*                                                                           */
/*  Output Para : TRUE if connected, FALSE otherwise                         */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 08.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int IsConnected(int Handle)
{
   /* procedure data */

   /* procedure code */

   /* Check for correct Arguments */
   if ((Handle < 0) || (Handle >= 20) || (Connections[Handle].Valid == 0)) {
      CODING_ERROR("Error in IsConnected, Invalid Handle");
   }

   /* return state of connection */
   return Connections[Handle].Connected;
}
/*****************************************************************************/
/*  End         : IsConnected                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : Disconnect                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Closes a connection                                        */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Handle        Handle of connection to close                */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void Disconnect(int Handle)
{
   /* procedure data */

   /* procedure code */

   /* Check for correct Arguments */
   if ((Handle < 0) || (Handle >= 20) || (Connections[Handle].Valid == 0)) {
      CODING_ERROR("Error in Disconnect, Invalid Handle");
   }

   /* close the connection */
   if (Connections[Handle].Valid != 0) {
      SendHandshake(Handle, TYPE_DISCONNECT, 0, NULL);
      FreeConnection(Handle);
   }

   /* and release the handle */
   Connections[Handle].Handle = -1;
   Connections[Handle].Valid = 0;
   Connections[Handle].Connected = 0;
}
/*****************************************************************************/
/*  End         : Disconnect                                                 */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : AcceptConnection                                           */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Accepts any incomming connection request                   */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : NameOfConnector   Name of the host trying to connect       */
/*                MaxLength         Maximal reserved space for host name     */
/*                                  Name will be truncated if longer         */
/*                MyName            Name server will respond to broadcast    */
/*                                                                           */
/*  Output Para : Handle of newly creted connection or -1 if no connection   */
/*                occured                                                    */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                24.10.2009  IO  Modified for serversearch (response to     */
/*                                ping)                                      */
/*                                                                           */
/*****************************************************************************/
int  AcceptConnection(char *NameOfConnector, int MaxLength, char *MyName)
{
   /* procedure data */
   char          Buffer[500];
   TelegramType *ReceivedTelegram = (TelegramType *) Buffer;
   int           Length, ReceivedLength;
   int           Sender;
   int i;
   int found;
   int MyNameLength = 0;

   /* procedure code */
   /* repeat as long as datas are availlable */
   do {
      /* Read datas from network */
      ReadMessage(&Sender, ReceivedTelegram, 450, &ReceivedLength);

      /* check for a valid telegram */
      if ((Sender >= 0) && (ReceivedTelegram->Header.ID == TELEGRAM_ID)) {
         Length = ReceivedTelegram->Header.Length;
         if (    (((char *)ReceivedTelegram)[Length-2] == 0x55)
              && (((char *)ReceivedTelegram)[Length-3] == (char)0xaa)) {

            /* Telegram is valid, analyze further */
            switch (ReceivedTelegram->Header.Type) {

               case TYPE_PING:
                  /* reply ping */
                  if (MyName != NULL) {
                     MyNameLength = strlen(MyName)+1;
                     if (MyNameLength > 40) {
                        MyNameLength = 40;
                     }
                  }
                  /* Reply with own name to a ping */
                  SendHandshake(Sender, TYPE_PONG, MyNameLength, MyName);
                  found = 0;
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       found = 1;
                     }
                  }
                  /* only free connection if not connected */
                  if (found == 0) {
                     FreeConnection(Sender);
                  }
                  break;

               case TYPE_CONNECT:
                  /* ahh, we got a connection request */
                  found = 0;
                  /* check if not allready connected */
                  for (i = 0; i < 20; i++) {
                     if (   (Connections[i].Valid != 0)
                         && (Connections[i].Handle == Sender)) {
                       found = 1;
                       Connections[i].Connected = 1;
                       /*printf("Allready connected..."); */
                       // allready connected, what shall we do ??
                       break;
                     }
                  }
                  if (found == 0) {
                     /* find a free handle */
                     for (i = 0; i < 20; i++) {
                        if (Connections[i].Valid == 0) {
                           /* and create new handle entry for this connection */
                           Connections[i].Handle = Sender;
                           Connections[i].Valid = 1;
                           Connections[i].Connected = 1;
                           /* confirm connection to connector */
                           SendHandshake(Sender, TYPE_ACCEPT, 0, NULL);
                           /* printf("sent accept");  */
                           /* and return name of connector and handle of connection */
                           strncpy(NameOfConnector, ReceivedTelegram->Data.Raw, MaxLength);
                           return Sender;
                        }
                     }
                     // no free entry left, ignore
                     FreeConnection(Sender);
                  }
                  break;

               case TYPE_DISCONNECT:
                  /* got a disconnect, honor it */
                  SendHandshake(Sender, TYPE_ACCEPT_DISCONNECT, 0, NULL);
                  /* find corresponding connection */
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       /* and disconnect it */
                       Connections[i].Connected = 0;
                       Connections[i].Handle = -1;
                       break;
                     }
                  }
                  FreeConnection(Sender);
                  break;

               case TYPE_MESSAGE:
               case TYPE_PONG:
               case TYPE_REJECT:
               case TYPE_ACCEPT_DISCONNECT:
                  /* ignore this commands */
                  found = 0;
                  for (i = 0; i < 20; i++) {
                     if (Connections[i].Handle == Sender) {
                       found = 1;
                       break;
                     }
                  }
                  /* only free connection if not connected */
                  if (found == 0) {
                     FreeConnection(Sender);
                  }
                  break;
            }
         }
      }
      /* do as long as datas are availabe */
   } while (Sender >= 0);

   return -1;
}
/*****************************************************************************/
/*  End         : AcceptConnection                                           */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : SendHandshake                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sends a handshake telegram                                 */
/*                                                                           */
/*  Type        : Local                                                      */
/*                                                                           */
/*  Input Para  : Handle      Where to send the handshake                    */
/*                Command     Command to send                                */
/*                ExtraLength Length of additional data to send              */
/*                Extradata   Pointer to additional data to send             */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
static void SendHandshake(int Handle, short Command, int ExtraLength, char *Extradata)
{
   /* procedure data */
   char          Buffer[500];
   TelegramType *NewTelegram = (TelegramType *) Buffer;
   int           Length = sizeof(TelegramType) + ExtraLength + 4;

   /* procedure code */

   /* Check for correct Arguments */
   if (ExtraLength > 400) {
      CODING_ERROR("Error in SendHandshake, Telegram too long");
   }

   /* Setup the telegramheader */
   NewTelegram->Header.ID     = TELEGRAM_ID;
   NewTelegram->Header.Length = Length;
   NewTelegram->Header.Type   = Command;

   /* add the security marker */
   ((char *)NewTelegram)[Length-2] = 0x55;
   ((char *)NewTelegram)[Length-3] = 0xaa;

   /* check if we have to copy the aditional data */
   if ((ExtraLength > 0) && (Extradata != NULL)) {
      memcpy(NewTelegram->Data.Raw, Extradata, ExtraLength);
   }


   /* and send the telegram to its destination */
   if (Handle == H_BROADCAST) {
      SendBroadcastMessage(NewTelegram, Length);
   } else {
      SendMessageTo(Handle, NewTelegram, Length);
   }

}
/*****************************************************************************/
/*  End         : SendHandshake                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  End Module  : Communication                                              */
/*****************************************************************************/














