#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QVector>
#include "myvideosurface.h"
#include "shared.h"

/* 因為 QAbstractVideoSurface 只提供從記憶體處理圖、取圖，但沒有畫板功能
 * 所以這裏做一個 QWidget 當 QAbstractVideoSurface 的畫版*/

class VideoWidget : public QWidget
{
public:
    explicit VideoWidget(QSize imageSize, QWidget *parent = 0);
    ~VideoWidget();
    QAbstractVideoSurface *refVideoSurface() const { return surface; } //已 QAbstractVideoSurface 傳給外界(取景器)
    MyVideoSurface *refImageSurface() { return surface; } //已 MyVideoSurface 傳給外界(取圖用)
    void lock();
    void draw(INFO info);

protected:
    void paintEvent(QPaintEvent *event);

private:
    MyVideoSurface *surface;
    QSize imageSize;
    float Wratio;
    float Hratio;
    QPen pen;
    QVector<QRect> rects;
    QRect rect_null;
    volatile bool isPush;
    volatile bool isdraw;
};

#endif // VIDEOWIDGET_H
