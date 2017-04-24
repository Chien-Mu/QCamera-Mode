#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QVector>
#include "myvideosurface.h"
#include "shared.h"

class VideoWidget : public QWidget
{
public:
    explicit VideoWidget(int W,int H, QWidget *parent = 0);
    ~VideoWidget();
    QAbstractVideoSurface *refVideoSurface() const { return surface; } //已 QAbstractVideoSurface 傳給外界(取景器)
    MyVideoSurface *refImageSurface() { return surface; } //已 MyVideoSurface 傳給外界(取圖用)
    void lock();
    void draw(INFO info);

protected:
    void paintEvent(QPaintEvent *event);

private:
    MyVideoSurface *surface;
    int W;
    int H;
    QPen pen;
    QVector<QRect> rects;
    QVector<QRect> rect_null;
    bool isPush;
};

#endif // VIDEOWIDGET_H
