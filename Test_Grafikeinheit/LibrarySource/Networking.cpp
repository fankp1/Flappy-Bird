/* general control */

/*****************************************************************************/
/*  Module     : Networking                                     Version 1.0  */
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
/*               07.07.2002  IO Changed bufferlength in call to              */
/*                              GetIPAddress in function InitializeNetwork   */
/*                              Used to crash when users delivered Buffers   */
/*                              with length < 90                             */
/*               28.03.2004  IO Added some typecasts in SendMessageTo() and  */
/*                              ReadMessage() for cpp compatibility          */
/*                              Added cpp compatibility in header            */
/*                                                                           */
/*               28.04.2004  IO Added support for arbitrary Telegrammlengths */
/*                              (MaxTelegramLength)                          */
/*                                                                           */
/*               24.10.2009  IO Ported to QT, added support for Broadcast    */
/*                                                                           */
/*               01.01.2010  IO Added debugging support                      */
/*                                                                           */
/*  File       : Networking.c                                                */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
//#define FD_SETSIZE 32000

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <winsock2.h>


#include "networking.h"
#include "error.h"

#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>


/* module constant declaration */
#define DEFAULT_PORT    8357
#define MAXMSG  512
#define MAX_CONNECTIONS  20

/* module type declaration */
typedef struct ConnectionType {
   QHostAddress client;           /* Clientaddress of connection */
   unsigned int Port;             /* Client port of connection */
   int    Valid;                  /* state of address */
} ConnectionType;


/* module data declaration */
static QUdpSocket *MySendSocket = NULL;   /* The socket for sending   */
static QUdpSocket *MySocket = NULL;       /* The socket for receiving */
//static fd_set active_fd_set;  /* socketset to wait for events */

/* list of active connections */
static ConnectionType Connections[MAX_CONNECTIONS];

static unsigned int MaxTelegramLength = 0;
static unsigned int LocalPort  = DEFAULT_PORT;
static unsigned int RemotePort = DEFAULT_PORT;

FILE *DebugOut = NULL;

/* module procedure declaration */

/*****************************************************************************/
/*  Procedure   : GetIPAddress                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns IP Address of local host                           */
/*                                                                           */
/*  Type        : Local                                                      */
/*                                                                           */
/*  Input Para  : address        Buffer to place IP-Address                  */
/*                len            Maximum Size of Buffer                      */
/*                                                                           */
/*  Output Para : True if address could be determined, fals otherwise        */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 03.04.2002  IO  Created                                    */
/*                24.10.2009  IO Ported to QT, added support for getting     */
/*                               all local IP Adresses (Multiple network     */
/*                               adapters                                    */
/*                                                                           */
/*****************************************************************************/
static int GetIPAddress( char *address, int len )
{
   /* procedure data */

   /* procedure code */
   if (MySendSocket != NULL) {
      /* Fetch our first IP Adress */
      qstrncpy (address, MySendSocket->localAddress().toString().toLatin1(), len);
      // return the IP address
      return( TRUE );

   } else {
      if (len > 1) {
         address[0] = '?';
         address[1] = 0;
      }
      return( FALSE );  // Unable to get the IP address
   }
}

