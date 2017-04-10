#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include "myvideosurface.h"

class VideoWidget : public QWidget
{
public:
    explicit VideoWidget(int W,int H, QWidget *parent = 0);
    ~VideoWidget();
    QAbstractVideoSurface *refVideoSurface() const { return surface; } //將使用的 MyVideoSurface 傳給外界(取景器用)
    QImage currentImage() { return surface->currentImage(); } //直接從 MyVideoSurface出圖

protected:
    void paintEvent(QPaintEvent *event);

private:
    MyVideoSurface *surface;
    int W;
    int H;
};

#endif // VIDEOWIDGET_H
