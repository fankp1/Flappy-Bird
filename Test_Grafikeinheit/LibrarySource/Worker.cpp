/*****************************************************************************/
/*  Class      : Worker                                         Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : This is our Workerthread, in which the clients code run     */
/*               (The Client code is in gfxmain())                           */
/*               Is in fact a singleton, e. g. there should be only one      */
/*               instance.                                                   */
/*               Access to the only instance via GetTheWorker()              */
/*                                                                           */
/*  Methodes   : Worker()                                                    */
/*              ~Worker()                                                    */
/*               GetTheWorker()                                              */
/*               run()                                                       */
/*                                                                           */
/*              A lot of interfacefunctions between client and Qt            */
/*              Converts client calls to signals, a manages thus             */
/*              interthread communication                                    */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 02.09.2009  IO Created                                      */
/*                                                                           */
/*  File       : Worker.cpp                                                  */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
#include<iostream>
#include <QCoreApplication>
#include <QStringList>
#include <QThreadPool>
//#include <QFileDialog>
#include <QPolygon>
#include "Worker.h"

/* Class constant declaration  */

/* Class Type declaration      */

/* Class data declaration      */
extern volatile int vol;
extern "C" int gfxmain(int argc, char *argv[], const char *ApplicationPath);

/* Our only instance */
Worker *Worker::TheWorker = NULL;

/* Class procedure declaration */


/*****************************************************************************/
/*  Class       : MyRunableObject                                            */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Helperclass for Multithreading, objects can be             */
/*                parametrized with a function to call                       */
/*                                                                           */
/*  Methodes   : MyRunableObject()                                           */
/*              ~MyRunableObject()                                           */
/*               run()                                                       */
/*                                                                           */
/*              A lot of interfacefunctions between client and Qt            */
/*              Converts client calls to signals, a manages thus             */
/*              interthread communication                                    */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 02.04.2012  IO Created                                      */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
class MyRunableObject : public QRunnable {

   void *Parameter;
   void (*Function)(void *);
   public:
             /* Creates a new runable object with Function to call and its parameter */
             MyRunableObject(void *Para, void (*_Function)(void *)) : Parameter(Para), Function(_Function) {};
   virtual  ~MyRunableObject() {};
   virtual void run ();
};

void MyRunableObject::run()
{
   /* Call our function with its parameter */
   if (Function != NULL) {
      Function(Parameter);
   }
}
/*****************************************************************************/
/*  End Class   : MyRunableObject                                            */
/*****************************************************************************/




/*****************************************************************************/
/*  Method      : Worker                                                     */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Constructor for the Worker. Initializes the worker         */
/*                                                                           */
/*  Type        : Constructor                                                */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
Worker::Worker() : MyMutex(QMutex::Recursive)
{
   /* procedure data */

   /* procedure code */

   stopped = false;
   TheWorker = this;
}
/*****************************************************************************/
/*  End  Method : Worker                                                     */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetTheWorker                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the only instance of our Worker. Creates a Worker  */
/*                if none exist                                              */
/*                                                                           */
/*  Type        : Constructor                                                */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : Pointer to the worker instance                             */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
Worker* Worker::GetTheWorker()
{
   /* procedure data */

   /* procedure code */

   /* Create our only instance if not allready exist */
   if (TheWorker == NULL) {
      TheWorker = new Worker;
   }
   return TheWorker;
}
/*****************************************************************************/
/*  End  Method : GetTheWorker                                               */
/*****************************************************************************/


/*****************************************************************************/
/*  Method      : run                                                        */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Runs the client code by calling gfxmain()                  */
/*                if none exist                                              */
/*                                                                           */
/*  Type        : Protected                                                  */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void Worker::run()
{
    /* procedure data */

   /* procedure code */

   /* Create standard arguments for gfxmain, using */
   /* informtions from Qt of original arguments    */

   /* Get original arguments */
    QStringList CommandLine = QCoreApplication::arguments();

   /* And convert to c-main form */
    int argc = CommandLine.size();
    char **Argv = new char *[argc];
    for (int i = 0; i < argc; ++i) {
       QByteArray text = CommandLine.at(i).toLocal8Bit();
       Argv[i] = new char[text.size() + 1];
       strcpy(Argv[i], text.constData());
    }

    /* Get path to executable */
    QString BasePath = QCoreApplication::applicationDirPath ();

    /* Call usercode, passin standardargumets and applicationpath */
    gfxmain(argc, Argv, BasePath.toLatin1().constData());

    /* Shut down our application if gfxmain() returns */
    stopped = false;
    for (int i = 0; i < argc; ++i) {
       delete[] Argv[i];
    }
    delete[] Argv;
    emit Sclose();
}
/*****************************************************************************/
/*  End  Method : run                                                        */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : Interface functions                                        */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Just convert calls from client code int signals            */
/*                and thus managing interthread communication.               */
/*                                                                           */
/*  Type        : Protected                                                  */
/*                                                                           */
/*  Input Para  : Variable                                                   */
/*                                                                           */
/*  Output Para : Variable                                                   */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void Worker::stop()
{
    stopped = true;
}

