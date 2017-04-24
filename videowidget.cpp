#include "videowidget.h"
#include <QDebug>

VideoWidget::VideoWidget(int W, int H, QWidget *parent) : QWidget(parent)
{
    this->W = W;
    this->H = H;
    this->isPush = true;
    QRect rect_null(0,0,0,0);
    this->rect_null.push_back(rect_null);
    for(int i=0;i<SCANTOTAL;i++)
        this->rects.push_back(rect_null);

    surface = new MyVideoSurface(this,W,H);
    this->setFixedSize(W,H); //如果沒設畫板大小，可能顯示會有問題。
}

VideoWidget::~VideoWidget(){
    delete surface;
}

void VideoWidget::lock(){
    this->isPush = true;
}

void VideoWidget::draw(INFO info){
    for(int i=0;i<SCANTOTAL;i++){
        if(i<info.total || !info.infoSN[i].rects.isNull() || !info.infoSN[i].rects.isEmpty())
            this->rects[i] = info.infoSN[i].rects;
        else
            this->rects[i] = this->rect_null[0];
    }

    isPush = false;
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (surface->isActive()) {
        surface->Drawing(true);
        QPainter painter; //把widget 當畫板
        painter.begin(this); //實驗這樣把 .begin() .end() 寫出來效率會更快
        surface->paintImage(&painter); //從記憶體取得圖

        //draw
        if(!isPush){
            pen.setBrush(Qt::red);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawRects(rects); //在畫上矩形
        }

        painter.end();
        surface->Drawing(false);
    }
}
