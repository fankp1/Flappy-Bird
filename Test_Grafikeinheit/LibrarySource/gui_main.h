#ifndef MAIN_GUI_H
#define MAIN_GUI_H

#include <QWidget>

#define NO_PROTOTYPES_IN_WINDOW_H

//#include <QMainWindow>
#include "Worker.h"
#include <QMutex>
#include <list>
#include <vector>

#include "window.h"

#include <QSound>

class MainGui : public QWidget
{
    Q_OBJECT

public:
   MainGui(QWidget *parent = 0);
    ~MainGui();

    void paintEvent(QPaintEvent * /* event */);
    void closeEvent(QCloseEvent *event);

public slots:
   void setfillstyle(int FillType, unsigned long RGBColor);
   void SetDrawMode(int Mode);
   void SetBackgroundColor(unsigned long RGBColor);
   void bar(int x , int y, int w, int h);
   void SetWindowSize(int Width, int Height);
   void GetKeyEvent(int *Code);
   void GetBufferedKeyPressEvents(int *Count);
   void GetBufferedKeyEvents(int *Count);
   void SetAutoUpdate(bool Mode);
   void setcolor(unsigned long RGBColor);
   void setlinestyle(int Width);
   void line(int x1 , int y1, int x2, int y2);
   void polygon(QPolygon *Polygon);
   void polyline(QPolygon *Polygon);
   void pixel(int x, int y);
   void GetPixel(unsigned long *RGBColor, int x, int y);
   void DrawEllipse(int x, int y, int w, int h);
   void DrawText(int x, int y,  QString Text);
   void SelectFont(const char *FontName, int Points, int Style);
   void GetTextDimensions (TextDimensionType *TextInfo, const char *Text);
   void DoClose(void);
   void cleardevice(void);

   void CreateImage(int *Id, int Width, int Height);
   void CreateImage(int *Id, int SrcID, int x, int y, int Width, int Height);
   void GetImageSize(int ImageId, int *Width, int *Height);
   void GetPixelDataPointer(int ImageId, void **Ptr, int *Length);
   void SetEditedImage(int ImageId);
   void DrawImage(int ImageId, int x, int y);
   void CopyToImage(int x, int y, int Width, int Height, int ImageId);
   void AddAlphaMask(int ImageId, int Mode, unsigned long Color);
   void LoadImage(int *Id, const char *FileName);
   void SaveImage(int Id, const char *FileName, const char *Format, int Quality);

   void Rotate(float Angle);
   void Translate(float dx, float dy);
   void Scale(float Scalingx, float Scalingy);
   void ResetTransformations(void);
   void DestroyImage(int ImageId);

   void SetQtOptions(int Option, long Value);

   void PlaySoundContinuous(QString FileName);
   void StartPlaySoundContinuous(void);
   void StopContinuousSound(void);
   void PlaySound(QString FileName);

   void GetMouseState (struct MouseInfo *Info);
   void GetMouseEvent (struct MouseInfo *Info);

   void TimerTick(void);
   void StartTimer(int IntervalTime, void *Parameter, void *);
   void StopTimer(void);

   void OpenFileDialog(bool *Sucess, int Mode, const char *Title, const char *Directory, const char *Filter, const char *PreferredFilter, int Options, char *FilenameBuffer, int MaxNameLength, char *SelectedFilter, int MaxFilterLength);

  // void IncrementUnhandledCommands(void);


protected:
   void keyPressEvent     (QKeyEvent *event);
   void keyReleaseEvent   (QKeyEvent *event);
   void mouseMoveEvent    (QMouseEvent *event);
   void mousePressEvent   (QMouseEvent *event);
   void mouseReleaseEvent (QMouseEvent *event);
   void mouseDoubleClickEvent (QMouseEvent *event);
   void wheelEvent        (QWheelEvent  * event);

private:
  //  Ui::qttest2Class ui;
    Worker MyThread;
   // int ToPaint;
   // int Repaint;
    unsigned long FillColor;
    unsigned long PenColor;
    QImage   *Image;
    QPainter *MyPainter;
    QPainter *CurrentPainter;
    int CurrentImage;
    QMutex mutex;
    std::list<int> KeyBuffer;
    MouseInfoType CurrentMouse;
    std::list<MouseInfoType> MouseEventBuffer;
    std::vector<QImage *> ImageBuffers;
    int NumberOfUnhandledKeyPresses;
    bool AutoUpdate;
    bool AcceptClosing;

    QTimer  *MyTimer;
    void *TimerTickParameter;
    void (*TimerTickHandler)(void *TimerTickParameter);

    QSound *MyBackgroundSong;
   // int NumberOfUnhandledCommands;
};

#endif // MAIN_GUI_H