void Worker::SetAutoUpdate(int Mode)
{
   emit SSetAutoUpdate(Mode != 0);
}

void Worker::SetDrawMode(int Mode)
{
   emit SSetDrawMode(Mode);
}

void Worker::setfillstyle(int FillType, unsigned long RGBColor)
{
   emit Ssetfillstyle(FillType, RGBColor);
}

void Worker::SetBackgroundColor(unsigned long RGBColor)
{
   emit SSetBackgroundColor(RGBColor);
}

void Worker::bar(int x , int y, int w, int h)
{
   emit Sbar(x, y, w, h);
}

void Worker::setcolor(unsigned long RGBColor)
{
   emit Ssetcolor(RGBColor);
}

void Worker::setlinestyle(int LineStyle, int dummy, int Width)
{
   emit Ssetlinestyle(Width);

   (void)LineStyle;  /*Silence unused param warning */
   (void)dummy;      /*Silence unused param warning */
}

#if 0
// unused
void Worker::rectangle(int x, int y, int w, int h)
{
    (void)x;  /*Silence unused param warning */
    (void)y;  /*Silence unused param warning */
    (void)w;  /*Silence unused param warning */
    (void)h;  /*Silence unused param warning */
}
#endif


void Worker::ellipse(int x, int y, int w, int h)
{
   emit SDrawEllipse(x, y, w, h);
}
;
void Worker::line(int x1 , int y1, int x2, int y2)
{
   emit Sline(x1 , y1, x2, y2);
}

void Worker::polygon(int *Edges, int NumberOfEdges)
{
   QPolygon *polygon = new QPolygon();
   polygon->setPoints(NumberOfEdges, Edges);
   emit Spolygon(polygon);
   // Polygon will be deletet by recipient */
}

void Worker::polyline(int *Edges, int NumberOfEdges)
{
   QPolygon *polygon = new QPolygon();
   polygon->setPoints(NumberOfEdges, Edges);
   emit Spolyline(polygon);
   // Polygon will be deletet by recipient */
}


void Worker::putpixel(int x1 , int y1)
{
   emit Spixel(x1, y1);
}

unsigned long Worker::GetPixel (int x, int y)
{
   unsigned long RGBColor;

   emit SGetPixel(&RGBColor, x, y);
   return RGBColor;
}

void Worker::outtextxy(int x, int y,  const char *Text)
{
   emit SDrawText(x, y, QString(Text));
}

/*****************************************************************************/
/*  Procedure   : SelectFont                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Selects a font for future Textdrawings                     */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : FontName     Name of the Font (eg. "Times")                */
/*                             May be NULL if only size ore style of current */
/*                             are to be changed                             */
/*                Points       Fontsize                                      */
/*                Style        May be any Combination of the Enum Fontstyle  */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 11.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void Worker::SelectFont(const char *FontName, int Points, int Style)
{
   /* procedure data */

   /* procedure code */

   /* Count Commands in interprocessqueue */
   emit SIncrementUnhandledCommands();

   /* Set the color for the text to be drawn with */
   emit SSelectFont(FontName, Points, Style);

}
/*****************************************************************************/
/*  End         : SelectFont                                                 */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetTextDimensions                                          */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Calculates the requred space for the given Text            */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Text           Text to find required space for             */
/*                                                                           */
/*  Output Para : Textdimensons (Length, Height and underlength)             */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 11.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
TextDimensionType Worker::GetTextDimensions (const char *Text)
{
   /* procedure data */
   TextDimensionType TextInfo;

   /* procedure code */

   /* Set the color for the text to be drawn with */
   emit SGetTextDimensions(&TextInfo, Text);

   return TextInfo;

}
/*****************************************************************************/
/*  End         : GetTextDimensions                                          */
/*****************************************************************************/

