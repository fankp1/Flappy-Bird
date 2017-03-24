/* general control */

/*****************************************************************************/
/*  Module     : WindowFunctions                                Version 1.0  */
/*****************************************************************************/
/*                                                                           */
/*  Function   : A collection of drawing and keyboard functions              */
/*               Serves as interface to the Qt/C++  library                  */
/*                                                                           */
/*                                                                           */
/*  Procedures : InitGraphic            Opens the graphic window             */
/*               CloseGraphic           Closes the graphic window            */
/*               SetAutoUpdate          Enables automatic update             */
/*               ClearWindow            Clears the window                    */
/*               SetDrawMode            Set hoe to draw (Transparent...)     */
/*               DrawFilledCircle       draws a filled circle                */
/*               DrawEmptyCircle        draws an empty circle                */
/*               DrawFilledRectangle    Draws a filled rectangle             */
/*               DrawEmptyRectangle     Draws an empty rectangle             */
/*               DrawLine               Draws a line                         */
/*               DrawPixel              Draws a single pixel/point           */
/*               GetPixel               Gets Color of a pixel/point          */
/*               DrawText               Draws a text                         */
/*               SelectFont             Selects the font for text to draw    */
/*               GetTextDimensions      Returns the space needed for text    */
/*               GetKeyPress            Waits for a pressed key and returns it*/
/*               IsKeyPressReady        Checks if key has been pressed       */
/*               GetKeyEvent            Waits for a released or pressed      */
/*               IsKeyEventReady        Checks if key has been pressed/released*/
/*               GetMouseState          Get position and buttonstate of mouse*/
/*               GetMouseEvent          Returns position of last mouseevent  */
/*               WaitMs                 Waits the given time in ms           */
/*                                                                           */
/*  Advanced functions:                                                      */
/*                                                                           */
/*               CreateSubImage         Creates an copy of an image          */
/*               CreateImage            Creates an Imagebuffer               */
/*               LoadImage              Creates an Imagebuffer and Loads an Image*/
/*               SaveImage              Saves an Imagebuffer to a file       */
/*               DestroyImage           Releases an Imagebuffer              */
/*               SetEditedImage         Selects buffer for future drawings   */
/*               DrawImage              Draws the given imagebuffer          */
/*               CopyToImage            Copies part of an image to another   */
/*               GetImageSize           Delivers size of image               */
/*               Rotate                 Rotates future drawings              */
/*               Translate              Translates future drawings           */
/*               Scale                  Scales future Drawings               */
/*               ResetTransformations   Future drawings will be direct       */
/*               GetPixelDataPointer    Gets pointer to Bitmap of an image   */
/*               AddAlphaMask           Creates transparency mask for image  */
/*                                                                           */
/*               PlaySoundOnce          Plays a sound one time               */
/*               PlaySoundContinuous    Plays a sound continuous             */
/*               StopContinuousSound    Stops continuous soundplaying        */
/*               StartContinuousSound   Restarts continuous soundplaying     */
/*                                                                           */
/*  Very Advanced functions:                                                 */
/*                                                                           */
/*               SetQtOptions           Directly sets some Qt Flags          */
/*                                                                           */
/*               StartTimer             Starts a periodically functioncall   */
/*               StopTimer              Stops the periodically functioncall  */
/*                                                                           */
/*               CreateThread           Multithreading support               */
/*               Lock(void)             Multithreading synchronisation       */
/*               Unlock(void)           Multithreading synchronisation       */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*  Author     : I. Oesch                                                    */
/*                                                                           */
/*  History    : 02.04.2002  IO Created                                      */
/*               28.03.2004  IO Added cpp compatibility in header            */
/*               29.03.2004  IO Changed char to unsigned char in ColorType   */
/*               06.10.2010  IO Converted to QT as graphics library          */
/*                                                                           */
/*  File       : window.c                                                    */
/*                                                                           */
/*****************************************************************************/
/* MagicSoft                                                                 */
/*****************************************************************************/

/* imports */
#include "window.h"
#include "Worker.h"
#include "error.h"

/* module constant declaration */
#define _KEY_UPARROW           0x0048
#define _KEY_DOWNARROW         0x0050
#define _KEY_LEFTARROW         0x004B
#define _KEY_RIGHTARROW        0x004D
#define _KEY_F1                0x003B
#define _KEY_F2                0x003C
#define _KEY_F3                0x003D
#define _KEY_F4                0x003E
#define _KEY_F5                0x003F
#define _KEY_F6                0x0040
#define _KEY_F7                0x0041
#define _KEY_F8                0x0042
#define _KEY_F9                0x0043
#define _KEY_F10               0x0044
#define _KEY_F11               0x0085
#define _KEY_F12               0x0086


