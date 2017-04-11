#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H

#include <QAbstractVideoSurface>
#include <QWidget>
#include <QVideoSurfaceFormat>
#include <QPainter>

//實作 QAbstractVideoSurface 抽象類別
class MyVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    MyVideoSurface(QWidget *widget, int W, int H, QObject *parent = 0); //為了觸發外界的 eventpaint，所以建構子讓 widget 帶進來

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

    bool present(const QVideoFrame &frame);

    void paintImage(QPainter *painter); //讓外界的 painter 可以進來被劃上image
    QImage currentImage; //因為要送指標出去，所以用 public
    QImage *getCurrentImage();

private:
    QWidget *widget;
    QVideoFrame currentFrame;
    int W;
    int H;
};

#endif // MYVIDEOSURFACE_H