void Worker::initwindow(int Width, int Height)
{
   emit SSetWindowSize(Width, Height);
   emit SShow();
}
;


//void settextstyle(TRIPLEX_FONT, HORIZ_DIR, 0);
//void setusercharsize(1, 3, 1, 3);
void Worker::closegraph( )
{
   emit SHide();
}

int  Worker::getch()
{
   int Code = 0;
   emit SGetKeyEvent(&Code);
   return Code;
}

int Worker::kbhit()
{
   int Code = 0;
   emit SGetBufferedKeyPressEvents(&Code);
   return Code;
}

int Worker::kbEvent()
{
   int Code = 0;
   emit SGetBufferedKeyEvents(&Code);
   return Code;
}

void Worker::GetMouseState (struct MouseInfo *Info)
{
   emit SGetMouseState(Info);
}

void Worker::GetMouseEvent (struct MouseInfo *Info)
{
   emit SGetMouseEvent(Info);
}

void Worker::delay(int Time)
{
   msleep(Time);
}

void Worker::cleardevice()
{
   emit Scleardevice();
}

int Worker::CreateImage(int Width, int Height)
{
   int Id = 0;
   emit SCreateImage(&Id, Width, Height);
   return Id;
}

int  Worker::CreateImage(int SrcID, int x, int y, int Width, int Height)
{
   int Id = 0;
   emit SCreateImage(&Id, SrcID, x, y, Width, Height);
   return Id;
}


int Worker::SetEditedImage(int ImageId)
{
   /* Count Commands in interprocessqueue */
   emit SIncrementUnhandledCommands();

   emit SSetEditedImage(ImageId);
   return 0;
}

void Worker::GetImageSize(int ImageId, int *Width, int *Height)
{
   emit SGetImageSize(ImageId, Width, Height);
}

ColorType *Worker::GetPixelDataPointer(int ImageId, int *Length)
{
   void *Ptr = 0;
   emit SGetPixelDataPointer(ImageId, &Ptr, Length);
   return (ColorType *)Ptr;
}


int Worker::DrawImage(int ImageId, int x, int y)
{
   emit SDrawImage(ImageId, x, y);
   return 0;
}

void Worker::CopyToImage(int x, int y, int Width, int Height, int ImageId)
{
   emit SCopyToImage(x, y, Width, Height, ImageId);
}

void Worker::AddAlphaMask(int ImageId, int Mode, unsigned long Color)
{
   emit SAddAlphaMask(ImageId, Mode, Color);
}

int Worker::LoadImage(const char *FileName)
{
   int Id = 0;
   emit SLoadImage(&Id, FileName);
   return Id;
}

void Worker::SaveImage(int Id, const char *FileName, const char *Format, int Quality)
{
   emit SSaveImage(Id, FileName, Format, Quality);
}

void Worker::Rotate(float Angle)
{
   emit SRotate(Angle);
}

void Worker::Translate(float dx, float dy)
{
   emit STranslate(dx, dy);
}

void Worker::Scale(float Scalingx, float Scalingy)
{
   emit SScale(Scalingx, Scalingy);
}

void Worker::ResetTransformations(void)
{
   emit SResetTransformations();
}

int Worker::DestroyImage(int ImageId)
{
   emit SDestroyImage(ImageId);
   return 0;
}

void Worker::SetQtOptions(enum QTOptions Option, long int Value)
{
   emit(SSetQtOptions(Option, Value));
}


void Worker::PlaySoundContinuous(const char *FileName)
{
   emit SPlaySoundContinuous(QString(FileName));
}

void Worker::PlaySoundContinuous(void)
{
   emit SStartPlaySoundContinuous();
}

void Worker::StopContinuousSound(void)
{
   emit SStopContinuousSound();
}

void Worker::PlaySound(const char *FileName)
{
   emit SPlaySound(FileName);
}

void Worker::StartTimer(int IntervalTime, void *Parameter, void (*Handler)(void *))
{
   emit SStartTimer(IntervalTime, Parameter, (void *)Handler);
}