#define SOLID_FILL 0xFF
#define NO_FILL 0x00
#define SOLID_LINE 0xFF

/* Some helpers to create RGB / RGBA colors */
#define COLOR(R, G, B) (0xFF000000 | ((((R)<<16)&0x00FF0000) | (((G)<<8)&0x0000FF00) | (((B)<<0)&0x000000FF)))
#define ALPHACOLOR(R, G, B, A) ((((A) << 24)&0xFF000000) | ((((R)<<16)&0x00FF0000) | (((G)<<8)&0x0000FF00) | (((B)<<0)&0x000000FF)))

/* module type declaration */

/* module data declaration */
static int MaxAllowedHandle = -1;

/* module procedure declaration */

/*****************************************************************************/
/*  Procedure   : DrawFilledRectangle                                        */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a filled rectangle, whose lower left corner, width,  */
/*                height and RGB color is provided by the caller             */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : x, y           Coordinates of lower left corner            */
/*                Width, Height  Dimensions of rectangle                     */
/*                Color          RGB (Red Green Blue) Color of rectangle     */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawFilledRectangle (int x, int y, int Width, int Height, ColorType Color, int LineWidth)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the rectangle to be filled with */
   Worker::GetTheWorker()->setfillstyle(SOLID_FILL, ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the color for the rectangle to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the width of the rectangle line */
   Worker::GetTheWorker()->setlinestyle(SOLID_LINE, 0, LineWidth);


   /* Draw the rectangle */
   Worker::GetTheWorker()->bar(x , y, Width, Height);
}
/*****************************************************************************/
/*  End         : DrawFilledRectangle                                        */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DrawEmptyRectangle                                         */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws an nonfilled rectangle, whose lower left corner,     */
/*                width, height and RGB frame color is provided by the caller*/
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : x, y           Coordinates of lower left corner            */
/*                Width, Height  Dimensions of rectangle                     */
/*                Color          RGB (Red Green Blue) Color of rectangle     */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawEmptyRectangle (int x, int y, int Width, int Height, ColorType Color, int LineWidth)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the rectangle to be filled with */
   Worker::GetTheWorker()->setfillstyle(NO_FILL, ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the color for the rectangle to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the width of the rectangle line */
   Worker::GetTheWorker()->setlinestyle(SOLID_LINE, 0, LineWidth);


   /* Draw the rectangle */
   Worker::GetTheWorker()->bar(x , y, Width, Height);

}
/*****************************************************************************/
/*  End         : DrawEmptyRectangle                                         */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DrawFilledCircle                                           */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a filled circle, whose lower left corner, width,     */
/*                height and RGB color is provided by the caller             */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : x, y           Coordinates of lower left corner            */
/*                Width, Height  Dimensions of bounding rectangle            */
/*                Color          RGB (Red Green Blue) Color of circle        */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawFilledCircle(int x, int y, int Width, int Height, ColorType Color, int LineWidth)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the circle to be filled with */
   Worker::GetTheWorker()->setfillstyle(SOLID_FILL, ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the color for the circle to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the width of the circleline */
   Worker::GetTheWorker()->setlinestyle(SOLID_LINE, 0, LineWidth);

   /* Draw the circle */
   Worker::GetTheWorker()->ellipse(x , y, Width, Height);
}
/*****************************************************************************/
/*  End         : DrawFilledCircle                                           */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DrawEmptyCircle                                            */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws an empty circle, whose lower left corner, width,     */
/*                height and RGB color is provided by the caller             */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : x, y           Coordinates of lower left corner            */
/*                Width, Height  Dimensions of bounding rectangle            */
/*                Color          RGB (Red Green Blue) Color of circle        */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 25.03.2008  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawEmptyCircle(int x, int y, int Width, int Height, ColorType Color, int LineWidth)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the rectangle to be filled with */
   Worker::GetTheWorker()->setfillstyle(NO_FILL, ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the color for the rectangle to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the width of the rectangle line */
   Worker::GetTheWorker()->setlinestyle(SOLID_LINE, 0, LineWidth);

   /* Draw the circle */
   Worker::GetTheWorker()->ellipse(x, y, Width, Height);
}
/*****************************************************************************/
/*  End         : DrawEmptyCircle                                            */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DrawLine                                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a simple line from x1, y1 to x2, y2 and in the       */
/*                RGB color provided by the caller                           */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : x1, y1         Coordinates of startpoint of the line       */
/*                x2, y2         Coordinates of endpoint of the line         */
/*                Color          RGB (Red Green Blue) Color of the line      */
/*                Width          Width of line                               */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawLine (int x1, int y1, int x2, int y2, ColorType Color, int LineWidth)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the line to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the width of the lines */
   Worker::GetTheWorker()->setlinestyle(SOLID_LINE, 0, LineWidth);

   /* Draw the line */
   Worker::GetTheWorker()->line(x1 , y1, x2, y2);
}
/*****************************************************************************/
/*  End         : DrawLine                                                   */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DrawPolyLine                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a (unclosed) polyline with the given line color      */
/*                and line width                                             */
/*                For a closed, unfilled polygon, first and last point must  */
/*                be the same                                                */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Edges:         Edge points of polyline {x1, y1, x2, y2,...}*/
/*                NumberOfEdges: Number of x/y pairs in Edges                */
/*                Color          RGB (Red Green Blue) Color of the line      */
/*                Width          Width of line                               */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 11.04.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawPolyLine (int *Edges, int NumberOfEdges, ColorType Color, int LineWidth)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the line to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the width of the lines */
   Worker::GetTheWorker()->setlinestyle(SOLID_LINE, 0, LineWidth);

   /* Draw the polyline */
   Worker::GetTheWorker()->polyline(Edges, NumberOfEdges);
}
/*****************************************************************************/
/*  End         : DrawPolyLine                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DrawFilledPolygon                                          */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a (closed and filled) polygon with the given fill    */
/*                color, line color and line width                           */
/*                The first and last edge are automatically connected        */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Edges:         Edge points of polyline {x1, y1, x2, y2,...}*/
/*                NumberOfEdges: Number of x/y pairs in Edges                */
/*                Color          RGB (Red Green Blue) Color of the polygon   */
/*                Width          Width of line                               */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 11.04.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawFilledPolygon (int *Edges, int NumberOfEdges, ColorType Color, int LineWidth)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the polygon to be filled with */
   Worker::GetTheWorker()->setfillstyle(SOLID_FILL, ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the color for the line to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the width of the lines */
   Worker::GetTheWorker()->setlinestyle(SOLID_LINE, 0, LineWidth);

   /* Draw the polygon */
   Worker::GetTheWorker()->polygon(Edges, NumberOfEdges);
}

