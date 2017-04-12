#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include "myvideosurface.h"

class VideoWidget : public QWidget
{
public:
    explicit VideoWidget(int W,int H, QWidget *parent = 0);
    ~VideoWidget();
    QAbstractVideoSurface *refVideoSurface() const { return surface; } //已 QAbstractVideoSurface 傳給外界(取景器)
    MyVideoSurface *refImageSurface() { return surface; } //已 MyVideoSurface 傳給外界(取圖用)
    void draw(QRect rect);

protected:
    void paintEvent(QPaintEvent *event);

private:
    MyVideoSurface *surface;
    int W;
    int H;
    QPen pen;
    QPainter painter;
    QRect rect;
};

#endif // VIDEOWIDGET_H
