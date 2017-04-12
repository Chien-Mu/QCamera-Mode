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

void VideoWidget::draw(QRect rect){
    this->rect = rect;
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    painter.begin(this); //把widget 當畫板

    if (surface->isActive()) {
        surface->paintImage(&painter); //從記憶體取得圖

        //levy
        pen.setBrush(Qt::red);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawRect(this->rect); //在畫上矩形
    }
    painter.end();
}