/*****************************************************************************/
/*  End         : DrawFilledPolygon                                          */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DrawPixel                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a Pixel at the given Position and in the             */
/*                RGB color provided by the caller                           */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : x1, y1         Coordinates of Pixel to set                 */
/*                Color          RGB (Red Green Blue) Color of the Pixel     */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 14.03.2005  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawPixel (int x, int y, ColorType Color)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the line to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Set the width of the lines */
   Worker::GetTheWorker()->setlinestyle(SOLID_LINE, 0, 1);

   /* Draw the Pixel */
   Worker::GetTheWorker()->putpixel(x, y);
}
/*****************************************************************************/
/*  End         : DrawPixel                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetPixel                                                   */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the RGB-Value of the given Pixel                   */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : x1, y1         Coordinates of Pixel to get                 */
/*                                                                           */
/*  Output Para : ARGB Vlaue of the selected Pixel                           */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 13.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
ColorType GetPixel (int x, int y)
{
   /* procedure data */
   ColorType Color;
   unsigned long RGBColor;

   /* procedure code */

   /* Get the Pixelvalue */
   RGBColor = Worker::GetTheWorker()->GetPixel(x, y);
   Color.Alpha = (RGBColor >> 24) & 0xFF;
   Color.Red   = (RGBColor >> 16) & 0xFF;
   Color.Green = (RGBColor >>  8) & 0xFF;
   Color.Blue  = (RGBColor >>  0) & 0xFF;
   return Color;
}
/*****************************************************************************/
/*  End         : GetPixel                                                   */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DrawTextXY                                                 */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws a simple Text at given position and in the           */
/*                RGB color provided by the caller                           */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : x, y           Position to place the Text                  */
/*                Color          RGB (Red Green Blue) Color of the Text      */
/*                Text           String to draw                              */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawTextXY (int x, int y, ColorType Color, const char *Text)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the text to be drawn with */
   Worker::GetTheWorker()->setcolor(ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));

   /* Draw the text */
   Worker::GetTheWorker()->outtextxy(x, y, Text);
}
/*****************************************************************************/
/*  End         : DrawTextXY                                                 */
/*****************************************************************************/

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
void SelectFont(const char *FontName, int Points, enum FontStyle Style)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the text to be drawn with */
   Worker::GetTheWorker()->SelectFont(FontName, Points, Style);

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
TextDimensionType GetTextDimensions (const char *Text)
{
   /* procedure data */

   /* procedure code */

   /* Set the color for the text to be drawn with */
   return Worker::GetTheWorker()->GetTextDimensions(Text);

}
/*****************************************************************************/
/*  End         : GetTextDimensions                                          */
/*****************************************************************************/


