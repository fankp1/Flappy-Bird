/*****************************************************************************/
/*  Class      : MainGui                                        Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : This is our Mainwindow                                      */
/*                                                                           */
/*  Methodes   : MainGui()                                                   */
/*              ~MainGui()                                                   */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 15.12.2009  IO Created                                      */
/*               02.03.2015  IO Renamed drewmodes due windowsconflicts       */
/*                              (DM_OVER allready used by windows header)    */
/*                                                                           */
/*  File       : gui_main.cpp                                                */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
#include "gui_main.h"
#include <QtGui>
#include <QDir>
#include <QFileDialog>
#include "window.h"
#include <iostream>

/* Class constant declaration  */

/* Class Type declaration      */
#undef DrawText
#undef LoadImage
#undef PlaySound

/* Class data declaration      */

/* Class procedure declaration */

/*****************************************************************************/
/*  Method      : MainGui                                                    */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Constructor for a MainGui. Initializes the whole           */
/*                application                                                */
/*                                                                           */
/*  Type        : Constructor                                                */
/*                                                                           */
/*  Input Para  : parent Pointer to parent widget                            */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/

//volatile int vol;

MainGui::MainGui(QWidget *parent)
    : QWidget(parent)
{
   /* Method data declaration      */

   /* Method code declaration      */

   TimerTickHandler = NULL;
   TimerTickParameter = NULL;
   MyTimer = new QTimer(this);

	/* Connect all drawcommand signals to their slots             */
   /* Attention, commands with returnvalues are direct connected */
   /* using Qt::DirectConnection. Must be carefully using this   */
   /* across threads !!! Locking may be nescessary.              */

   /* BlockingQueuedConnection is the solution if returnvalues   */
   /* are needed and all the emits before this call should have  */
   /* been completed. (E.g before GetPixel, all drawcommands     */
   /* should be competed)                                        */

   /* (Direct connection is a problem, if the result of this       */
   /* operation  is based on  other calls, since direct connection */
   /* may overtake commads in Comandqueue)                         */

	QObject::connect(&MyThread, SIGNAL(SSetBackgroundColor(unsigned long)),
	        this, SLOT(SetBackgroundColor(unsigned long)));

	QObject::connect(&MyThread, SIGNAL(Ssetfillstyle(int, unsigned long)),
           this, SLOT(setfillstyle(int, unsigned long)));

   QObject::connect(&MyThread, SIGNAL(SSetDrawMode(int)),
           this, SLOT(SetDrawMode(int)));

   QObject::connect(&MyThread, SIGNAL(SSetQtOptions(int, long)),
           this, SLOT(SetQtOptions(int, long)));

   QObject::connect(&MyThread, SIGNAL(Sbar(int, int, int, int)),
           this, SLOT(bar(int, int, int, int)));

   QObject::connect(&MyThread, SIGNAL(SSetWindowSize(int, int)),
           this, SLOT(SetWindowSize(int, int)));

   QObject::connect(&MyThread, SIGNAL(SShow()),
           this, SLOT(show()));

   QObject::connect(&MyThread, SIGNAL(SHide()),
           this, SLOT(hide()));

   QObject::connect(&MyThread, SIGNAL(SGetKeyEvent(int *)),
           this, SLOT(GetKeyEvent(int *)), Qt::DirectConnection);

   QObject::connect(&MyThread, SIGNAL(SGetBufferedKeyEvents(int *)),
           this, SLOT(GetBufferedKeyEvents(int *)), Qt::DirectConnection);

   QObject::connect(&MyThread, SIGNAL(SGetBufferedKeyPressEvents(int *)),
           this, SLOT(GetBufferedKeyPressEvents(int *)), Qt::DirectConnection);

   QObject::connect(&MyThread, SIGNAL(SGetMouseState (struct MouseInfo *)),
           this, SLOT(GetMouseState (struct MouseInfo *)), Qt::DirectConnection);

   QObject::connect(&MyThread, SIGNAL(SGetMouseEvent (struct MouseInfo *)),
           this, SLOT(GetMouseEvent (struct MouseInfo *)), Qt::DirectConnection);

   QObject::connect(&MyThread, SIGNAL(SSetAutoUpdate(bool)),
           this, SLOT(SetAutoUpdate(bool)));

   QObject::connect(&MyThread, SIGNAL(Ssetlinestyle(int)),
           this, SLOT(setlinestyle(int)));

   QObject::connect(&MyThread, SIGNAL(Ssetcolor(unsigned long)),
              this, SLOT(setcolor(unsigned long)));

   QObject::connect(&MyThread, SIGNAL(Spixel(int, int)),
           this, SLOT(pixel(int, int)));

   QObject::connect(&MyThread, SIGNAL(SGetPixel(unsigned long *, int, int)),
              this, SLOT(GetPixel(unsigned long *, int, int)), Qt::BlockingQueuedConnection);

   QObject::connect(&MyThread, SIGNAL(Sline(int, int,  int, int)),
           this, SLOT(line(int, int,  int, int)));

   QObject::connect(&MyThread, SIGNAL(Spolyline(QPolygon *)),
           this, SLOT(polyline(QPolygon *)));

   QObject::connect(&MyThread, SIGNAL(Spolygon(QPolygon *)),
           this, SLOT(polygon(QPolygon *)));

   QObject::connect(&MyThread, SIGNAL(SDrawEllipse(int, int, int, int)),
              this, SLOT(DrawEllipse(int, int, int, int)));

   QObject::connect(&MyThread, SIGNAL(SDrawText(int, int,  QString)),
              this, SLOT(DrawText(int, int,  QString)));

   QObject::connect(&MyThread, SIGNAL(SSelectFont(const char *, int, int)),
              this, SLOT(SelectFont(const char *, int, int)));

   QObject::connect(&MyThread, SIGNAL(SGetTextDimensions (TextDimensionType *, const char *)),
              this, SLOT(GetTextDimensions (TextDimensionType *, const char *)), Qt::BlockingQueuedConnection);

 //  QObject::connect(&MyThread, SIGNAL(SIncrementUnhandledCommands()),
 //             this, SLOT(IncrementUnhandledCommands()), Qt::DirectConnection);

   QObject::connect(&MyThread, SIGNAL(Sclose()),
              this, SLOT(DoClose()));

   QObject::connect(&MyThread, SIGNAL(Scleardevice()),
              this, SLOT(cleardevice()));

   QObject::connect(&MyThread, SIGNAL(SCreateImage(int *, int,  int)),
              this, SLOT(CreateImage(int *, int,  int)), Qt::DirectConnection);

   QObject::connect(&MyThread, SIGNAL(SCreateImage(int *, int, int, int, int, int)),
              this, SLOT(CreateImage(int *, int, int, int, int, int)), Qt::BlockingQueuedConnection);

   QObject::connect(&MyThread, SIGNAL(SSetEditedImage(int)),
              this, SLOT(SetEditedImage(int)));

   QObject::connect(&MyThread, SIGNAL(SDrawImage(int, int, int)),
              this, SLOT(DrawImage(int, int, int)));

   QObject::connect(&MyThread, SIGNAL(SCopyToImage(int, int, int, int, int)),
              this, SLOT(CopyToImage(int, int, int, int, int)));

   QObject::connect(&MyThread, SIGNAL(SAddAlphaMask(int, int, unsigned long)),
                 this, SLOT(AddAlphaMask(int, int, unsigned long)));


   QObject::connect(&MyThread, SIGNAL(SGetImageSize(int, int *, int *)),
              this, SLOT(GetImageSize(int, int *, int *)), Qt::BlockingQueuedConnection);

   QObject::connect(&MyThread, SIGNAL(SGetPixelDataPointer(int, void **, int *)),
              this, SLOT(GetPixelDataPointer(int, void **, int *)), Qt::BlockingQueuedConnection);

   QObject::connect(&MyThread, SIGNAL(SLoadImage(int *, const char *)),
              this, SLOT(LoadImage(int *, const char *)), Qt::BlockingQueuedConnection);

   QObject::connect(&MyThread, SIGNAL(SSaveImage(int, const char *, const char *, int)),
              this, SLOT(SaveImage(int, const char *, const char *, int)), Qt::BlockingQueuedConnection);

   QObject::connect(&MyThread, SIGNAL(SRotate(float)),
              this, SLOT(Rotate(float)));

   QObject::connect(&MyThread, SIGNAL(STranslate(float, float)),
              this, SLOT(Translate(float, float)));

   QObject::connect(&MyThread, SIGNAL(SScale(float, float)),
              this, SLOT(Scale(float, float)));

   QObject::connect(&MyThread, SIGNAL(SResetTransformations()),
              this, SLOT(ResetTransformations()));

   QObject::connect(&MyThread, SIGNAL(SDestroyImage(int)),
              this, SLOT(DestroyImage(int)));

   QObject::connect(&MyThread, SIGNAL(SPlaySoundContinuous(QString)),
              this, SLOT(PlaySoundContinuous(QString)));

   QObject::connect(&MyThread, SIGNAL(SStartPlaySoundContinuous()),
              this, SLOT(StartPlaySoundContinuous()));

   QObject::connect(&MyThread, SIGNAL(SStopContinuousSound()),
              this, SLOT(StopContinuousSound()));

   QObject::connect(&MyThread, SIGNAL(SPlaySound(QString)),
              this, SLOT(PlaySound(QString)));

   QObject::connect(MyTimer, SIGNAL(timeout ()),
              this, SLOT(TimerTick()));

   QObject::connect(&MyThread, SIGNAL(SStartTimer(int, void *, void *)),
              this, SLOT(StartTimer(int, void *, void *)));

   QObject::connect(&MyThread, SIGNAL(SStopTimer()),
              this, SLOT(StopTimer()));

   QObject::connect(&MyThread, SIGNAL(SOpenFileDialog(bool *, int, const char *, const char *, const char *, const char *, int, char *, int, char *, int)),
              this, SLOT(OpenFileDialog(bool *, int, const char *, const char *, const char *, const char *, int, char *, int, char *, int)), Qt::BlockingQueuedConnection);

 	//ToPaint = 0;
	//Repaint = 0;
   /* Initialize our attributes */
	NumberOfUnhandledKeyPresses = 0;
	//NumberOfUnhandledCommands = 0;
//	vol = 5;
	int Width = 400;
	int Height = 400;
	AutoUpdate = true;
	AcceptClosing = false;
	setAutoFillBackground (false);
	/* Create our main image to paint on, all drawingcommands   */
	/* will draw onto this image, and this image will be copied */
	/* onto the screen (double buffering)                       */
	Image = new QImage(Width, Height, QImage::Format_ARGB32);

	/* Create a painter for this image and make it the current painter */
	MyPainter = new QPainter(Image);
	CurrentPainter = MyPainter;
	CurrentImage = -1;

   /* Enable mousetracking */
	CurrentMouse.ButtonState = W_BUTTON_NO_EVENT;
   CurrentMouse.MousePosX = 0;
   CurrentMouse.MousePosY = 0;

	setMouseTracking(true);

	MyBackgroundSong = 0;

	/* Start our thread (which will call gfx_main) */
	MyThread.start();
}
/*****************************************************************************/
/*  End  Method : MainGui                                                    */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : ~MainGui                                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Destructor for a MainGui. Frees all allocated resources    */
/*                                                                           */
/*  Type        : Constructor                                                */
/*                                                                           */
/*  Input Para  : parent Pointer to parent widget                            */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
MainGui::~MainGui()
{
   /* Method data declaration      */

   /* Method code declaration      */

    /* Try to stop our main thread */
    MyThread.stop();
    MyTimer->stop();

    /* If it will not stopp on its own, just kill it... */
    if (MyThread.wait(2000) == false) {
       // Und bist Du nicht willig, so brauch ich Gewalt...
       MyThread.terminate();
       MyThread.wait(1000);
    };

    /* Free allocated painter and images */
    delete MyPainter;
    delete Image;
    for (unsigned int i = 0; i < ImageBuffers.size(); i++) {
       delete ImageBuffers[i];
    }
    delete MyTimer;
}
/*****************************************************************************/
/*  End  Method : ~MainGui                                                   */
/*****************************************************************************/