static char** GetAllIPAddresses( void )
{
   /* Prepare space for 20 IP Adresses               */
   /* Hopefully, we wont never have more adresses... */
   /* Warning, function is not reentrant!!!          */
#define MAX_NUMBER_OF_TABLE_ENTRIES 20

   static char *EntryTable[MAX_NUMBER_OF_TABLE_ENTRIES+1];
   static char Entries[MAX_NUMBER_OF_TABLE_ENTRIES][100];
   int i;

   /* Clear table */
   for (i=0; i < MAX_NUMBER_OF_TABLE_ENTRIES; i++) {
      EntryTable[i] = Entries[i];
   }
   EntryTable[MAX_NUMBER_OF_TABLE_ENTRIES] = NULL;

   i = 0;

   QString item;

   /* Walk trough all networ adapters */
   foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
   {
      if (interface.flags().testFlag(QNetworkInterface::IsRunning)) {

        /* Walk throug all adresses for this adapter */
        foreach (QNetworkAddressEntry entry, interface.addressEntries())
        {
          /* Place valid entries in table, stop if more then 20 entries are reached */
          if ((interface.hardwareAddress() != "00:00:00:00:00:00") && entry.ip().toString().contains(".")) {
            item = interface.humanReadableName() + " >" + entry.ip().toString() + "<";
            qstrncpy(EntryTable[i++], item.toLatin1().constData(), 98);
          }
          /* Table is full, stop looking... */
          if (i >= MAX_NUMBER_OF_TABLE_ENTRIES) {
             break;
          }
        }
      }
      /* Table is full, stop looking... */
      if (i >= MAX_NUMBER_OF_TABLE_ENTRIES) {
         break;
      }
   }
   EntryTable[i] = NULL;
   return EntryTable;
}
/*****************************************************************************/
/*  End         : GetIPAddress                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : CreateConnection                                           */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates a connection, in fact it only stores               */
/*                addressinformation in a table (no real connection)         */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : InetAddress    String with ip-address to connect (x.y.z.w) */
/*                                                                           */
/*  Output Para : Handle for this connection, -1 on error                    */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 03.04.2002  IO  Created                                    */
/*                24.10.2009  IO Ported to QT                                */
/*                                                                           */
/*****************************************************************************/
int CreateConnection( char *InetAddress)
{
   /* procedure data */
   int Index = -1;
   int i;

   unsigned int Port = RemotePort;

   QString FullAdress(InetAddress);

   /* Check if we got a port as well */
   if (FullAdress.count(':') > 0) {
      /* extract Port from Address */
      Port = FullAdress.section(':',1,1).toUInt();
      if (Port == 0) {
         Port = RemotePort;
      }
      FullAdress = FullAdress.section(':',0,0);
   }

   QHostAddress Address = QHostAddress(FullAdress);

   /* procedure code */

   /* check for valid address */
   if (Address.isNull()) {
      CODING_ERROR("Error in CreateConnection, Invalid Inetaddress");
      return -1;
   }

   /* check if conection allready exists */
   for (i = 0; i < MAX_CONNECTIONS; i++) {
      if (    (Address == Connections[i].client)
           && (Port    == Connections[i].Port)
           && (Connections[i].Valid != 0)) {
         Index = i;
      }
   }
   /* if not, search e free entry and create a new connection */
   if (Index < 0) {
      for (i = 0; i < MAX_CONNECTIONS; i++) {
         if (Connections[i].Valid == 0) {
            Connections[i].client = Address;
            Connections[i].Port = Port;
            Connections[i].Valid = 1;
            Index = i;
            break;
         }
      }
   }
   /* return handle for the connection (-1 on failure) */
   return Index;
}
/*****************************************************************************/
/*  End         : CreateConnection                                           */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : FreeConnection                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Removes a conection (just releases entry from table)       */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Index          Handle of connection to release             */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 03.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void FreeConnection( int Index )
{
   /* procedure data */

   /* procedure code */

   /* invalidate the connection */
   if ((Index >= 0) && (Index < MAX_CONNECTIONS)) {
      Connections[Index].Valid = 0;
   } else {
      CODING_ERROR("Error in FreeConnection, Index out of range");
   }
}
/*****************************************************************************/
/*  End         : FreeConnection                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetHostAddress                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sends a block of data to the recipient                     */
/*                (No secured transfer, no handshaking)                      */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Index          Handle of recipient to send to              */
/*                Message        Pointer to block of data to send            */
/*                Length         length of Datablock                         */
/*                                                                           */
/*  Output Para : 0 on success, -1 otherwise                                 */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 23.10.2009  IO  Created                                    */
/*                24.10.2009  IO Ported to QT                                */
/*                                                                           */
/*****************************************************************************/
int GetHostAddress(int Index, char *IPAddress, int MaxLength)
{
   /* procedure data */

   /* procedure code */

   /* check for correct arguments */
   if ((Index >= MAX_CONNECTIONS) || (Index < 0)) {
      CODING_ERROR("Error in GetHostAddress, Index out of range");
      return -1;
   }

   if ((IPAddress == NULL)||(MaxLength < 0)) {
      return -1;
   }

   /* Copy the IPAdress to the user data block */
   qstrncpy(IPAddress, Connections[Index].client.toString().toLatin1().constData(), MaxLength);
   return 0;
}
/*****************************************************************************/
/*  End         : GetHostAddress                                             */
/*****************************************************************************/


