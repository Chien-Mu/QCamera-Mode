#include "videowidget.h"

VideoWidget::VideoWidget(int W, int H, QWidget *parent) : QWidget(parent)
{
    this->W = W;
    this->H = H;

    surface = new MyVideoSurface(this,W,H);
    this->setFixedSize(W,H); //如果沒設畫板大小，可能顯示會有問題。
}

VideoWidget::~VideoWidget(){
    delete surface;
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); //把widget 當畫板

    if (surface->isActive()) {
        surface->paintImage(&painter); //從記憶體取得圖

        //levy
        QPen pen;
        pen.setBrush(Qt::red);
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawRect(10,10,500,500); //在畫上矩形
    }
}