/*****************************************************************************/
/*  Procedure   : SetDrawMode                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets mode of future draws                                  */
/*                  Selects how next drawings will be drawn on existing      */
/*                  drawing. (Drawing over, mixing, blending, ...            */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Mode:     How future drawings are merged into image        */
/*                          May be one of the predefined modes or a standard */
/*                          QT Mode                                          */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 11.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void SetDrawMode(int Mode)
{
   /* procedure data */

   /* procedure code */

   /* Set the mode for future draw operations */
   Worker::GetTheWorker()->SetDrawMode(Mode);

}
/*****************************************************************************/
/*  End         : SetDrawMode                                                */
/*****************************************************************************/


/*****************************************************************************/
/*  Procedure   : CreateImage                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates an Image with the given Size, can be used to draw  */
/*                into or to be drawn onto another Image                     */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Width, Height  Size of Imagebuffer to be created           */
/*                                                                           */
/*  Output Para : Handle to reference Image in Future (-1 on failure)        */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int CreateImage(int Width, int Height)
{
   /* procedure data */

   /* procedure code */

   /* Create a new Image */
   MaxAllowedHandle = Worker::GetTheWorker()->CreateImage(Width, Height);
   return MaxAllowedHandle;
}
/*****************************************************************************/
/*  End         : CreateImage                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : CreateSubImage                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates an Image with the given Size, is filled with       */
/*                contents of given image at given position                  */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : SrcID  Handle of image to take content from                */
/*                x, y   Top left corner of source area to copy from         */
/*                Width, Height  Size of Imagebuffer to create               */
/*                                                                           */
/*  Output Para : Handle to reference Image in Future (-1 on failure)        */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int CreateSubImage(int SrcID, int x, int y, int Width, int Height)
{
   /* procedure data */

   /* procedure code */

   /* Check for valid handle */
   if ((SrcID < -1) || (SrcID > MaxAllowedHandle)) {
      CODING_ERROR("Error in CreateSubImage, invalid image handle");
   }

   /* Create a new Image */
   MaxAllowedHandle = Worker::GetTheWorker()->CreateImage(SrcID, x, y, Width, Height);
   return MaxAllowedHandle;
}
/*****************************************************************************/
/*  End         : CreateSubImage                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : LoadImage                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Loads an Image from a File (gif, bmp, png, jpeg, ...)      */
/*                can be used to draw into or to be drawn onto another Image */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Name of file to load, if NULL a file-dialog will be opened */
/*                                                                           */
/*  Output Para : Handle to reference Image in Future (-1 on failure)        */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int LoadImage(const char *FileName)
{
   /* procedure data */

   /* procedure code */

   /* Create a new Image */
   MaxAllowedHandle = Worker::GetTheWorker()->LoadImage(FileName);
   return MaxAllowedHandle;
}
/*****************************************************************************/
/*  End         : LoadImage                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : SaveImage                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Saves an Image to a File (gif, bmp, png, jpeg, ...)        */
/*                (For mor information, see Qt QImage documentation)         */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Id     Handle of image to svae content from                */
/*                Name   of file to save, if NULL a file-dialog will be opened */
/*                Format of file to save (bmp, png, jpg,... whatever is      */
/*                       supported by Qt (Gif is optional in Qt)             */
/*                Quality Quality of compressed formats                      */
/*                                                                           */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.10.2011  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void SaveImage(int Id, const char *FileName, const char *Format, int Quality)
{
	   /* procedure data */

	   /* procedure code */

	   /* Save a Image */
	   Worker::GetTheWorker()->SaveImage(Id, FileName, Format, Quality);

}
/*****************************************************************************/
/*  End         : SaveImage                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : DestroyImage                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Releases all occupied resources for an image               */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : ImageId Handle of picture to destroy                       */
/*                                                                           */
/*  Output Para : none                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DestroyImage(int ImageId)
{
   /* procedure data */

   /* procedure code */

   /* Check for valid handle */
   if ((ImageId < -1) || (ImageId > MaxAllowedHandle)) {
      CODING_ERROR("Error in DestroyImage, invalid image handle");
   }

   /* destroys the given image, frees resources */
   Worker::GetTheWorker()->DestroyImage(ImageId);
}
/*****************************************************************************/
/*  End         : DestroyImage                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : SetQtOptions                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Sets directly some QPainter options in Qt                  */
/*                See Qt QPainter documentation for more information         */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Option  QPainter option to change                          */
/*                Value   Value for the selected option                      */
/*                                                                           */
/*  Output Para : none                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.04.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void SetQtOptions(enum QTOptions Option, long int Value)
{
   Worker::GetTheWorker()->SetQtOptions(Option, Value);
}
/*****************************************************************************/
/*  End         : SetQtOptions                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : SetEditedImage                                             */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Selects the Image, on which all future draw operations will*/
/*                take place                                                 */
/*                Use ID_WINDOW to select the visible screen (Default)       */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : ImageId Handle of image to use                             */
/*                                                                           */
/*  Output Para : none                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void SetEditedImage(int ImageId)
{
   /* procedure data */

   /* procedure code */

   /* Check for valid handle */
   if ((ImageId < -1) || (ImageId > MaxAllowedHandle)) {
      CODING_ERROR("Error in SetEditedImage, invalid image handle");
   }

   /* Create a new Image */
   Worker::GetTheWorker()->SetEditedImage(ImageId);
}
/*****************************************************************************/
/*  End         : SetEditedImage                                             */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetImageSize                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the size (Height and Width) of the given image     */
/*                Use ID_WINDOW to select the visible screen (Default)       */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : ImageId Handle of image to use                             */
/*                Width, Height pointer to place to store height resp. width */
/*                                                                           */
/*  Output Para : none                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void GetImageSize(int ImageId, int *Width, int *Height)
{
   /* procedure data */

   /* procedure code */

   /* Check for valid handle */
   if ((ImageId < -1) || (ImageId > MaxAllowedHandle)) {
      CODING_ERROR("Error in GetImageSize, invalid image handle");
   }

   /* Create a new Image */
   Worker::GetTheWorker()->GetImageSize(ImageId, Width, Height);
}
/*****************************************************************************/
/*  End         : GetImageSize                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetPixelDataPointer                                        */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns a pointer to the Pixelarray of the selected image  */
/*                Use ID_WINDOW to select the visible screen (Default)       */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : ImageId Handle of image to use                             */
/*                                                                           */
/*  Output Para : Pointer to the Image data                                  */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
ColorType *GetPixelDataPointer(int ImageId, int *Length)
{
   /* procedure data */

   /* procedure code */

   /* Check for valid handle */
   if ((ImageId < -1) || (ImageId > MaxAllowedHandle)) {
      CODING_ERROR("Error in GetImageSize, invalid image handle");
   }

   /* Create a new Image */
   return Worker::GetTheWorker()->GetPixelDataPointer(ImageId, Length);
}
/*****************************************************************************/
/*  End         : GetImageSize                                               */
/*****************************************************************************/