/*****************************************************************************/
/*  Procedure   : SendMessageTo                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sends a block of data to the recipient                     */
/*                (No secured transfer, no handshaking)                      */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Index          Handle of recipient to send to              */
/*                Message        Pointer to block of data to send            */
/*                Length         length of Datablock                         */
/*                                                                           */
/*  Output Para : 0 on success, -1 otherwise                                 */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 03.04.2002  IO  Created                                    */
/*                24.10.2009  IO Ported to QT                                */
/*                                                                           */
/*****************************************************************************/
int SendMessageTo(int Index, void *Message, unsigned int Length)
{
   /* procedure data */

   /* procedure code */
   //printf("Sending to %s \n", inet_ntoa( Connections[Index].client.sin_addr ));
   /* check for correct arguments */
   if ((Index >= MAX_CONNECTIONS) || (Index < 0)) {
      CODING_ERROR("Error in SendMessageTo, Index out of range");
      return -1;
   }

   if (Length > MaxTelegramLength) {
      CODING_ERROR("Error in SendMessageTo, Telegramm too long");
      return -1;
   }

   if (MySendSocket == NULL) {
      CODING_ERROR("Network not initialized");
      return -1;
   }

   /* send the datablock to the recipient */
   if (MySendSocket->writeDatagram ((const char*) Message, Length, Connections[Index].client, Connections[Index].Port) < 0) {
      perror ("sendto failed");
      printf("Index = %d", Index);
      return -1;
   }
   if (DebugOut != NULL) {
      fprintf(DebugOut, "NW: [Send to %s] [L=%d] <",  Connections[Index].client.toString().toLatin1().constData(), Length);
      for (unsigned int i = 0; (i < 16)&&(i < Length); i++) {
         fprintf(DebugOut, "%02X ", ((const unsigned char*) Message)[i]);
      }
      fprintf(DebugOut, ">\n");
   }
   return 0;
}
/*****************************************************************************/
/*  End         : SendMessageTo                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : SendBroadcastMessage                                       */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sends a block of data as broadcast to all attached systems */
/*                (No secured transfer, no handshaking)                      */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Message        Pointer to block of data to send            */
/*                Length         length of Datablock                         */
/*                                                                           */
/*  Output Para : 0 on success, -1 otherwise                                 */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 24.10.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int SendBroadcastMessage(void *Message, unsigned int Length)
{
   /* procedure data */

   /* procedure code */
   //printf("Sending to %s \n", inet_ntoa( Connections[Index].client.sin_addr ));
   /* check for correct arguments */

   if (Length > MaxTelegramLength) {
      CODING_ERROR("Error in SendMessageTo, Telegramm too long");
      return -1;
   }

   if (MySendSocket == NULL) {
      CODING_ERROR("Network not initialized");
      return -1;
   }

   /* send the datablock to the recipient */
   if (MySendSocket->writeDatagram ((const char*) Message, Length, QHostAddress(QHostAddress::Broadcast), RemotePort) < 0) {
//   if (MySendSocket->writeDatagram ((const char*) Message, Length, QHostAddress("147.87.174.245"), PORT) < 0) {
      perror ("sendbroadcast failed");
      return -1;
   }
   if (DebugOut != NULL) {
      fprintf(DebugOut, "NW: [Send to %s] [L=%d] <",  QHostAddress(QHostAddress::Broadcast).toString().toLatin1().constData(), Length);
      for (unsigned int i = 0; (i < 16)&&(i < Length); i++) {
         fprintf(DebugOut, "%02X ", ((const unsigned char*) Message)[i]);
      }
      fprintf(DebugOut, ">\n");
   }
   return 0;
}
/*****************************************************************************/
/*  End         : SendMessageTo                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : ReadMessage                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Reads a message from any sender, if availlable             */
/*                If a message is available, it is returned with valid       */
/*                handle. If no message is available handle is set to -1     */
/*                This is a nonblocking call, it allways returns immediately */
/*                (Creates automatically a connection if not allready exists)*/
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Index          Pointer where to write Handle of sender     */
/*                Message        Pointer to Buffer to store received MSG     */
/*                MaxLength      Maximum capacity of receiving buffer        */
/*                ReceivedLength Pointer to write size of received block     */
/*                                                                           */
/*  Output Para : 0 on success, -1 otherwise                                 */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 03.04.2002  IO  Created                                    */
/*                24.10.2009  IO Ported to QT                                */
/*                                                                           */
/*****************************************************************************/
int ReadMessage(int *Index, void *Message, int MaxLength, int *ReceivedLength)
{
   /* procedure data */
    int nbytes;
   int i;

   /* procedure code */
   *Index = -1;

   if (MySocket == NULL) {
       CODING_ERROR("Network not initialized");
       return -1;
    }

   /* check if messages are availlable */
   if (MySocket->pendingDatagramSize() >= 0)
   {
      QHostAddress Address;

      /* receive the message */
      /*printf("Received something Networking"); */
      nbytes = MySocket->readDatagram((char *)Message, MaxLength, &Address);
      if (nbytes < 0) {
         /* Read error. */
         perror ("read");
         return -1;
      }
      if (DebugOut != NULL) {
         fprintf(DebugOut, "NW: [received from %s] [L=%d] <",  Address.toString().toLatin1().constData(), nbytes);
         for (int i = 0; (i < 16)&&(i < nbytes); i++) {
            fprintf(DebugOut, "%02X ", ((const unsigned char*) Message)[i]);
         }
         fprintf(DebugOut, ">\n");
      }

      /* store length of received datablock */
      *ReceivedLength = nbytes;

      /* check if this connection allready exist */
      for (i = 0; i < MAX_CONNECTIONS; i++) {
         if (    (Address == Connections[i].client)
              && (Connections[i].Valid != 0)) {
            *Index = i;
         }
      }

      /* if not, create a new connection */
      if (*Index < 0) {
         for (i = 0; i < MAX_CONNECTIONS; i++) {
            if (Connections[i].Valid == 0) {
               Connections[i].client = Address;
               Connections[i].Port = RemotePort;
               Connections[i].Valid = 1;
               *Index = i;
               break;
            }
         }
      }
    }
    return 0;
}
/*****************************************************************************/
/*  End         : ReadMessage                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : InitializeNetwork                                          */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Initializes the networkmodule. Must be called before any   */
/*                other function of this module.                             */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : MaximumTelegramLength   Maximum allowed Telegrammlength    */
/*                MyIpAddress    Where to place IP Address of this computer  */
/*                               (in fact only the address of the first      */
/*                                Network connection, if multiple adapters   */
/*                                are availlable)                            */
/*                                                                           */
/*                Port            Port to look for incomming messages, also  */
/*                                used als default destination port for      */
/*                                outgoing messages                          */
/*                                                                           */
/*  Output Para : Returns pointer to array of pointer to stings of local IP  */
/*                adresses, end of array is marked by a NULL-Pointer         */
/*                                                                           */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 03.04.2002  IO  Created                                    */
/*                07.07.2002  IO  Changed bufferlength in call to            */
/*                                GetIPAddress from 90 to 16.                */
/*                                Used to crash when users delivered Buffers */
/*                                with length < 90                           */
/*                                Better solution would be to propagate      */
/*                                Bufferlength from user to this call        */
/*                24.10.2009  IO Ported to QT                                */
/*                                                                           */
/*****************************************************************************/
char ** InitializeNetwork(char *MyIpAddress, unsigned int MaximumTelegramLength, unsigned int NewLocalPort, unsigned int NewRemotePort)
{
   /* procedure data */
   int i;
   char **IpAdresses;
   unsigned int MaxTelLength = 512;

   /* procedure code */

   if (NewLocalPort > 0) {
      LocalPort = NewLocalPort;
   } else {
      LocalPort = DEFAULT_PORT;
   }
   if (NewRemotePort > 0) {
      RemotePort = NewRemotePort;
   } else {
      RemotePort = DEFAULT_PORT;
   }

   MySendSocket = new QUdpSocket();
   MySocket = new QUdpSocket();

   /* check if sockets could be created */
   if ((MySocket == NULL)||(MySendSocket == NULL)) {
      CODING_ERROR("Error in InitializeNetwork, Could not create socket");
      /* release allready allocated resources... */
      if (MySocket != NULL) {
         delete MySocket;
         MySocket = NULL;
      }
      if (MySendSocket != NULL) {
         delete MySendSocket;
         MySendSocket = NULL;
      }
      return NULL;
   }

   /* fetch our own IP Address */
   GetIPAddress( MyIpAddress, 16);   /* 07.07.2002 IO Changed length from 90 to 16, crashed */
                                     /* when users delivered Buffer < 90 in length */

   /* Create the sockets */
   IpAdresses = GetAllIPAddresses();

   //MySendSocket->bind(PORT);
   MySocket->bind(LocalPort);

   MaxTelegramLength = MaximumTelegramLength & TELEGRAMLENGTH_MASK;

   /* Check if we can grant whished length */
#if 1
   if ((MaximumTelegramLength & IGNORE_MAXIMAL_RECOMENDED_TELEGRAMLENGTH_MASK) != IGNORE_MAXIMAL_RECOMENDED_TELEGRAMLENGTH) {
      if (MaxTelegramLength > MaxTelLength) {
         CODING_ERROR("Error in InitializeNetwork, maximum telegramm lenghth exceeds cpability of network");
         return NULL;
      }
   }
#endif


   /* initialize the conections array */
   for ( i = 0; i < MAX_CONNECTIONS; i++) {
     Connections[i].Valid = 0;
   }
   return IpAdresses;
}
/*****************************************************************************/
/*  End         : InitializeNetwork                                          */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : CleanupNetwork                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Shuts down the networkmodule. Releases all aquired         */
/*                resources. Must be called before program ends              */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 03.04.2002  IO  Created                                    */
/*                24.10.2009  IO Ported to QT                                */
/*                                                                           */
/*****************************************************************************/
void CleanupNetwork(void)
{
  /* procedure data */
  int i;

   /* procedure code */

   /* initilaize the connections array */
   for ( i = 0; i < MAX_CONNECTIONS; i++) {
      Connections[i].Valid = 0;
   }

   /* close the sockets */
   if (MySocket != NULL) {
      delete MySocket;
      MySocket = NULL;
   }
   if (MySendSocket != NULL) {
      delete MySendSocket;
      MySendSocket = NULL;
   }

}
/*****************************************************************************/
/*  End         : CleanupNetwork                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  End Module  : Networking                                                 */
/*****************************************************************************/