void Worker::StopTimer(void)
{
   emit SStopTimer();
}
/*****************************************************************************/
/*  End  Method : Interfacefunctions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  Method      : CreateThread                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates and starts a new Thread (Uses Threadpool from Qt)  */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : Parameter Parameter for Threadfunction                     */
/*                Function  Function to be called as new Thread with         */
/*                          Parameter parameter. Thread will end if Function */
/*                          returns                                          */
/*                                                                           */
/*  Output Para : True if thread was created and started sucessfully         */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 05.4.2012  IO  Created                                     */
/*                                                                           */
/*****************************************************************************/
bool Worker::CreateThread(void *Parameter, void (*Function)(void *))
{
   /* Method data declaration      */

   /* Method code declaration      */

   /* Only create Thread if got valid functionpointer */
   if (Function != NULL) {

      /* Create a new runable object for this function and send to threadpool */
      /* for execution                                                        */
      MyRunableObject *runnable = new MyRunableObject(Parameter, Function);
      return QThreadPool::globalInstance()->tryStart (runnable);
   } else {
      return false;
   }
}
/*****************************************************************************/
/*  End  Method : CreateThread                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : OpenFileDialog                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Opens a Filedialog and returns selected filename           */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : Mode      Defines if load or save dialog                   */
/*                Title     Title of dialog                                  */
/*                Directory Starting directory, may be empty string          */
/*                Filter    Filefilter in the form                           */
/*                          "My files (*.mf *.myf);;All Files (*.*)"         */
/*                          Set to empty string of not used                  */
/*                SelectedFilter Selected Filefilter                         */
/*                          Set to empty string of not used                  */
/*                Options   Optional behavior of Dialog, use FDO_USE_DEFAULT */
/*                FilenameBuffer  Buffer to store filename with path         */
/*                MaxNameLength   Length of buffer, this is the maximum      */
/*                                amount of characters placed in Buffer      */
/*                                Filename will be truncated if too long     */
/*                                                                           */
/*  Output Para : True if thread was created and started sucessfully         */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 05.4.2012  IO  Created                                     */
/*                                                                           */
/*****************************************************************************/
bool Worker::OpenFileDialog(int Mode, const char *Title, const char *Directory, const char *Filter, const char *PreferredFilter, int Options, char *FilenameBuffer, int MaxNameLength, char *SelectedFilter, int MaxFilterLength)
{
#if 1
	// Must run in GUI Main Thread
	/* Method data declaration      */
	bool RCode;

	/* Method code declaration      */

	emit SOpenFileDialog(&RCode, Mode, Title, Directory, Filter, PreferredFilter, Options, FilenameBuffer, MaxNameLength, SelectedFilter, MaxFilterLength);

	return RCode;
#else


   /* Method data declaration      */
    QString TheFileName;
    QFileDialog::Options MyOptions(Options);
    QString QSSelectedFilter(SelectedFilter);

   /* Method code declaration      */


   /* Open selected kind of dialog */
   if (Mode == FDM_FILE_SAVE) {
      if (MyOptions == FDO_USE_DEFAULT) {
         MyOptions = 0;
      }
      TheFileName = QFileDialog::getSaveFileName(NULL, QString(Title), QString(Directory), QString(Filter), &QSSelectedFilter, MyOptions);
   } else if (Mode == FDM_DIRECTORY) {
      if (MyOptions == FDO_USE_DEFAULT) {
         MyOptions = QFileDialog::ShowDirsOnly;
      }
      TheFileName = QFileDialog::getExistingDirectory(NULL, Title, Directory, MyOptions);
   } else {
      if (MyOptions == FDO_USE_DEFAULT) {
         MyOptions = 0;
      }
      TheFileName = QFileDialog::getOpenFileName(NULL, QString(Title), QString(Directory), QString(Filter), &QSSelectedFilter, MyOptions);
   }


   /* Copy resulting path to receiver array */
   strncpy(FilenameBuffer, TheFileName.toLatin1().constData(), MaxNameLength);

   if (SelectedFilter != NULL) {
      /* Copy selected filter receiver array */
      strncpy(SelectedFilter, QSSelectedFilter.toLatin1().constData(), MaxFilterLength);
   }

   /* return false if no file was selected */
   if (TheFileName.isNull()) {
      return false;
   } else {
      return true;
   }
#endif
}
/*****************************************************************************/
/*  End  Method : OpenFileDialog                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  End Class   : Worker                                                     */
/*****************************************************************************/