/*****************************************************************************/
/*  Procedure   : DrawImage                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Draws the given Image at the given position onto the       */
/*                current image                                              */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : ImageId Handle of image to use                             */
/*                x, y    Position to draw image at                          */
/*                                                                           */
/*  Output Para : none                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void DrawImage(int ImageId, int x, int y)
{
   /* procedure data */

   /* procedure code */

   /* Check for valid handle */
   if ((ImageId < -1) || (ImageId > MaxAllowedHandle)) {
      CODING_ERROR("Error in DrawImage, invalid image handle");
   }

   /* Create a new Image */
   Worker::GetTheWorker()->DrawImage(ImageId, x, y);
}
/*****************************************************************************/
/*  End         : DrawImage                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : CopyToImage                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Copies the given part (by x,y,width and Height) from the   */
/*                current image into the given Image                         */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : ImageId Handle of image to use                             */
/*                x, y    Position to draw image at                          */
/*                                                                           */
/*  Output Para : none                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void CopyToImage(int x, int y, int Width, int Height, int ImageId)
{
   /* procedure data */

   /* procedure code */

   /* Check for valid handle */
   if ((ImageId < -1) || (ImageId > MaxAllowedHandle)) {
      CODING_ERROR("Error in CopyToImage, invalid image handle");
   }

   /* Create a new Image */
   Worker::GetTheWorker()->CopyToImage(x, y, Width, Height, ImageId);
}
/*****************************************************************************/
/*  End         : CopyToImage                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : AddAlphaMask                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates an alphamask for the given image, uses color for   */
/*                masking pixels, or an heuristic automatical mode to mask   */
/*                image, masking pixels from border till first nonborder-    */
/*                pixel is found                                             */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : ImageId Handle of image to mask                            */
/*                Mode    Color in, color out or auto                        */
/*                Color   Color used for mask in color in/out, not used for  */
/*                        auto mode                                          */
/*                                                                           */
/*  Output Para : none                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void AddAlphaMask(int ImageId, int Mode, ColorType Color)
{
   /* procedure data */

   /* procedure code */

   /* Check for valid handle */
   if ((ImageId < -1) || (ImageId > MaxAllowedHandle)) {
      CODING_ERROR("Error in AddAlphaMask, invalid image handle");
   }

   /* Create a new Image */
   Worker::GetTheWorker()->AddAlphaMask(ImageId, Mode, ALPHACOLOR(Color.Red, Color.Green, Color.Blue, Color.Alpha));
}
/*****************************************************************************/
/*  End         : AddAlphaMask                                               */
/*****************************************************************************/


