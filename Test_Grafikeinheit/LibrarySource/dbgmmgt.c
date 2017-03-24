/* general control */

/*****************************************************************************/
/*  Module     : DebugMemorymanagement                          Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : This module capsulates the systemmemorymanagement and       */
/*               catches errors in the memorymanagement of the user          */
/*               (Unallocated blocks, Memory leaks, overwritten blocks,      */
/*                wrongly freed blocks and more)                             */
/*                                                                           */
/*      ***                                                                  */
/*      ***                                                                  */
/*      ***      ATTENTION: Any modul wanting to use this module must        */
/*      ***                 include the header dbgmmgt.h AFTER all system    */
/*       *                  headers, otherwise it will not work !!!!!        */
/*                                                                           */
/*      ***                                                                  */
/*      ***                                                                  */
/*                                                                           */
/*  Procedures : MyMalloc                 Allocates a Block of Memory        */
/*               MyFree                   Frees a Block of Memory            */
/*               PrintMemoryStatistics    Gives infos about the current      */
/*                                        state of the memorymanagement      */
/*                                                                           */
/*               Attention, the header dbgmmgt will reroute free and         */
/*               malloc to these functions, do not call them directly        */
/*               use malloc and free as usual and include dbgmmgt.h          */
/*               in your project                                             */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 04.02.1997  IO  Created                                     */
/*               28.03.2004  IO Modified for usage in C                      */
/*                                                                           */
/*  File       : dbgmmgt.c                                                   */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
#include <stdio.h>
#include <stdlib.h>


#include "dbgmmgt.h"
/* Undo defines from our own headerfile, we want */
/* to use the real malloc and free functions     */
#undef malloc
#undef free

#ifdef USE_DEBUG_MEMORYMANAGEMENT


#define coreleft() 1000   /* some function returning amount of free memory left */

/* module constant declaration */
#define IN_USE 0x3355ACACL
#define NOT_IN_USE 0x0L

/* structure to keep debuginfo for each requested block */
typedef struct BlockInfo {
    unsigned long     Size;
    unsigned long    *EndOfBlock;
    void             *Where;
    char             *AllocatingModule;
    int               AllocatingLineNumber;
    struct BlockInfo *Next;
    struct BlockInfo *Previous;
    unsigned long     Flag;
} BlockInfo;


/* module data declaration */
static void *BadDeleter     = 0;
static long  BlocksInUse    = 0;
static long  MaxBlocksInUse = 0;
static long  MemoryUsed     = 0;
static long  MaxMemoryUsed  = 0;
static long  LastCore = 0xFFFFFFL;
static BlockInfo *UsedList = NULL;

/* module procedure declaration */
static void PrintBlockInfo(BlockInfo *Block);
static void PrintOwnerOfUsedBlocks (void);


/*****************************************************************************/
/*  Procedure   : Some helpfull statistics                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the required statistic infos                       */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : The required values                                        */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 28.03.2004  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/

long GetBlocksInUse(void) {
   return BlocksInUse;
}

long GetMaxBlocksInUse(void) {
   return MaxBlocksInUse;
}

long GetUsedMemory (void) {
	 return MemoryUsed;
}

long GetMaxUsedMemory (void) {
	 return MaxMemoryUsed;
}

