#include "videowidget.h"
#include <QDebug>

VideoWidget::VideoWidget(int W, int H, QWidget *parent) : QWidget(parent)
{
    this->W = W;
    this->H = H;
    this->rectCount = 0;

    surface = new MyVideoSurface(this,W,H);
    this->setFixedSize(W,H); //如果沒設畫板大小，可能顯示會有問題。
}

VideoWidget::~VideoWidget(){
    delete surface;
}

void VideoWidget::draw(QRect *rects ,int rectCount){
    if(rects->isNull()){
        this->rectCount = 0;
        return;
    }

    //check
    for(int i=0;i<rectCount;i++)
        if(rects[i].isNull() || rects[i].isEmpty()){
            this->rectCount = 0;
            qDebug() << "isNull error";
            return;
        }

    this->rects = rects;
    this->rectCount = rectCount;
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (surface->isActive()) {
        painter.begin(this); //把widget 當畫板
        surface->paintImage(&painter); //從記憶體取得圖

        //draw
        pen.setBrush(Qt::red);     
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawRects(this->rects,this->rectCount); //在畫上矩形
        painter.end();
    }
}