/*****************************************************************************/
/*  Procedure   : Transform drawings (rotate, scale, translate)              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : All future drawing operations will be transformed by the   */
/*                selected transforms. Use ResetTransformations to disable   */
/*                all transformations.                                       */
/*                                                                           */
/*                (Attention, in fact the coordinatesystem of the            */
/*                 currently selected image is transformed, and all future   */
/*                 drawing commands are related to this transformed          */
/*                 coordinate system)                                        */
/*                                                                           */
/*                If a sequence of transformations is applied, all           */
/*                transformations will be applied in the given sequence      */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Parameter for transformation                               */
/*                                                                           */
/*  Output Para : none                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/

/* all future drawing operations will be rotated by the given Angle */
void Rotate(float Angle)
{
   /* procedure data */

   /* procedure code */

   /* Create a new Image */
   Worker::GetTheWorker()->Rotate(Angle);
}

/* all future drawing operations will be translated by the given offset */
void Translate(float dx, float dy)
{
   /* procedure data */

   /* procedure code */

   /* Create a new Image */
   Worker::GetTheWorker()->Translate(dx, dy);
}

/* all future drawing operations will be scaled by the given offset */
void Scale(float Scalingx, float Scalingy)
{
   /* procedure data */

   /* procedure code */

   /* Create a new Image */
   Worker::GetTheWorker()->Scale(Scalingx, Scalingy);
}

/* all future drawing operations will be drwan dirctly, all transformations are disabled */
void ResetTransformations(void)
{
   /* procedure data */

   /* procedure code */

   /* Create a new Image */
   Worker::GetTheWorker()->ResetTransformations();
}
/*****************************************************************************/
/*  End         : Transform drawings (rotate, scale, translate)              */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : SetAutoUpdate                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Enables or disables autoupdating. When autoupdating is     */
/*                enabled, screen will be refreshed after each drawing       */
/*                command. May slow down system if a lot of drawings are     */
/*                made                                                       */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Mode  1 = Enable, 0 = Disable autoupdating                 */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 06.01.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void SetAutoUpdate (int Mode){
   Worker::GetTheWorker()->SetAutoUpdate(Mode);
};
/*****************************************************************************/
/*  End         : SetAutoUpdate                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : InitGraphic                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Opens a window and initializes the windowspacket           */
/*                Must be called before any other function of this modul is  */
/*                called                                                     */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Width, Height  Size of the Window (-1 for fullscreen)      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void InitGraphic (int Width, int Height)
{
   /* procedure data */

   /* procedure code */

   /* Initialize the window */
   Worker::GetTheWorker()->initwindow(Width, Height);
   Worker::GetTheWorker()->SetAutoUpdate(1);
   Worker::GetTheWorker()->SetBackgroundColor(0x00000000);  // Transparent
    //settextstyle(TRIPLEX_FONT, HORIZ_DIR, 0);
    //setusercharsize(1, 3, 1, 3);
}