/*****************************************************************************/
/*  Method      : setfillstyle                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets the fill style for future draw commands               */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : FillType: 0 = No Fill, 1 = Fill                            */
/*                RGBColor: Color for future fillings                        */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::setfillstyle(int FillType, unsigned long RGBColor)
{
   /* Method data declaration      */

   /* Method code declaration      */

   if (FillType == 0) {
      CurrentPainter->setBrush(QBrush(QColor::fromRgba(RGBColor), Qt::NoBrush));
   } else {
      CurrentPainter->setBrush(QBrush(QColor::fromRgba(RGBColor), Qt::SolidPattern));
   }
}
/*****************************************************************************/
/*  End  Method : setfillstyle                                               */
/*****************************************************************************/


/*****************************************************************************/
/*  Method      : SetDrawMode                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets mode of future draws                                  */
/*                  Selects how next drawings will be drawn on existing      */
/*                  drawing. (Drawing over, mixing, blending, ...            */
/*                                                                           */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : Mode:     How future drawings are merged into image        */
/*                          May be one of the predefined modes or a standard */
/*                          QT Mode                                          */
/*                                                                           */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::SetDrawMode(int Mode)
{
   /* Method data declaration      */

   /* Method code declaration      */

   switch(Mode) {
      case EDM_COPY:     CurrentPainter->setCompositionMode(QPainter::CompositionMode_Source); break;
      case EDM_COPYALPHA:CurrentPainter->setCompositionMode(QPainter::CompositionMode_DestinationIn); break;
      case EDM_OVER:     CurrentPainter->setCompositionMode(QPainter::CompositionMode_SourceOver); break;
      case EDM_MIX:      CurrentPainter->setCompositionMode(QPainter::CompositionMode_SourceAtop); break;
      case EDM_XOR:      CurrentPainter->setCompositionMode(QPainter::CompositionMode_Difference); break;
      case EDM_MULTIPLY: CurrentPainter->setCompositionMode(QPainter::CompositionMode_Multiply); break;
      default:          if((Mode >= 0) && (Mode <= QPainter::RasterOp_SourceAndNotDestination)) {
                           CurrentPainter->setCompositionMode((QPainter::CompositionMode)Mode);
                        } else {
                           CurrentPainter->setCompositionMode(QPainter::CompositionMode_SourceOver);
                        }
                        break;
   }
}
/*****************************************************************************/
/*  End  Method : SetDrawMode                                                */
/*****************************************************************************/


/*****************************************************************************/
/*  Method      : setcolor                                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets the draw color for future drawings                    */
/*                                                                           */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : RGBColor:  Color for future drawings                       */
/*                                                                           */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::setcolor(unsigned long RGBColor)
{
   /* Method data declaration      */

   /* Method code declaration      */

   QPen MyPen(CurrentPainter->pen());
   MyPen.setBrush(QBrush(QColor::fromRgba(RGBColor)));
   CurrentPainter->setPen(MyPen);
}
/*****************************************************************************/
/*  End  Method : setcolor                                                   */
/*****************************************************************************/


