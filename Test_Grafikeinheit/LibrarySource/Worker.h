/*
 * Worker.h
 *
 *  Created on: 02.09.2009
 *      Author: osi1
 */

#ifndef WORKER_H_
#define WORKER_H_
#include <QThread>
#include <QMutex>
#include <QCoreApplication>
#include <QPolygon>
#include "window.h"


//typedef struct MouseInfo MouseInfo;
struct MouseInfo;


class Worker : public QThread
{
    Q_OBJECT


public:
    void stop();
    static Worker* GetTheWorker();

    // Should be private (Singleton), but Qt does not like it this way
   // private:
   Worker();


private:
    static Worker *TheWorker;
    QMutex MyMutex;

signals:
   void Ssetfillstyle(int FillType, unsigned long RGBColor);
   void SSetDrawMode(int Mode);
   void SSetBackgroundColor(unsigned long RGBColor);
   void Sbar(int x , int y, int w, int h);
   void SSetWindowSize(int Width, int Height);
   void SShow(void);
   void SHide(void);
   void SGetKeyEvent(int *Code);
   void SGetBufferedKeyEvents(int *Count);
   void SGetBufferedKeyPressEvents(int *Count);
   void SSetAutoUpdate(bool Mode);
   void Ssetcolor(unsigned long RGBColor);
   void Ssetlinestyle(int Width);
   void Sline(int x1 , int y1, int x2, int y2);
   void Spolygon(QPolygon *Polygon);
   void Spolyline(QPolygon *Polygon);
   void SDrawEllipse(int x, int y, int w, int h);
   void SDrawText(int x, int y,  QString Text);
   void SSelectFont(const char *FontName, int Points, int Style);
   void SGetTextDimensions (TextDimensionType *TextInfo, const char *Text);
   void Spixel(int x1, int y1);
   void SGetPixel(unsigned long *RGBColor, int x, int y);
   void Sclose(void);
   void Scleardevice(void);

   void SCreateImage(int *Id, int Width, int Height);
   void SCreateImage(int *Id, int SrcID, int x, int y, int Width, int Height);
   void SGetImageSize(int ImageId, int *Width, int *Height);
   void SGetPixelDataPointer(int ImageId, void **Ptr, int *Length);
   void SSetEditedImage(int ImageId);
   void SDrawImage(int ImageId, int x, int y);
   void SCopyToImage(int x, int y, int Width, int Height, int ImageId);
   void SAddAlphaMask(int ImageId, int Mode, unsigned long Color);
   void SLoadImage(int *Id, const char *FileName);
   void SSaveImage(int Id, const char *FileName, const char *Format, int Quality);
   void SRotate(float Angle);
   void STranslate(float dx, float dy);
   void SScale(float Scalingx, float Scalingy);
   void SResetTransformations(void);
   void SDestroyImage(int ImageId);

   void SSetQtOptions(int Option, long Value);

   void SPlaySoundContinuous(QString FileName);
   void SStartPlaySoundContinuous(void);
   void SStopContinuousSound(void);
   void SPlaySound(QString FileName);


   void SGetMouseState (struct MouseInfo *Info);
   void SGetMouseEvent (struct MouseInfo *Info);

   void SIncrementUnhandledCommands(void);

   void SStartTimer(int IntervalTime, void *Parameter, void *Handler);
   void SStopTimer(void);

   void SOpenFileDialog(bool *Sucess, int Mode, const char *Title, const char *Directory, const char *Filter, const char *PreferredFilter, int Options, char *FilenameBuffer, int MaxNameLength, char *SelectedFilter, int MaxFilterLength);



protected:
    void run();
private:

    volatile bool stopped;

public:
   void setfillstyle(int FillType, unsigned long RGBColor);
   void SetDrawMode(int Mode);
   void SetBackgroundColor(unsigned long RGBColor);
   void bar(int x , int y, int w, int h);
   void setcolor(unsigned long RGBColor);
   void setlinestyle(int LineStyle, int dummy, int Width);
#if 0
   void rectangle(int x, int y, int w, int h);
#endif
   void ellipse(int x, int y, int w, int h);
   void line(int x1 , int y1, int x2, int y2);
   void polygon(int *Edges, int NumberOfEdges);
   void polyline(int *Edges, int NumberOfEdges);
   void putpixel(int x1 , int y1);
   unsigned long GetPixel (int x, int y);
   void outtextxy(int x, int y, const char *Text);
   void SelectFont(const char *FontName, int Points, int Style);
   TextDimensionType GetTextDimensions (const char *Text);

   void initwindow(int Width, int Height);
   void SetAutoUpdate(int Mode);
   //void settextstyle(TRIPLEX_FONT, HORIZ_DIR, 0);
   //void setusercharsize(1, 3, 1, 3);
   void closegraph( );
   int  getch();
   int  kbhit();
   int  kbEvent();
   void delay(int Time);
   void cleardevice();

   int  CreateImage(int Width, int Height);
   int  CreateImage(int SrcID, int x, int y, int Width, int Height);
   void GetImageSize(int ImageId, int *Width, int *Height);
   ColorType *GetPixelDataPointer(int ImageId, int *Length);
   int  SetEditedImage(int ImageId);
   int  DrawImage(int ImageId, int x, int y);
   void CopyToImage(int x, int y, int Width, int Height, int ImageId);
   void AddAlphaMask(int ImageId, int Mode, unsigned long Color);
   int  LoadImage(const char *FileName);
   void SaveImage(int Id, const char *FileName, const char *Format, int Quality);
   void Rotate(float Angle);
   void Translate(float dx, float dy);
   void Scale(float Scalingx, float Scalingy);
   void ResetTransformations(void);
   int  DestroyImage(int ImageId);

   void SetQtOptions(enum QTOptions Option, long int Value);


   void PlaySoundContinuous(const char *FileName);
   void PlaySoundContinuous(void);
   void StopContinuousSound(void);
   void PlaySound(const char *FileName);

   void GetMouseState (struct MouseInfo *Info);
   void GetMouseEvent (struct MouseInfo *Info);

   void StartTimer(int IntervalTime, void *Parameter, void (*Handler)(void *));
   void StopTimer(void);


   // Support for Multithreading, if anybody needs it...
   void Lock() {MyMutex.lock();};
   void Unlock() {MyMutex.unlock();};
   bool CreateThread(void *Parameter, void (*Function)(void *));

   bool OpenFileDialog(int Mode, const char *Title, const char *Directory, const char *Filter, const char *PreferredFilter, int Options, char *FilenameBuffer, int MaxNameLength, char *SelectedFilter, int MaxFilterLength);


};








#endif /* WORKER_H_ */