/*****************************************************************************/
/*  End         : InitGraphic                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : CloseGraphic                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Closes the window and frees all resources                  */
/*                Must be called before exiting any program using this lib   */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void CloseGraphic (void)
{
   /* procedure data */

   /* procedure code */

   /* closes the window */
   Worker::GetTheWorker()->closegraph( );
}

/*****************************************************************************/
/*  End         : CloseGraphic                                               */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetKeyPress                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the code of the oldest unprocessed keypress, Waits */
/*                for a pressed key in none is availlable                    */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : Code of Key pressesd                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int GetKeyPress (void)
{
   /* procedure data */

   int Code;
   /* procedure code */

   /* propagate call */
   Code = Worker::GetTheWorker()->getch();

   while ((Code == 0)||((Code & (W_KEY_MODIFIER | W_KEY_RELEASED)) != 0)) {
      if (Code == 0) {
         /* if no key was availlable, wait a bit... */
         Worker::GetTheWorker()->delay(50);
      }
      Code = Worker::GetTheWorker()->getch();

   }
   return Code;
}

/*****************************************************************************/
/*  End         : GetKeyPress                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetKeyEvent                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the code of the oldest unprocessed key press or    */
/*                key release event, Waits for a key event if none is        */
/*                availlable                                                 */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : Code of Key pressed/released                               */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int GetKeyEvent (void)
{
   /* procedure data */

   int Code;
   /* procedure code */

   /* propagate call */
   Code = Worker::GetTheWorker()->getch();

   while (Code == 0) {
      Worker::GetTheWorker()->delay(50);
      Code = Worker::GetTheWorker()->getch();

   }
   return Code;
}

/*****************************************************************************/
/*  End         : GetKeyEvent                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : IsKeyPressReady                                            */
/*****************************************************************************/
/*                                                                           */
/*  Function    : returns true is key has been pressed (if a unprocessed     */
/*                keypress is availlable), otherwise false.                  */
/*                Does not wait for key                                      */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : true if key has been pressed, false otherwise              */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int IsKeyPressReady (void)
{
   /* procedure data */

   /* procedure code */

   /* propagate call */
    return Worker::GetTheWorker()->kbhit();
}

/*****************************************************************************/
/*  End         : IsKeyPressReady                                            */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : IsKeyEventReady                                            */
/*****************************************************************************/
/*                                                                           */
/*  Function    : returns true is key has been pressed or released (if a     */
/*                unprocessed key event is availlable), otherwise false.     */
/*                Does not wait for a key event                                      */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : true if key has been pressed/released, false otherwise     */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int IsKeyEventReady (void)
{
   /* procedure data */

   /* procedure code */

   /* propagate call */
    return Worker::GetTheWorker()->kbEvent();
}