/*****************************************************************************/
/*  Method      : SetBackgroundColor                                         */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets the backgroundcolor for text (and space in dotted     */
/*                lines)                                                     */
/*                                                                           */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : RGBColor:  Color for future drawings                       */
/*                           0 means 'Do not draw any background'            */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::SetBackgroundColor(unsigned long RGBColor)
{
   /* Method data declaration      */

   /* Method code declaration      */

   if (RGBColor != 0) {
      CurrentPainter->setBackgroundMode (Qt::OpaqueMode);
      CurrentPainter->setBackground(QBrush(QColor::fromRgba(RGBColor), Qt::SolidPattern));
   } else {
      CurrentPainter->setBackgroundMode (Qt::TransparentMode);
   }
}
/*****************************************************************************/
/*  End  Method : SetBackgroundColor                                         */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : setlinestyle                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets the linewidth                                         */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : Width:  Requested future linewidth for drawings            */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::setlinestyle(int Width)
{
   /* Method data declaration      */

   /* Method code declaration      */

   QPen MyPen(CurrentPainter->pen());
   MyPen.setStyle(Qt::SolidLine);
   MyPen.setWidth(Width);
   CurrentPainter->setPen(MyPen);
}
/*****************************************************************************/
/*  End  Method : setlinestyle                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : line                                                       */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a line with the current color and width              */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : x1,y1:  Startcoordinate of line                            */
/*                x2,y2:  Endcoordinate of line                              */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::line(int x1 , int y1, int x2, int y2)
{
   /* Method data declaration      */

   /* Method code declaration      */

   CurrentPainter->drawLine (x1, y1, x2, y2);
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : line                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : polygon                                                    */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a (filled) polygon with the current color and width  */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : Polygon:  Edge points of polyline, will be deleted after   */
/*                          use (For faster inter thread communication)      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 11.04.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::polygon(QPolygon *Polygon)
{
   /* Method data declaration      */

   /* Method code declaration      */

   CurrentPainter->drawPolygon(*Polygon);
   delete Polygon;
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : polygon                                                    */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : polyline                                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a (unclosed) polyline with the current line color    */
/*                and line width                                             */
/*                For a closed, unfilled polygon, first and last point must  */
/*                be the same                                                */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : Polygon:  Edge points of polyline, will be deleted after   */
/*                          use (For faster inter thread communication)      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 11.04.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::polyline(QPolygon *Polygon)
{
   /* Method data declaration      */

   /* Method code declaration      */

   CurrentPainter->drawPolyline(*Polygon);
   delete Polygon;
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : polyline                                                   */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : pixel                                                      */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a pixel with the current color                       */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : x,y:    Coordinate of pixel to draw                        */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::pixel(int x, int y)
{
   /* Method data declaration      */

   /* Method code declaration      */

   CurrentPainter->drawPoint(x, y);
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : pixel                                                      */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetPixel                                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the color of the given pixel                       */
/*                This Slot must be direct connected, since it returns a     */
/*                Value by a pointer                                         */
/*                                                                           */
/*  Type        : Public Slot (Direct connected)                             */
/*                                                                           */
/*  Input Para  : x,y:      Coordinate of pixel to get                       */
/*                RGBColor: Pointer to where place the color of the pixel    */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetPixel(unsigned long *RGBColor, int x, int y)
{
   /* Method data declaration      */

   /* Method code declaration      */

   QImage *ImageToReadFrom = Image;

   /* Find the currently active Image */
   if ((CurrentImage >= 0) && (CurrentImage < (int)ImageBuffers.size())) {
      ImageToReadFrom = ImageBuffers[CurrentImage];
   }
   QRgb Color = ImageToReadFrom->pixel(x,y);

   *RGBColor = Color;
}
/*****************************************************************************/
/*  End  Method : GetPixel                                                   */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : bar                                                        */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a filled rectangle                                   */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : x,y:      Coordinate of lower left edge of rectangle       */
/*                w, h:     Width and Height of rectangle                    */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::bar(int x , int y, int w, int h)
{
   /* Method data declaration      */

   /* Method code declaration      */

   CurrentPainter->drawRect(x, y, w, h);
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : bar                                                        */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : DrawEllipse                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a filled ellipse                                     */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : x,y:      Coordinate of lower left edge of ellipse         */
/*                w, h:     Width and Height of ellipse                      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::DrawEllipse(int x, int y, int w, int h)
{
   /* Method data declaration      */

   /* Method code declaration      */

   CurrentPainter->drawEllipse (x, y, w, h);
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : DrawEllipse                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : DrawText                                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a text at the given position                         */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : x,y:      Coordinate of lower (Baseline) left edge of text */
/*                Text:     Text to draw                                     */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::DrawText(int x, int y, QString Text)
{
   /* Method data declaration      */

   /* Method code declaration      */

   CurrentPainter->drawText (x, y, Text );
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : DrawText                                                   */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : SelectFont                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Selects Font for future writings                           */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : FontName: Name of new Font, NULL if no new font is requred */
/*                Points:   Sets size of font                                */
/*                Style:    Sets style (italic, bold, black, normal) of font */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::SelectFont(const char *FontName, int Points, int Style)
{
   /* Method data declaration      */

   QFont NewFont;

   /* Method code declaration      */

   /* Use current font if no Fontname was given */
   if (FontName != NULL) {
      NewFont.setFamily(QString(FontName));
   } else {
      NewFont.setFamily(CurrentPainter->font().family());
   }
   NewFont.setPointSize(Points);

   if (Style & FONT_ITALIC) {
      NewFont.setItalic(true);
   }
   if (Style & FONT_BOLD) {
      NewFont.setWeight(QFont::Bold);
   }
   if (Style & FONT_BLACK) {
      NewFont.setWeight(QFont::Black);
   }
   CurrentPainter->setFont(NewFont);
}
/*****************************************************************************/
/*  End  Method : SelectFont                                                 */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetTextDimensions                                          */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns space the given text would use to draw with the    */
/*                current settings.                                          */
/*                This Slot must be direct connected, since it returns a     */
/*                Value by a pointer                                         */
/*                                                                           */
/*  Type        : Public Slot (Direct connected)                             */
/*                                                                           */
/*  Input Para  : Text:     Text to get size of                              */
/*                TextInfo: Pointer to place to store the size of the text   */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetTextDimensions (TextDimensionType *TextInfo, const char *Text)
{
   /* Method data declaration      */

   /* Method code declaration      */


 //  mutex.lock();
 //  while (NumberOfUnhandledCommands != 0) {
 //     mutex.unlock();
 //    // QThread::currentThread()->wait(1);  // Need to find some nicer synchronisation  QWaitCondition?
 //     mutex.lock();
 //  }
   /* Get Boundingbox, define just a dummy box as parameter */
 //  QRect BoundingBox = CurrentPainter->boundingRect ( 0, 0, 100, 100, Qt::TextSingleLine, QString(Text));
   QRect BoundingBox = CurrentPainter->fontMetrics().boundingRect(QString(Text));
   TextInfo->Length = BoundingBox.right();
   TextInfo->Overhang = -BoundingBox.left();
   TextInfo->Up   = -BoundingBox.top();
   TextInfo->Down = BoundingBox.bottom();
 //  mutex.unlock();
}
/*****************************************************************************/
/*  End  Method : GetTextDimensions                                          */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : SetAutoUpdate                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Enables or disables autoupdate mode. If autoupdate is      */
/*                disabled, drawings will only be done in the internal       */
/*                image buffer, there will be no update on the screen.       */
/*                If autoupdate is enabled, drawings will  be done in the    */
/*                internal image buffer and automatically updated on the     */
/*                screen.                                                    */
/*                If autoupdate gets enabled, there will also be an          */
/*                imediately update.                                         */
/*                This mechanism (Doublebuffering) is required to reduce     */
/*                flickering.                                                */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : Mode:     0 = Disable, 1 = enable autoupdate               */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::SetAutoUpdate(bool Mode)
{
   /* Method data declaration      */

   /* Method code declaration      */

   AutoUpdate = Mode;
   if (Mode) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : SetAutoUpdate                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : paintEvent                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system, whenever a repaint of the window    */
/*                is required, just repaints everything. (Copies the image   */
/*                onto the screen).                                          */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : QPaintEvent, not used                                      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::paintEvent(QPaintEvent * /* event */)
{
   /* Method data declaration      */

   /* Method code declaration      */

   QPainter painter(this);
   painter.drawImage(QPoint(0, 0), *Image);
}
/*****************************************************************************/
/*  End  Method : paintEvent                                                 */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : SetWindowSize                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets the window dimensions to the given size, will also    */
/*                clear the contents of the current window                   */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : Width, Height:  New size for the window                    */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::SetWindowSize(int Width, int Height)
{
   /* Method data declaration      */

   QBrush     OldBrush;
   QPen       OldPen;
   QBrush     OldBackground;
   Qt::BGMode OldBGMode = Qt::OpaqueMode;
   QTransform OldTransform;
   QFont      OldFont;
   QPainter::CompositionMode OldCompositionMode = QPainter::CompositionMode_SourceOver;

   /* Method code declaration      */

   if ((Width < 0) || (Height < 0)) {
      setWindowState(Qt::WindowFullScreen);
      show();
      Width = width();
      Height = height();
      //printf("h:%d w:%d", Width, Height);
   }

   if (CurrentPainter == MyPainter) {
      CurrentPainter = NULL;
   }
   /* save all settings of current painter */
   if (MyPainter != NULL) {
      OldBrush      = MyPainter->brush();
      OldPen        = MyPainter->pen();
      OldBackground = MyPainter->background();
      OldBGMode     = MyPainter->backgroundMode();
      OldTransform  = MyPainter->transform();
      OldFont       = MyPainter->font();
      OldCompositionMode = MyPainter->compositionMode();

      delete MyPainter;
   }
   /* Create a new image with the required size */
   if (Image != NULL) {
      delete Image;
   }
   Image = new QImage(Width, Height, QImage::Format_ARGB32);

   /* Create a painter for the new image */
   MyPainter = new QPainter(Image);

   /* and restore the paintersettings */
   MyPainter->setBrush(OldBrush);
   MyPainter->setPen(OldPen);
   MyPainter->setBackground(OldBackground);
   MyPainter->setBackgroundMode(OldBGMode);
   MyPainter->setTransform(OldTransform);
   MyPainter->setFont(OldFont);
   MyPainter->setCompositionMode(OldCompositionMode);

   if (CurrentPainter == NULL) {
      CurrentPainter = MyPainter;
   }

   /* and change the size of our window */
   setFixedSize(Width, Height);
   activateWindow();
}
/*****************************************************************************/
/*  End  Method : SetWindowSize                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : keyPressEvent                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system whenever a keystroke is received for */
/*                our window. Stores the keystrokes in a queue and protects  */
/*                queue by a mutex to make it threadsave.                    */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : event: Received keyevent                                   */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                15.12.2009  IO  Changed order of evaluatuion, check first  */
/*                                for special keys, then for ASCII-Keys      */
/*                                                                           */
/*****************************************************************************/
void MainGui::keyPressEvent ( QKeyEvent * event )
{
   /* Method data declaration      */
   int Code = 0;

   /* Method code declaration      */

   /* Translate non-ascii-keystrokes into special codes */
      // printf("Key = <%d>", event->key());
      switch (event->key()) {

         case Qt::Key_Up:         Code = W_KEY_CURSOR_UP; break;
         case Qt::Key_Down:       Code = W_KEY_CURSOR_DOWN; break;
         case Qt::Key_Left:       Code = W_KEY_CURSOR_LEFT; break;
         case Qt::Key_Right:      Code = W_KEY_CURSOR_RIGHT; break;
         case Qt::Key_F1:         Code = W_KEY_F1; break;
         case Qt::Key_F2:         Code = W_KEY_F2; break;
         case Qt::Key_F3:         Code = W_KEY_F3; break;
         case Qt::Key_F4:         Code = W_KEY_F4; break;
         case Qt::Key_F5:         Code = W_KEY_F5; break;
         case Qt::Key_F6:         Code = W_KEY_F6; break;
         case Qt::Key_F7:         Code = W_KEY_F7; break;
         case Qt::Key_F8:         Code = W_KEY_F8; break;
         case Qt::Key_F9:         Code = W_KEY_F9; break;
         case Qt::Key_F10:        Code = W_KEY_F10; break;
         case Qt::Key_F11:        Code = W_KEY_F11; break;
         case Qt::Key_F12:        Code = W_KEY_F12; break;

         case Qt::Key_Shift:      Code = W_KEY_SHIFT; break;
         case Qt::Key_Control:    Code = W_KEY_CTRL; break;
         case Qt::Key_Meta:       Code = W_KEY_META; break;
         case Qt::Key_Alt:        Code = W_KEY_ALT; break;
         case Qt::Key_AltGr:      Code = W_KEY_ALTCTRL; break;

         default:                 if (event->text().size() > 0) {
                                     Code = event->text().at(0).toLatin1();
                                     // printf("Code = <%d>", Code);
                                  } else {
                                     Code = 0;
                                  }
                                  break;
   }

   /* If code was not recognized, just ignore it */
   if (Code == 0) {
      event->ignore();
   } else {
      /* Mark autorepeat events */
      if (event->isAutoRepeat()) {
         Code |= W_KEY_AUTOREPEAT;
      }
      /* Accept keyevent and store it in queue */
      event->accept();
      mutex.lock();
      /* Dont count modifierkeys (shift, alt, ctrl, ...) as keypresses */
      if ((Code & W_KEY_MODIFIER ) == 0) {
         NumberOfUnhandledKeyPresses++;
      }
      KeyBuffer.push_front(Code);
      mutex.unlock();
   }
}
/*****************************************************************************/
/*  End  Method : keyPressEvent                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : keyReleaseEvent                                            */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system whenever a keyrelease is received for*/
/*                our window. Stores the key event in a queue and protects   */
/*                queue by a mutex to make it threadsave.                    */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : event: Received keyevent                                   */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::keyReleaseEvent(QKeyEvent *event)
{
   /* Method data declaration      */
   int Code = 0;

   /* Method code declaration      */

   /* Translate non-ascii-keys into special codes */
   if (event->text().size() > 0) {
      Code = event->text().at(0).toLatin1();
    //  printf("Released Code = <%d>", Code);
   } else {
    //  printf("Released Key = <%d>", event->key());
      switch (event->key()) {

         case Qt::Key_Up:         Code = W_KEY_CURSOR_UP; break;
         case Qt::Key_Down:       Code = W_KEY_CURSOR_DOWN; break;
         case Qt::Key_Left:       Code = W_KEY_CURSOR_LEFT; break;
         case Qt::Key_Right:      Code = W_KEY_CURSOR_RIGHT; break;
         case Qt::Key_F1:         Code = W_KEY_F1; break;
         case Qt::Key_F2:         Code = W_KEY_F2; break;
         case Qt::Key_F3:         Code = W_KEY_F3; break;
         case Qt::Key_F4:         Code = W_KEY_F4; break;
         case Qt::Key_F5:         Code = W_KEY_F5; break;
         case Qt::Key_F6:         Code = W_KEY_F6; break;
         case Qt::Key_F7:         Code = W_KEY_F7; break;
         case Qt::Key_F8:         Code = W_KEY_F8; break;
         case Qt::Key_F9:         Code = W_KEY_F9; break;
         case Qt::Key_F10:        Code = W_KEY_F10; break;
         case Qt::Key_F11:        Code = W_KEY_F11; break;
         case Qt::Key_F12:        Code = W_KEY_F12; break;

         case Qt::Key_Shift:      Code = W_KEY_SHIFT; break;
         case Qt::Key_Control:    Code = W_KEY_CTRL; break;
         case Qt::Key_Meta:       Code = W_KEY_META; break;
         case Qt::Key_Alt:        Code = W_KEY_ALT; break;
         case Qt::Key_AltGr:      Code = W_KEY_ALTCTRL; break;

         default:                 Code = 0; break;
      }
   }
   /* If code was not recognized, just ignore it */
   if (Code == 0) {
      event->ignore();
   } else {
      /* mark autorepeat events, should not be possible for release events... */
      if (event->isAutoRepeat()) {
         Code |= W_KEY_AUTOREPEAT;
      }
      /* mark event as release event */
      Code |= W_KEY_RELEASED;

      /* Accept event and store it in queue */
      event->accept();
      mutex.lock();
      KeyBuffer.push_front(Code);
      mutex.unlock();
   }
}
/*****************************************************************************/
/*  End  Method : keyReleaseEvent                                            */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : mouseMoveEvent                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system whenever a mouse move is detected for*/
/*                our window. Keps track of the mouse position and button    */
/*                state.                                                     */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : event: Received mouse event                                */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::mouseMoveEvent(QMouseEvent *event)
{
   /* Method data declaration      */

   /* Method code declaration      */

   /* Just store mouse info for future reference */
   CurrentMouse.ButtonState = event->buttons();
   CurrentMouse.MousePosX = event->x();
   CurrentMouse.MousePosY = event->y();
}
/*****************************************************************************/
/*  End  Method : mouseMoveEvent                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : mousePressEvent                                            */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system whenever a mouse button is pressed   */
/*                for our window. Keeps track of the mouse position and      */
/*                button state and stores mouse event in mouse event queue.  */
/*                                                                           */
/*                ToDo: Perhaps we should limit queuelength if nobody is     */
/*                      interested in mouseevents                            */
/*                  Done, limited to 100 entries                             */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : event: Received mouse event                                */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::mousePressEvent   (QMouseEvent *event)
{
   /* Method data declaration      */
   MouseInfoType Info;

   /* Method code declaration      */

   /* Determine mousebutton event and translate it for queue */
   switch (event->button()) {
      case Qt::LeftButton:  Info.ButtonState = W_BUTTON_LEFT   | W_BUTTON_PRESSED; break;
      case Qt::RightButton: Info.ButtonState = W_BUTTON_RIGHT  | W_BUTTON_PRESSED; break;
      case Qt::MidButton:   Info.ButtonState = W_BUTTON_MIDDLE | W_BUTTON_PRESSED; break;
      default:              Info.ButtonState = W_BUTTON_NO_EVENT; break;
   }

   /* Add mouseposition */
   Info.MousePosX = event->x();
   Info.MousePosY = event->y();
   Info.MouseWheelDelta = 0;
   Info.MouseWheelKind  = MW_NONE;

   /* also update our local mouse info for future reference */
   CurrentMouse.ButtonState = event->buttons();
   CurrentMouse.MousePosX = event->x();
   CurrentMouse.MousePosY = event->y();

   /* store event in queue, if any interesting event happened at all */
   if (Info.ButtonState != W_BUTTON_NO_EVENT) {
      mutex.lock();
      MouseEventBuffer.push_front(Info);

      /* If nobody asks for mouseevents, discard oldest */
      while (MouseEventBuffer.size() > 100) {
         MouseEventBuffer.pop_back();
      }
      mutex.unlock();
   }
}
/*****************************************************************************/
/*  End  Method : mousePressEvent                                            */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : mouseReleaseEvent                                          */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system whenever a mouse button is released  */
/*                for our window. Keps track of the mouse position and       */
/*                button state and stores mouse event in mouse event queue.  */
/*                                                                           */
/*                ToDo: Perhaps we should limit queuelength if nobody is     */
/*                      interested in mouseevents                            */
/*                  Done, limited to 100 entries                             */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : event: Received mouse event                                */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::mouseReleaseEvent (QMouseEvent *event)
{
   /* Method data declaration      */
   MouseInfoType Info;

   /* Method code declaration      */

   /* Determine mousebutton event and translate it for queue */
   switch (event->button()) {
      case Qt::LeftButton:  Info.ButtonState = W_BUTTON_LEFT   | W_BUTTON_RELEASED; break;
      case Qt::RightButton: Info.ButtonState = W_BUTTON_RIGHT  | W_BUTTON_RELEASED; break;
      case Qt::MidButton:   Info.ButtonState = W_BUTTON_MIDDLE | W_BUTTON_RELEASED; break;
      default:              Info.ButtonState = W_BUTTON_NO_EVENT; break;
   }

   /* Add mouseposition */
   Info.MousePosX = event->x();
   Info.MousePosY = event->y();
   Info.MouseWheelDelta = 0;
   Info.MouseWheelKind  = MW_NONE;

   /* also update our local mouse info for future reference */
   CurrentMouse.ButtonState = event->buttons();
   CurrentMouse.MousePosX = event->x();
   CurrentMouse.MousePosY = event->y();

   /* store event in queue, if any interesting event happened at all */
   if (Info.ButtonState != W_BUTTON_NO_EVENT) {
      mutex.lock();
      MouseEventBuffer.push_front(Info);

      /* If nobody asks for mouseevents, discard oldest */
      while (MouseEventBuffer.size() > 100) {
         MouseEventBuffer.pop_back();
      }
      mutex.unlock();
   }
}
/*****************************************************************************/
/*  End  Method : mouseReleaseEvent                                          */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : mouseDoubleClickEvent                                      */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system whenever a mouse button is double    */
/*                clicked for our window. Keeps track of the mouse position  */
/*                and button state and stores mouse event in mouse event     */
/*                queue.                                                     */
/*                                                                           */
/*                ToDo: Perhaps we should limit queuelength if nobody is     */
/*                      interested in mouseevents                            */
/*                  Done, limited to 100 entries                             */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : event: Received mouse event                                */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::mouseDoubleClickEvent (QMouseEvent *event)
{
   /* Method data declaration      */
   MouseInfoType Info;

   /* Method code declaration      */

   /* Determine mousebutton event and translate it for queue */
   switch (event->button()) {
      case Qt::LeftButton:  Info.ButtonState = W_BUTTON_LEFT   | W_BUTTON_DOUBLECLICK; break;
      case Qt::RightButton: Info.ButtonState = W_BUTTON_RIGHT  | W_BUTTON_DOUBLECLICK; break;
      case Qt::MidButton:   Info.ButtonState = W_BUTTON_MIDDLE | W_BUTTON_DOUBLECLICK; break;
      default:              Info.ButtonState = W_BUTTON_NO_EVENT; break;
   }

   /* Add mouseposition */
   Info.MousePosX = event->x();
   Info.MousePosY = event->y();
   Info.MouseWheelDelta = 0;
   Info.MouseWheelKind  = MW_NONE;

   /* also update our local mouse info for future reference */
   CurrentMouse.ButtonState = event->buttons();
   CurrentMouse.MousePosX = event->x();
   CurrentMouse.MousePosY = event->y();

   /* store event in queue, if any interesting event happened at all */
   if (Info.ButtonState != W_BUTTON_NO_EVENT) {
      mutex.lock();
      MouseEventBuffer.push_front(Info);

      /* If nobody asks for mouseevents, discard oldest */
      while (MouseEventBuffer.size() > 100) {
         MouseEventBuffer.pop_back();
      }
      mutex.unlock();
   }
}
/*****************************************************************************/
/*  End  Method : mouseDoubleClickEvent                                      */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : wheelEvent                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system whenever a mouse wheele has          */
/*                changed for our window. Keeps track of the mouse position  */
/*                and button state and stores mouse event in mouse event     */
/*                queue.                                                     */
/*                Mose wheel change is given in 1/8 Degree                   */
/*                                                                           */
/*                ToDo: Perhaps we should limit queuelength if nobody is     */
/*                      interested in mouseevents                            */
/*                  Done, limited to 100 entries                             */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : event: Received mouse wheel event                          */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.3.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::wheelEvent(QWheelEvent  * event)
{

   /* Method data declaration      */
   MouseInfoType Info;

   /* Method code declaration      */

   /* Determine mousebutton event and translate it for queue */
   Info.ButtonState = W_MOUSE_WHEEL_CHANGE;
   if (event->buttons() & Qt::LeftButton )  {Info.ButtonState |= W_BUTTON_LEFT ;}
   if (event->buttons() & Qt::RightButton ) {Info.ButtonState |= W_BUTTON_RIGHT;}
   if (event->buttons() & Qt::MidButton )   {Info.ButtonState |= W_BUTTON_MIDDLE;}

   /* Add mouseposition */
   Info.MousePosX = event->x();
   Info.MousePosY = event->y();

   /* Get mouse wheel change (in 1/8 Degree) */
   Info.MouseWheelDelta = event->delta();
   switch(event->orientation()) {
      case Qt::Vertical: Info.MouseWheelKind  = MW_VERTICAL; break;
      case Qt::Horizontal: Info.MouseWheelKind  = MW_HORIZONTAL; break;
      default: Info.MouseWheelKind  = MW_NONE; break;
   }

   /* also update our local mouse info for future reference */
   CurrentMouse.ButtonState = event->buttons();
   CurrentMouse.MousePosX = event->x();
   CurrentMouse.MousePosY = event->y();

   /* Consume event */
   event->accept();

   /* store event in queue, if any interesting event happened at all */
   if (Info.ButtonState != W_BUTTON_NO_EVENT) {
      mutex.lock();
      MouseEventBuffer.push_front(Info);

      /* If nobody asks for mouseevents, discard oldest */
      while (MouseEventBuffer.size() > 100) {
         MouseEventBuffer.pop_back();
      }
      mutex.unlock();
   }
}
/*****************************************************************************/
/*  End  Method : wheelEvent                                                 */
/*****************************************************************************/



/*****************************************************************************/
/*  Method      : GetKeyEvent                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns oldest availlable keyevent                         */
/*                This Slot must be direct connected, since it returns a     */
/*                Value by a pointer                                         */
/*                Uses a mutex to be threadsafe                              */
/*                                                                           */
/*  Type        : Public Slot (Direct connected)                             */
/*                                                                           */
/*  Input Para  : Code: Pointer to place to store key event                  */
/*                      stores 0 if no key event was availlable              */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetKeyEvent(int *Code)
{
   /* Method data declaration      */

   /* Method code declaration      */

   *Code = 0;

   /* make it threadsafe */
   mutex.lock();

   /* Get oldest event from queue, if there is any at all */
   if (!KeyBuffer.empty()) {

      *Code = KeyBuffer.back();

      /* Decrement count of keypresses, if a keypress was consumed */
      if (((*Code & W_KEY_RELEASED) == 0) && ((*Code & W_KEY_MODIFIER) == 0))  {
         NumberOfUnhandledKeyPresses--;
      }
      /* remove event from queue */
      KeyBuffer.pop_back();

   }
   mutex.unlock();
}
/*****************************************************************************/
/*  End  Method : GetKeyEvent                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetBufferedKeyEvents                                       */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns number of availlable keyevents                     */
/*                This Slot must be direct connected, since it returns a     */
/*                Value by a pointer                                         */
/*                Uses a mutex to be threadsafe                              */
/*                                                                           */
/*  Type        : Public Slot (Direct connected)                             */
/*                                                                           */
/*  Input Para  : Count: Pointer to place to store number of availlable      */
/*                       key events                                          */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetBufferedKeyEvents(int *Count)
{
   /* Method data declaration      */

   /* Method code declaration      */

    mutex.lock();
   *Count = KeyBuffer.size();
    if (*Count == 0) {
       /* should be 0 allready in this case, but we do it anyway... */
       NumberOfUnhandledKeyPresses = 0;
    }
    mutex.unlock();
}
/*****************************************************************************/
/*  End  Method : GetBufferedKeyEvents                                       */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetBufferedKeyPressEvents                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns number of availlable key press events (without     */
/*                modifier keys like shift, alt, ctrl, ...)                  */
/*                This Slot must be direct connected, since it returns a     */
/*                Value by a pointer                                         */
/*                Uses a mutex to be threadsafe                              */
/*                                                                           */
/*  Type        : Public Slot (Direct connected)                             */
/*                                                                           */
/*  Input Para  : Count: Pointer to place to store number of availlable      */
/*                       key events                                          */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetBufferedKeyPressEvents(int *Count)
{
   /* Method data declaration      */

   /* Method code declaration      */

    mutex.lock();
   *Count = NumberOfUnhandledKeyPresses;
    mutex.unlock();
}
/*****************************************************************************/
/*  End  Method : GetBufferedKeyPressEvents                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetMouseEvent                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the oldest availlable mouse button event           */
/*                This Slot must be direct connected, since it returns a     */
/*                Value by a pointer                                         */
/*                Uses a mutex to be threadsafe                              */
/*                                                                           */
/*  Type        : Public Slot (Direct connected)                             */
/*                                                                           */
/*  Input Para  : Info: Pointer to place to store mouse info                 */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetMouseEvent(MouseInfoType *Info)
{
   /* Method data declaration      */

   /* Method code declaration      */

   mutex.lock();
   if (MouseEventBuffer.empty()) {
      /* List is empty, no event availlable */
      Info->ButtonState = W_BUTTON_NO_EVENT;
      Info->MousePosX = 0;
      Info->MousePosY = 0;
      Info->MouseWheelDelta = 0;
      Info->MouseWheelKind  = MW_NONE;
   } else {
      /* get oldest event and remove it from queue */
      *Info = MouseEventBuffer.back();
      MouseEventBuffer.pop_back();

   }
   mutex.unlock();
}
/*****************************************************************************/
/*  End  Method : GetMouseEvent                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetMouseState                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns current state of mouse (Button and position)       */
/*                This Slot must be direct connected, since it returns a     */
/*                Value by a pointer                                         */
/*                Uses a mutex to be threadsafe                              */
/*                                                                           */
/*  Type        : Public Slot (Direct connected)                             */
/*                                                                           */
/*  Input Para  : Info: Pointer to place to store mouse state info           */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetMouseState(MouseInfoType *Info)
{
   /* Method data declaration      */

   /* Method code declaration      */

   mutex.lock();
   *Info = CurrentMouse;

   /* Translate Buttonstate */
   if (CurrentMouse.ButtonState & Qt::LeftButton) {
      Info->ButtonState = W_BUTTON_LEFT;
   } else {
      Info->ButtonState = W_BUTTON_NONE;
   }
   if (CurrentMouse.ButtonState & Qt::RightButton) {
      Info->ButtonState |= W_BUTTON_RIGHT;
   }
   if (CurrentMouse.ButtonState & Qt::MidButton) {
      Info->ButtonState |= W_BUTTON_MIDDLE;
   }
   mutex.unlock();
}
/*****************************************************************************/
/*  End  Method : GetMouseState                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : DoClose                                                    */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Closes the window and shuts the application down           */
/*                                                                           */
/*  Type        : Public Slot                                                */
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
void MainGui::DoClose(void)
{
   /* Method data declaration      */

   /* Method code declaration      */

   AcceptClosing = true;
   close();
}
/*****************************************************************************/
/*  End  Method : DoClose                                                    */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : closeEvent                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Is called from system whenever a close of the window is    */
/*                requested (e.g. click on closing symbol of window)         */
/*                Translates it to a keypress event, except when closing     */
/*                was allowed due to a call to DoClose().                    */
/*                                                                           */
/*  Type        : Public                                                     */
/*                                                                           */
/*  Input Para  : event: Received close event                                */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::closeEvent(QCloseEvent *event)
{
   /* Method data declaration      */

   /* Method code declaration      */

   /* Accept closing only if allowed by DoClose() */
   if (AcceptClosing) {
      event->accept();
   } else {
      /* Otherwise, generate an 'WindowClose' Key-Event and ignore close request */
      mutex.lock();
      KeyBuffer.push_front(W_KEY_CLOSE_WINDOW);
      NumberOfUnhandledKeyPresses++;
      mutex.unlock();
      event->ignore();
   }
}
/*****************************************************************************/
/*  End  Method : closeEvent                                                 */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : SetQtOptions                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets directly some PPainter options                        */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : Option  Option to change                                   */
/*                Value   New value for selected option                      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.04.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::SetQtOptions(int Option, long Value)
{
    /* Method data declaration      */

    /* Method code declaration      */

    /* Modify selected option */
    switch(Option) {
      case Qt_BackgroundBrushColor:  {
              QBrush MyBrush(CurrentPainter->background());
              MyBrush.setColor(Value);
              CurrentPainter->setBackground(MyBrush);
           } break;

      case Qt_BackgroundBrushStyle: {
              QBrush MyBrush(CurrentPainter->background());
              MyBrush.setStyle((Qt::BrushStyle)Value);
              CurrentPainter->setBackground(MyBrush);
           } break;

      case Qt_BackgroundMode:
    	   CurrentPainter->setBackgroundMode((Qt::BGMode)Value);
    	   break;

      case Qt_BrushStyle: {
	          QBrush MyBrush(CurrentPainter->brush());
	          MyBrush.setStyle((Qt::BrushStyle)Value);
	          CurrentPainter->setBrush(MyBrush);
           } break;

      case Qt_CompositionMode:
    	   CurrentPainter->setCompositionMode((QPainter::CompositionMode)Value);
    	   break;

      case Qt_PenBrushStyle: {
  	          QPen MyPen(CurrentPainter->pen());
  	          QBrush MyPensBrush(MyPen.brush());
  	          MyPensBrush.setStyle((Qt::BrushStyle)Value);
  	          MyPen.setBrush(MyPensBrush);
  	          CurrentPainter->setPen(MyPen);
           } break;

      case Qt_PenCapStyle: {
   	          QPen MyPen(CurrentPainter->pen());
   	          MyPen.setCapStyle((Qt::PenCapStyle)Value);
   	          CurrentPainter->setPen(MyPen);
           } break;

      case Qt_PenCosmetic: {
   	          QPen MyPen(CurrentPainter->pen());
   	          MyPen.setCosmetic(Value);
   	          CurrentPainter->setPen(MyPen);
           } break;

      case Qt_PenJoinStyle: {
   	          QPen MyPen(CurrentPainter->pen());
   	          MyPen.setJoinStyle((Qt::PenJoinStyle)Value);
   	          CurrentPainter->setPen(MyPen);
           } break;

      case Qt_PenStyle: {
    	      QPen MyPen(CurrentPainter->pen());
    	      MyPen.setStyle((Qt::PenStyle)Value);
    	      CurrentPainter->setPen(MyPen);
           } break;

      default:
    	   break;
   }
}
/*****************************************************************************/
/*  End  Method : SetQtOptions                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : cleardevice                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Clears the current image (Fills with intransparent black)  */
/*                                                                           */
/*  Type        : Public Slot                                                */
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
void MainGui::cleardevice(void)
{
    /* Method data declaration      */

    /* Method code declaration      */

   /* Determine if image or screen is active */
   /* and fill with black                    */
   if (CurrentPainter == MyPainter) {
      if (Image != NULL) {
#if 1
         Image->fill(0xff000000);
#else
         QPen MyPen(CurrentPainter->pen());
         MyPen.setBrush(QBrush(QColor::fromRgb(0x0)));
         CurrentPainter->setPen(MyPen);
         CurrentPainter->setBrush(QBrush(QColor::fromRgb(0x0), Qt::SolidPattern));
         CurrentPainter->drawRect(Image->rect());
#endif
      }
   } else {
      if ((CurrentImage >= 0) && (CurrentImage < (int)ImageBuffers.size())) {
#if 1
         ImageBuffers[CurrentImage]->fill(0xff000000);
#else
         QPen MyPen(CurrentPainter->pen());
         MyPen.setBrush(QBrush(QColor::fromRgb(0x0)));
         CurrentPainter->setPen(MyPen);
         CurrentPainter->setBrush(QBrush(QColor::fromRgb(0x0), Qt::SolidPattern));
         CurrentPainter->drawRect(ImageBuffers[CurrentImage]->rect());
#endif
      }
   }
}
/*****************************************************************************/
/*  End  Method : cleardevice                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : CreateImage                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates an empty image and returns its Id                  */
/*                The image is placed in the ImageBuffers Array              */
/*                The Id is an index in the ImageBuffers  Array              */
/*                                                                           */
/*  Type        : Public Slot (Needs BlockingQueuedConnection)               */
/*                                                                           */
/*  Input Para  : Id pointer where to place the Id of the new image          */
/*                Width, Height Dimansions of new image                      */
/*                                                                           */
/*  Output Para : Id of Image in Id, or -1 on faillure                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::CreateImage(int *Id, int Width, int Height)
{
    /* Method data declaration      */

    /* Method code declaration      */

   /* create new image */
   QImage *NewImage = new QImage(Width, Height, QImage::Format_ARGB32);
   if(NewImage->isNull()) {
      /* Could not create image content, release it and return -1 */
      delete NewImage;
      *Id = -1;
   } else {
      /* and place in buffer if sucessfully created, return Id of new Image */
      ImageBuffers.push_back(NewImage);
      NewImage->fill(0xff000000);
      *Id = ImageBuffers.size()-1;
   }
}
/*****************************************************************************/
/*  End  Method : CreateImage                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : CreateImage                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates an image with contents from another Image and      */
/*                returns the Id of the new image                            */
/*                The image is placed in the ImageBuffers Array              */
/*                The Id is an index in the ImageBuffers  Array              */
/*                                                                           */
/*  Type        : Public Slot (Needs BlockingQueuedConnection)               */
/*                                                                           */
/*  Input Para  : Id pointer where to place the Id of the new image          */
/*                SrcID  Id of image to take contents from                   */
/*                x, y   position of where to take contents from             */
/*                Width, Height Dimansions of new image                      */
/*                                                                           */
/*  Output Para : Id of Image in Id, or -1 on faillure                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::CreateImage(int *Id, int SrcID, int x, int y, int Width, int Height)
{
    /* Method data declaration      */

    /* Method code declaration      */

   /* Find image to take contents from */
   QImage *ImageToClone = Image;

   if ((SrcID >= 0) && (SrcID < (int)ImageBuffers.size())) {
      ImageToClone = ImageBuffers[SrcID];
      if (ImageToClone == NULL) {
    	  *Id = -1;
    	  std::cout << "Illegal image ID in CreateSubImage() (Image was destroyed)\n";
    	  return;
      }
   }

   /* create new image with contents from source image */
   QImage *NewImage = new QImage(ImageToClone->copy(x, y, Width, Height));
   if(NewImage->isNull()) {

      /* Could not create image content, release it and return -1 */
      delete NewImage;
      *Id = -1;
   } else {
     /* and place in buffer if sucessfully created, return Id of new Image */
       ImageBuffers.push_back(NewImage);
      *Id = ImageBuffers.size()-1;
   }
}
/*****************************************************************************/
/*  End  Method : CreateImage                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : SetEditedImage                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Defines on which image future operations should happen     */
/*                                                                           */
/*                We have a main image (Screen) with Id -1 an a variable     */
/*                amount of user created Images with Id from 0 on upwards    */
/*                                                                           */
/*                The main Image is in  Image, its painter in MyPainter      */
/*                                                                           */
/*                User defined Images are stored in ImageBuffers Array       */
/*                                                                           */
/*                CurrentPainter allways holds a reference to a pointer      */
/*                to the active image. (That may also be MyPainter)          */
/*                Painters to User defined Images are created and deleted    */
/*                dynamically when edited image changes, MyPainter is        */
/*                never deleted                                              */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : ImageId  Id of image to use in future                      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::SetEditedImage(int ImageId)
{
    /* Method data declaration      */

    /* Method code declaration      */

   /* Determine new image */
   int NewImage;
   if ((ImageId >= 0) && (ImageId < (int)ImageBuffers.size())) {
      NewImage = ImageId;
      /* Check if Image is still valid */
      if (ImageBuffers[ImageId] == NULL) {
    	  std::cout << "Illegal image ID in SetEditedImage() (Image was destroyed)\n";
    	  return;
      }
   } else {
      NewImage = -1;
   }

   /* Copy all setings of painter to new image (Styles and colors) */
   if (CurrentImage != NewImage) {

      /* MyPainter is not to be deleted !!*/
      if (CurrentPainter != MyPainter) {
         /* Save setings and free painter */
         MyPainter->setBrush(CurrentPainter->brush());
         MyPainter->setPen(CurrentPainter->pen());
         MyPainter->setBackground(CurrentPainter->background());
         MyPainter->setBackgroundMode(CurrentPainter->backgroundMode());
         MyPainter->setTransform(CurrentPainter->transform());
         MyPainter->setFont(CurrentPainter->font());
         MyPainter->setCompositionMode(CurrentPainter->compositionMode());

         delete CurrentPainter;
      }

      if (NewImage < 0) {
         /* use painter for main image */
         CurrentPainter = MyPainter;
         CurrentImage = NewImage;
      } else {
         /* create new painter with saved settings for user defined images */
         CurrentPainter = new QPainter(ImageBuffers[NewImage]);
         CurrentImage = NewImage;

         CurrentPainter->setBrush(MyPainter->brush());
         CurrentPainter->setPen(MyPainter->pen());
         CurrentPainter->setBackground(MyPainter->background());
         CurrentPainter->setBackgroundMode(MyPainter->backgroundMode());
         CurrentPainter->setTransform(MyPainter->transform());
         CurrentPainter->setFont(MyPainter->font());
         CurrentPainter->setCompositionMode(MyPainter->compositionMode());
      }
   }
 //  mutex.lock();
 //  NumberOfUnhandledCommands--;
 //  mutex.unlock();
}
/*****************************************************************************/
/*  End  Method : SetEditedImage                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetImageSize                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the size of the given image                        */
/*                                                                           */
/*  Type        : Public Slot (Needs BlockingQueuedConnection)               */
/*                                                                           */
/*  Input Para  : ImageId  Id of image to determine size of                  */
/*                Width    Pointer to location to place width of image       */
/*                Height   Pointer to location to place height of image      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetImageSize(int ImageId, int *Width, int *Height)
{
    /* Method data declaration      */

    /* Method code declaration      */

   /* use screen if invalid ID */
   if ((ImageId < 0) || (ImageId >= (int)ImageBuffers.size())) {
      ImageId = -1;
   }

   /* find requested image and return its size */
   if (ImageId < 0) {
      *Width = Image->width();
      *Height = Image->height();
   } else {
	  if (ImageBuffers[ImageId] == NULL) {
		  std::cout << "Illegal image ID in GetImageSize() (Image was destroyed)\n";
	     return;
	  }
      *Width = ImageBuffers[ImageId]->width();
      *Height = ImageBuffers[ImageId]->height();
   }
}
/*****************************************************************************/
/*  End  Method : GetImageSize                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : GetPixelDataPointer                                        */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns a pointer to the Bitmap array of the given image   */
/*                (Be careful what You do whit this pointer...)              */
/*                                                                           */
/*  Type        : Public Slot (Needs BlockingQueuedConnection)               */
/*                                                                           */
/*  Input Para  : ImageId  Id of image to determine size of                  */
/*                Ptr      Pointer to location to place pointer to bitmap    */
/*                Length   Pointer to location to place length of bitmaparray*/
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::GetPixelDataPointer(int ImageId, void **Ptr, int *Length)
{
   /* Method data declaration      */

   /* Method code declaration      */

   /* Set default values */
   *Ptr = NULL;
   if(Length != NULL) {
      *Length = 0;
   }

   /* use screen if invalid ID */
   if ((ImageId < 0) || (ImageId >= (int)ImageBuffers.size())) {
      ImageId = -1;
   }

   if (ImageId < 0) {
      /* Get pixelpointer and arraysize from screen image */
      *Ptr = Image->bits();
      if(Length != NULL) {
 #if QT_VERSION >= 0x040600
         *Length = Image->byteCount();
 #else
         *Length = Image->bytesPerLine()*Image->height();
 #endif
      }
   } else {
      /* Get pixelpointer and arraysize from selected image */
      if (ImageBuffers[ImageId] == NULL) {
    	  std::cout << "Illegal image ID in GetPixelDataPointer() (Image was destroyed)\n";
		     return;
	   }
      *Ptr = ImageBuffers[ImageId]->bits();
      if(Length != NULL) {
 #if QT_VERSION >= 0x040600
         *Length = ImageBuffers[ImageId]->byteCount();
 #else
         *Length = ImageBuffers[ImageId]->bytesPerLine()*ImageBuffers[ImageId]->height();
 #endif
      }
   }
}
/*****************************************************************************/
/*  End  Method : GetPixelDataPointer                                        */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : DrawImage                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws the given image at the Position x/y onto the         */
/*                currently selected image                                   */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : ImageId  Id of image to be drawn                           */
/*                x/y      Position to draw image                            */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::DrawImage(int ImageId, int x, int y)
{
   /* Method data declaration      */
   QImage *ImageToDraw = Image;

   /* Method code declaration      */

   /* determine which image is to be drawn */
   if ((ImageId >= 0) && (ImageId < (int)ImageBuffers.size())) {
	      if (ImageBuffers[ImageId] == NULL) {
	    	  std::cout << "Illegal image ID in DrawImage() (Image was destroyed)\n";
			     return;
		   }
      ImageToDraw = ImageBuffers[ImageId];
   }

   /* and draw the selected image at its position */
   CurrentPainter->drawImage(QPoint(x, y), *ImageToDraw);
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : DrawImage                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : CopyToImage                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Copy a section from the currently selected image to the    */
/*                given image                                                */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : x/y      Position where to get content from                */
/*                Width/Height Size of area to copy                          */
/*                ImageId  Id of Image to copy section to                    */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::CopyToImage(int x, int y, int Width, int Height, int ImageId)
{
   /* Method data declaration      */
   QImage *ImageToCopyTo   = Image;
   QImage *ImageToCopyFrom = Image;

   /* Method code declaration      */

   /* Find destination image */
   if ((ImageId >= 0) && (ImageId < (int)ImageBuffers.size())) {
	      if (ImageBuffers[ImageId] == NULL) {
	    	  std::cout << "Illegal image ID in CopyToImage() (Image was destroyed)\n";
			     return;
		   }
      ImageToCopyTo = ImageBuffers[ImageId];
   }

   /* Create a painter for copy operation */
   QPainter MyPainter(ImageToCopyTo);

   /* Get current image */
   if ((CurrentImage >= 0) && (CurrentImage < (int)ImageBuffers.size())) {
      ImageToCopyFrom = ImageBuffers[CurrentImage];
   }

   /* and copy selection into destination image */
   MyPainter.drawImage (0, 0, *ImageToCopyFrom, x, y, Width, Height);
   if (AutoUpdate) {
      update();
   }
}
/*****************************************************************************/
/*  End  Method : DrawImage                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : AddAlphaMask                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates an Alpha mask for the selected image. This allows  */
/*                to copy only certain parts of image when using copy        */
/*                or draw image.                                             */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : ImageId  Id of Image to create mask for                    */
/*                Mode     How the Mask is to be created                     */
/*                           MSK_COLOR_IN  All Pixels matching the given     */
/*                                         Color will be transparent         */
/*                           MSK_COLOR_OUT All Pixels Not matching the given */
/*                                         Color will be transparent         */
/*                           MSK_AUTO      Tries automatically to make       */
/*                                         border pixels transparent         */
/*                Color    Color to use for MASK_IN or MASK_OUT              */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::AddAlphaMask(int ImageId, int Mode, unsigned long Color)
{
   /* Method data declaration      */
   QImage *ImageToMask = Image;
   QImage Mask;

   /* Method code declaration      */

   /* Find selected image */
   if ((ImageId >= 0) && (ImageId < (int)ImageBuffers.size())) {
	      if (ImageBuffers[ImageId] == NULL) {
	    	  std::cout << "Illegal image ID in AddAlphaMask() (Image was destroyed)\n";
			     return;
		   }
	   ImageToMask = ImageBuffers[ImageId];
   }

   /* Create mask as required */
   switch (Mode & 0xFF) {
      case MSK_COLOR_IN:   Mask = ImageToMask->createMaskFromColor(Color, Qt::MaskInColor); break;
      case MSK_COLOR_OUT:  Mask = ImageToMask->createMaskFromColor(Color, Qt::MaskOutColor); break;
      case MSK_AUTO:       Mask = ImageToMask->createHeuristicMask(); break;
      default:             return;  /* Just do nothing at all */
   }

   /* Set Mask directly (Does not work in all cases) */
   if (Mode & 0x100000) {
      ImageToMask->setAlphaChannel(Mask);
   } else {
//   *ImageToMask = Mask;
//      if (!ImageToMask->paintingActive()) {
//         QPainter MyPainter(ImageToMask);
         //MyPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
         //MyPainter.drawImage (0, 0, Mask);
//         MyPainter.setCompositionMode(QPainter::CompositionMode_Source);
 //     }
      /* Set Mask manually, works for all cases */
       int x;
       int y;
       for(x = 0; x < ImageToMask->width(); x++) {
          for(y = 0; y < ImageToMask->height(); y++) {
             //printf("<%x>",Mask.pixel(x,y));
             if ((Mask.pixel(x,y) & 0x00FFFFFF) != 0) {
                ImageToMask->setPixel(x, y, 0x0);
             }
          }
       }
   }
}
/*****************************************************************************/
/*  End  Method : AddAlphaMask                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : LoadImage                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates an image from the contents of a file               */
/*                                                                           */
/*  Type        : Public Slot (Needs BlockingQueuedConnection)               */
/*                                                                           */
/*  Input Para  : Id        pointer where to place the Id of the new image   */
/*                FileName  Name of Imagefile to load, opens filedialog      */
/*                          if FileName is NULL                              */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::LoadImage(int *Id, const char *FileName)
{
   /* Method data declaration      */
    QString TheFileName;

   /* Method code declaration      */

   /* Open Dialog if no name was given */
   if (FileName == NULL) {
      TheFileName = QFileDialog::getOpenFileName(this, tr("Open Image File"),
            "", "Image files (*.png *.gif *.jpg *.jpeg *.bmp);;All Files (*.*)");
   } else {
      TheFileName = FileName;
   }

   /* Create Image if Name is valid */
   if (!TheFileName.isEmpty()) {
      QImage *NewImage = new QImage(TheFileName);
	   if(NewImage->isNull()) {
	      // Problem with file, image was not created

	      // Try to find some information for the user
	      if (QDir::isRelativePath(TheFileName)) {
	         TheFileName = QDir::currentPath() + QDir::separator() + TheFileName;
	      }
	      std::cout << "Could not load imagefile <" << TheFileName.toStdString() << ">!\n";


		   delete NewImage;
		   *Id =  -1;
		   return;
	   }
	   /* Convert to format with alpha channel if alpha is missing */
      if (!NewImage->hasAlphaChannel()) {
         QImage *t = new QImage(NewImage->convertToFormat(QImage::Format_ARGB32));
         delete NewImage;
         NewImage = t;
      }

      /* Place new image in image array and return its Id */
      ImageBuffers.push_back(NewImage);
      *Id = ImageBuffers.size()-1;
   } else {
      *Id = -1;
   }
}
/*****************************************************************************/
/*  End  Method : LoadImage                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : SaveImage                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Saves an image into a file                                 */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : ImageId   Id of Image to save                              */
/*                FileName  Name of Imagefile to save, opens filedialog      */
/*                          if FileName is NULL                              */
/*                Format    Image format to save (jpg, png, ...) See Qt      */
/*                          QImage documentation for supported formats       */
/*                Quality   Quality for compressed formats. See Qt           */
/*                          QImage documentation for supported formats       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::SaveImage(int ImageId, const char *FileName, const char *Format, int Quality)
{
   /* Method data declaration      */
   QString TheFileName;

   /* Method code declaration      */

   /* Open Dialog if no name was given */
   if (FileName == NULL) {
      TheFileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
            "", "Image files (*.png *.gif *.jpg *.jpeg *.bmp);;All Files (*.*)");
   } else {
      TheFileName = FileName;
   }

   /* Save Image if Name is valid */
   if (!TheFileName.isEmpty()) {

	   /* Find image to svae */
      QImage *ImageToSave = Image;

	   if ((ImageId >= 0) && (ImageId < (int)ImageBuffers.size())) {

	      /* Check for valid Image */
	      if (ImageBuffers[ImageId] == NULL) {
		      std::cout << "Illegal image ID in SaveImage() (Image was destroyed)\n";
				return;
			}
		   ImageToSave = ImageBuffers[ImageId];
	   }
	   /* save the image */
	   ImageToSave->save(TheFileName, Format, Quality);
   }
}
/*****************************************************************************/
/*  End  Method : SaveImage                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : Coordinate Transformations                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Transform the coordinatesystem for the active image, all   */
/*                future drawing operations on this image will use the       */
/*                transformed coordinate system                              */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : Angle       Rotation of coordinate system in radiant       */
/*                dx/dy       Translation of coordinate system               */
/*                Scalingx/y  Scaling of coordinate system                   */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/

/* Rotate the coordinate system */
void MainGui::Rotate(float Angle)
{
   CurrentPainter->rotate(Angle);
}

/* Translate the coordinate system */
void MainGui::Translate(float dx, float dy)
{
   CurrentPainter->translate (dx, dy);
}

/* Scale the coordinate system */
void MainGui::Scale(float Scalingx, float Scalingy)
{
   CurrentPainter->scale(Scalingx, Scalingy);
}

/* Restore the original coordinate system */
void MainGui::ResetTransformations(void)
{
   CurrentPainter->resetTransform ();
}
/*****************************************************************************/
/*  End  Method : Coordinate Transformations                                 */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : DestroyImage                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Releases all resources for the given image                 */
/*                Will not work on main image (Screen) and currently         */
/*                Ids of destroyed images will become invalid                */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : ImageId   Id of Image to destroy (Selected image and       */
/*                          main image will be ignored)                      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.12.2009  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::DestroyImage(int ImageId)
{
   /* Method data declaration      */

   /* Method code declaration      */

   /* Check for valid Id */
   if ((ImageId >= 0) && (ImageId < (int)ImageBuffers.size())) {
      if (ImageBuffers[ImageId] == NULL) {
         std::cout << "Illegal image ID in DestroyImage() (Image was already destroyed)\n";
		   return;
      }
      if (CurrentImage == ImageId) {
         std::cout << "Illegal image ID in DestroyImage(): Cannot destroy selected (Active) image\n";
         return;
      }

      /* free resources of selected image */
      delete ImageBuffers[ImageId];

      /* mark as destroyed */
      ImageBuffers[ImageId] = NULL;
      // Do not remove entry from Imagebuffers,
      // otherwise handles will point to wrong
      // entries (Handles are used as indices)
   }
}
/*****************************************************************************/
/*  End  Method : DestroyImage                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : Sound functions                                            */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Methodes to play and stop playing sound                    */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : FileName  Name of soundfile to play                        */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.03.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/

/* Plays the given sound once (returns immediately)*/
void MainGui::PlaySound(QString FileName)
{
   QSound::play(FileName);
}

/* Plays the given sound continuous (returns immediately)*/
void MainGui::PlaySoundContinuous(QString FileName)
{
   if (MyBackgroundSong != 0) {
      delete MyBackgroundSong;
   }
   MyBackgroundSong = new QSound(FileName);
   MyBackgroundSong->setLoops(-1);
   MyBackgroundSong->play();
}

/* Stops continuous playing */
void MainGui::StopContinuousSound(void)
{
   if (MyBackgroundSong != 0) {
      MyBackgroundSong->stop(); ;
   }
}

/* Restarts continuous playing of before stopped sound */
void MainGui::StartPlaySoundContinuous(void)
{
   if (MyBackgroundSong != 0) {
      MyBackgroundSong->setLoops(-1);
      MyBackgroundSong->play();
   }
}
/*****************************************************************************/
/*  End  Method : Sound functions                                            */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : StartTimer                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Starts a timer which periodically calls TimerTick          */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : IntervalTime  Period of timer                              */
/*                Parameter     Parameter to pass to timer handler           */
/*                Handler       Function (Handler) to call periodically      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.03.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/

/* Will be called from Qt each timer tick and calls attached handler */
void MainGui::TimerTick(void)
{
   /* Method data declaration      */

   /* Method code declaration      */

   /* Call handler if available */
   if (TimerTickHandler != NULL) {
      TimerTickHandler(TimerTickParameter);
   }
}

void MainGui::StartTimer(int IntervalTime, void *Parameter, void *Handler)
{
   /* Method data declaration      */

   /* Method code declaration      */

   /* Make sure Timer is not running any more */
   MyTimer->stop();

   /* Set new handler and its parameter */
   TimerTickHandler = (void(*)(void *)) Handler;
   TimerTickParameter = Parameter;

   /* Start Timer with given Period */
   MyTimer->start(IntervalTime);
}
/*****************************************************************************/
/*  End  Method : StartTimer                                                 */
/*****************************************************************************/

/*****************************************************************************/
/*  Method      : StopTimer                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Stops the timer, handler will no longer be called          */
/*                                                                           */
/*  Type        : Public Slot                                                */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 15.03.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void MainGui::StopTimer(void)
{
   /* Method data declaration      */

   /* Method code declaration      */

   /* Stop the Timer and clear handler and parameter */
   MyTimer->stop();
   TimerTickHandler = NULL;
   TimerTickParameter = NULL;
}
/*****************************************************************************/
/*  End  Method : StopTimer                                                  */
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
void MainGui::OpenFileDialog(bool *Success, int Mode, const char *Title, const char *Directory, const char *Filter, const char *PreferredFilter, int Options, char *FilenameBuffer, int MaxNameLength, char *SelectedFilter, int MaxFilterLength)
{
	   /* Method data declaration      */
	    QString TheFileName;
	    QFileDialog::Options MyOptions(Options);
	    QString QSSelectedFilter(SelectedFilter);

	   /* Method code declaration      */
        (void)PreferredFilter;  /*Silence unused param warning */


	   /* Open selected kind of dialog */
	   if (Mode == FDM_FILE_SAVE) {
	      if (MyOptions == FDO_USE_DEFAULT) {
	         MyOptions = 0;
	      }
	      TheFileName = QFileDialog::getSaveFileName(this, QString(Title), QString(Directory), QString(Filter), &QSSelectedFilter, MyOptions);
	   } else if (Mode == FDM_DIRECTORY) {
	      if (MyOptions == FDO_USE_DEFAULT) {
	         MyOptions = QFileDialog::ShowDirsOnly;
	      }
	      TheFileName = QFileDialog::getExistingDirectory(this, Title, Directory, MyOptions);
	   } else {
	      if (MyOptions == FDO_USE_DEFAULT) {
	         MyOptions = 0;
	      }
	      TheFileName = QFileDialog::getOpenFileName(this, QString(Title), QString(Directory), QString(Filter), &QSSelectedFilter, MyOptions);
	   }

	   if (SelectedFilter != NULL) {
	      /* Copy selected filter receiver array */
	      strncpy(SelectedFilter, QSSelectedFilter.toLatin1().constData(), MaxFilterLength);
	   }

	   /* return false if no file was selected */
	   if (TheFileName.isNull()) {
	      *Success = false;
	   } else {

           /* Copy resulting path to receiver array */
           strncpy(FilenameBuffer, TheFileName.toLatin1().constData(), MaxNameLength);

          *Success = true;
	   }

}
/*****************************************************************************/
/*  End  Method : OpenFileDialog                                             */
/*****************************************************************************/


/*****************************************************************************/
/*  End Class   : MainGui                                                    */
/*****************************************************************************/







