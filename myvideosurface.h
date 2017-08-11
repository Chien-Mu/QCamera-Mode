#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QWidget>
#include <QVideoSurfaceFormat>
#include <QPainter>
#include <QImage>

//實作 QAbstractVideoSurface 抽象類別
class MyVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    MyVideoSurface(QWidget *widget, QSize widgetSize, QObject *parent = 0); //為了觸發外界的 eventpaint，所以建構子讓 widget 帶進來

    //返回預設支援的格式（一定要實作)
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

    //開始後，會由 present() 自動接收 QVideoFrame
    //官網有說，Calling present() on a non-blocking surface may fail。要用 present() 一定不能同步執行，要配合blocking
    bool present(const QVideoFrame &frame);

    void paintImage(QPainter *painter); //讓外界的 painter 可以進來被劃上image
    void Drawing(bool isDraw);
    void getlock();
    QImage &getCurrentImage();

    
    /*
    //有實作start時一定要一起實作stop，兩個都要同時存在
    //QAbstractVideoSurface 用於影片時才有需要用到start，要將影片QImage轉成QVideoFrame再轉成QVideoSurfaceFormat丟進start
    bool start(const QVideoSurfaceFormat &format);
    void stop();
    */

private:
    QWidget *widget;
    QVideoFrame currentFrame;
    QImage currentImage;
    QImage _image;
    QSize widgetSize;
    volatile bool isDraw;
    volatile bool isGet;
};

#endif // MYVIDEOSURFACE_H