long GetLowestFreeMem (void) {
   return LastCore;
}
/*****************************************************************************/
/*  End         : Some helpfull statistics                                   */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : PrintBlockInfo                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Prints the debuginfos for a given memoryblock              */
/*                                                                           */
/*  Type        : Local                                                      */
/*                                                                           */
/*  Input Para  : BlockInfo  Pointer to block to print infos from            */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 28.03.2004  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
static void PrintBlockInfo(BlockInfo *Block)
{
   /* procedure data */

   /* Initialize with start of userdata */
   char *Contents =(((char *)Block)+sizeof(BlockInfo));
   int i, len = Block->Size;

   /* procedure code */

   /* Limit the amount of data printed */
   if (len > 20) {
      len = 20;
   }

   /* print the Blockinfos */
   printf("Blocksize: %d Bytes\n",  Block->Size );
   printf("Contents:");

   /* Print the contents of the block in hex */
   for (i = 0; i < len; i++) {
      printf(" %2x", Contents[i]);
   }

   /* Print the contents of the block in ascii, change controllcharacters to dots */
   printf("\n         ");
   for (i = 0; i < len; i++) {
      if ((Contents[i] >= ' ') && (Contents[i] < 127)) {
         putchar(Contents[i]);
      } else {
         putchar('.');
      }
   }
   /* Print where this block was allocated */
	printf("\nAllocated in file %s at line %d (Adress %p)\n", Block->AllocatingModule, Block->AllocatingLineNumber, Block->Where);
}
/*****************************************************************************/
/*  End         : PrintBlockInfo                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : PrintMemoryStatistics                                      */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Prints the debuginfos for the memorymanagement             */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : No9ne                                                      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 28.03.2004  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void PrintMemoryStatistics(void)
{
   /* procedure data */
   BlockInfo *Ptr;

   /* procedure code */

   /* print the statistic infos */
   printf("Memorystatistics:\n\n");
   printf("Still used Blocks: %ld  (Maximal used: %ld)\n", GetBlocksInUse(), GetMaxBlocksInUse());
   printf("Still used Memory: %ld  (Maximal used: %ld)\n", GetUsedMemory(), GetMaxUsedMemory());
   printf("Minimal amount of free memory was %ld\n\n", GetLowestFreeMem());

   /* and infos of all still allocated blocks */
   if (UsedList != NULL) {
      printf("List of blocks still in use:\n\n");
      for (Ptr = UsedList; Ptr != NULL; Ptr = Ptr->Next) {
         PrintBlockInfo(Ptr);
         printf("----------------\n");
      }
   }
}
/*****************************************************************************/
/*  End         : PrintMemoryStatistics                                      */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : MyMalloc                                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Allocates a block of memory, adding it to the list of      */
/*                allocated blocks, adds some debuginfos and returns a       */
/*                pointer to the user accessibla part of the block           */
/*                Also updates statistic informations                        */
/*                                                                           */
/*                Normally this function is not called directly, you should  */
/*                use malloc in your code, and include dbgmmgt.h in your     */
/*                code after the standardincludes                            */
/*                                                                           */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Size       Requested size of memoryblock in bytes          */
/*                Name       Filename of caller, will be inserted by macro   */
/*                           in dbgmmgt.h                                    */
/*                LineNumber Linenumber of invocation of this reoutine will  */
/*                           be inserted by macro in dbgmmgt.h               */
/*                                                                           */
/*                                                                           */
/*  Output Para : Pointer to memouryblock for user                           */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                28.03.2004  IO  Adepted for c usage                        */
/*                                                                           */
/*****************************************************************************/
void *MyMalloc(size_t Size, char *Name, int LineNumber)
{
   /* procedure data */

   /* procedure code */

   /* allocate the required memory and additional space for management */
   void *Mem = malloc(Size + sizeof(BlockInfo)+ sizeof(unsigned long));

   /* check if we got the memory */
   if (Mem != NULL) {

      /* Update statistics */
      BlocksInUse++;
      if (MaxBlocksInUse < BlocksInUse) {
         MaxBlocksInUse = BlocksInUse;
      }
      MemoryUsed += Size + sizeof(BlockInfo);
      if (MemoryUsed > MaxMemoryUsed) {
         MaxMemoryUsed = MemoryUsed;
      }
	   if (coreleft() < LastCore) {
         LastCore = coreleft();
      }

      /* Put infos into the blockheader */
      ((BlockInfo *)Mem)->Flag  = IN_USE;
      ((BlockInfo *)Mem)->AllocatingModule = Name;
      ((BlockInfo *)Mem)->AllocatingLineNumber = LineNumber;
      ((BlockInfo *)Mem)->Size  = Size;
      ((BlockInfo *)Mem)->Where = *(((void **)(&Size))-1); /* Get address of caller */
      ((BlockInfo *)Mem)->Next  = UsedList;
      ((BlockInfo *)Mem)->Previous = NULL;

      /* Add guardpattern to the end of the block */
      ((BlockInfo *)Mem)->EndOfBlock = (unsigned long *)(((char *)Mem) + Size + sizeof(BlockInfo));
      *(((BlockInfo *)Mem)->EndOfBlock) = ~IN_USE;

      /* and put in list of allocated blocks */
      if (UsedList != NULL) {
         UsedList->Previous = (BlockInfo *)Mem;
      }
      UsedList = (BlockInfo *)Mem;

      /* return a pointer to the ueser acessibla part of the block */
      return (((char *)Mem)+sizeof(BlockInfo));
   } else {

      /* We did not get the memory */
      return NULL;
   }
}
/*****************************************************************************/
/*  End         : MyMalloc                                                   */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : MyFree                                                     */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Frees a block of memory, removing it from the list of      */
/*                allocated blocks, checks some debuginfos and frees the     */
/*                block. Prints an errormessage and stopps if something is   */
/*                wrong                                                      */
/*                Also updates statistic informations                        */
/*                                                                           */
/*                Normally this function is not called directly, you should  */
/*                use free in your code, and include dbgmmgt.h in your       */
/*                code after the standardincludes                            */
/*                                                                           */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Mem        Pointer to Block to be released                 */
/*                Name       Filename of caller, will be inserted by macro   */
/*                           in dbgmmgt.h                                    */
/*                LineNumber Linenumber of invocation of this reoutine will  */
/*                           be inserted by macro in dbgmmgt.h               */
/*                                                                           */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 04.04.2002  IO  Created                                    */
/*                28.03.2004  IO  Adepted for c usage                        */
/*                                                                           */
/*****************************************************************************/
void MyFree(void *Mem, char *Name, int LineNumber) {
   /* procedure data */

   /* procedure code */

   /* check for valid pointer */
   if (Mem != NULL) {

      /* move to back to blockheader */
      Mem = ((char *)Mem)-sizeof(BlockInfo);

      /* Check if Block is valid */
      if (((BlockInfo *)Mem)->Flag != IN_USE) {
	      /* get address of bad guy */
	      BadDeleter = *(((void **)(&Mem))-1); /* Get address of caller */
	      printf("\n\aPANIC: Unallocated Block deleted\n");
	      printf("Deletet in file %s at line %d (Adress %p)\n", Name, LineNumber, BadDeleter);
         PrintBlockInfo((BlockInfo *)Mem);
         system("pause");
         abort();
      }
      /* check if end of block was overwritten */
      if (*(((BlockInfo *)Mem)->EndOfBlock) != ~IN_USE) {
	      /* get address of bad guy */
	      BadDeleter = *(((void **)(&Mem))-1); /* Get address of caller */
	      printf("\n\aPANIC: Block with overwritten end deleted\n");
	      printf("Deletet in file %s at line %d (Adress %p)\n", Name, LineNumber, BadDeleter);
         PrintBlockInfo((BlockInfo *)Mem);
         system("pause");
         abort();
      }
      /* update statistics */
      BlocksInUse--;
      MemoryUsed -= ((BlockInfo *)Mem)->Size + sizeof(BlockInfo);

      /* destroy flag, so we will detect multiple free()s of same block */
      ((BlockInfo *)Mem)->Flag = NOT_IN_USE;

      /* and remove from list of used blocks */
      if (((BlockInfo *)Mem)->Next != NULL) {
         ((BlockInfo *)Mem)->Next->Previous = ((BlockInfo *)Mem)->Previous;
      }
      if (((BlockInfo *)Mem)->Previous != NULL) {
         ((BlockInfo *)Mem)->Previous->Next = ((BlockInfo *)Mem)->Next;
      } else {
         UsedList = ((BlockInfo *)Mem)->Next;
      }

      /* and finally free the block if everything was ok */
      free(Mem);
   }
}
/*****************************************************************************/
/*  End         : MyFree                                                     */
/*****************************************************************************/
#endif  /* USE_DEBUG_MEMORYMANAGEMENT */

/*****************************************************************************/
/*  End Module  : DebugMemorymanagement                                      */
/*****************************************************************************/