/*****************************************************************************/
/*  End         : IsKeyEventReady                                            */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetMouseState                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the current state of the mouse (X/Y Position and   */
/*                current state of mouse buttons)                            */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : Structure containing mouse state information               */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
MouseInfoType GetMouseState (void)
{
   MouseInfoType Info;
   Worker::GetTheWorker()->GetMouseState(&Info);
   return Info;
}
/*****************************************************************************/
/*  End         : GetMouseState                                              */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : GetMouseEvent                                              */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Returns the oldes, not prozessed mouse event               */
/*                (Move, Clicks, Wheelchanges).                              */
/*                Each event gets its own event info, eg for each press,     */
/*                release, mousmove, wheelmove there will be a separate      */
/*                event                                                      */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : Structure containing mouse event information               */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
MouseInfoType GetMouseEvent (void)
{
   MouseInfoType Info;
   Worker::GetTheWorker()->GetMouseEvent(&Info);
   return Info;
}
/*****************************************************************************/
/*  End         : IsKeyPressReady                                            */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : WaitMs                                                     */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Waits the given time in ms                                 */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Time  Delaytime in ms                                      */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void WaitMs (int Time)
{
   /* procedure data */

   /* procedure code */

   /* propagate call */
    Worker::GetTheWorker()->delay(Time);
}

/*****************************************************************************/
/*  End         : WaitMs                                                     */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : ClearWindow                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Clears the screen                                          */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2002  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void ClearWindow (void)
{
   /* procedure data */

   /* procedure code */

   /* propagate call */
   Worker::GetTheWorker()->cleardevice();
}
/*****************************************************************************/
/*  End         : ClearWindow                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : ....Sound                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Some tool for soundhandling                                */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void PlaySoundOnce (const char *FileName)
{
   /* procedure data */

   /* procedure code */

   /* propagate call */
   Worker::GetTheWorker()->PlaySound(FileName);
}

void PlaySoundContinuous(const char *FileName)
{
   /* procedure data */

   /* procedure code */

   /* propagate call */
   Worker::GetTheWorker()->PlaySoundContinuous(FileName);
}

void StopContinuousSound (void)
{
   /* procedure data */

   /* procedure code */

   /* propagate call */
   Worker::GetTheWorker()->StopContinuousSound();
}

void StartContinuousSound (void)
{
   /* procedure data */

   /* procedure code */

   /* propagate call */
   Worker::GetTheWorker()->PlaySoundContinuous();
}

/*****************************************************************************/
/*  End         : ....Sound                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : ....Timer                                                  */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Some tool for Timer management, allows a function to be    */
/*                called periodically. If not protected by lock/unlock       */
/*                gfxmain and timerfunction must not use same resources      */
/*                together (Like graphiklibrary or networklibrary)           */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : IntervalTime  Intervall between functioncall               */
/*                Parameter     Parameter for handlerfunction, may be NULL   */
/*                Handler       Pointer to handlerfunction, this function    */
/*                              will be called periodically                  */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2010  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/

/* Starts the periodical functioncall mechanism */
void StartTimer(int IntervalTime, void *Parameter, void (*Handler)(void *))
{
   /* procedure data */

   /* procedure code */

   Worker::GetTheWorker()->StartTimer(IntervalTime, Parameter, Handler);
}

/* Stops the periodical functioncall mechanism */
void StopTimer(void)
{
   /* procedure data */

   /* procedure code */

   Worker::GetTheWorker()->StopTimer();
}
/*****************************************************************************/
/*  End         : ....Timer                                                  */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : Lock/Unlock                                                */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Allows protection from parallel access (Synchronization).  */
/*                Call Lock() of a part of Code accesses a resource shared   */
/*                by multiple threads. All other Threads calling Lock() will */
/*                be blocked until the first Thread calls unlock()           */
/*                (After Unlock, ot most one Thread can call Lock() without  */
/*                beeing blocked.                                            */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : None                                                       */
/*                                                                           */
/*  Output Para : None                                                       */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
void Lock(void)
{
   /* procedure data */

   /* procedure code */

   Worker::GetTheWorker()->Lock();
}

void Unlock(void)
{
   /* procedure data */

   /* procedure code */

   Worker::GetTheWorker()->Unlock();
}
/*****************************************************************************/
/*  End         : Lock/Unlock                                                */
/*****************************************************************************/

/*****************************************************************************/
/*  Procedure   : CreateThread                                               */
/*****************************************************************************/
/*                                                                           */
/*  Function    : Creates a new Thread, the new Thread will run after        */
/*                Creation until Function returns                            */
/*                                                                           */
/*  Type        : Global                                                     */
/*                                                                           */
/*  Input Para  : Parameter Parameter for Function                           */
/*                Function  Function which will be run as a new Thread       */
/*                          Thread will end if function returns              */
/*                                                                           */
/*  Output Para : 1 if Thread was created and started, 0 otherwise           */
/*                                                                           */
/*  Author      : I. Oesch                                                   */
/*                                                                           */
/*  History     : 02.04.2012  IO  Created                                    */
/*                                                                           */
/*****************************************************************************/
int CreateThread(void *Parameter, void (*Function)(void *))
{
   /* procedure data */

   /* procedure code */

   if(Worker::GetTheWorker()->CreateThread(Parameter, Function)) {
      return 1;
   } else {
      return 0;
   };
}
/*****************************************************************************/
/*  End         : CreateThread                                               */
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
int OpenFileDialog(int Mode, const char *Title, const char *Directory, const char *Filter, const char *PreferredFilter, int Options, char *FilenameBuffer, int MaxNameLength, char *SelectedFilter, int MaxFilterLength)
{
   /* Method data declaration      */

   /* Method code declaration      */


   /* Open the dialog */

   if(Worker::GetTheWorker()->OpenFileDialog(Mode, Title, Directory, Filter, PreferredFilter, Options, FilenameBuffer, MaxNameLength, SelectedFilter, MaxFilterLength)) {
      return 1;
   } else {
      return 0;
   };

}
/*****************************************************************************/
/*  End  Method : OpenFileDialog                                             */
/*****************************************************************************/


/*****************************************************************************/
/*  End Module  : WindowFunctions                                            */
/*****************************************************************************/














