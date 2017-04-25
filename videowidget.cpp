#include "videowidget.h"
#include <QDebug>

VideoWidget::VideoWidget(int W, int H, QWidget *parent) : QWidget(parent)
{
    this->W = W;
    this->H = H;
    this->isPush = true;
    this->isdraw = false;
    rect_null.setX(0);
    rect_null.setY(0);
    rect_null.setWidth(0);
    rect_null.setHeight(0);
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
    while(isdraw)
        continue;

    for(int i=0;i<SCANTOTAL && !isdraw;i++){
        if(i<info.total || !info.SN[i].rect.isNull() || !info.SN[i].rect.isEmpty())
            this->rects[i] = info.SN[i].rect;
        else
            this->rects[i] = rect_null;
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
        pen.setBrush(Qt::red);
        pen.setWidth(4);
        painter.setPen(pen);
        if(!isPush){
            isdraw = true;
            painter.drawRects(rects); //在畫上矩形
            isdraw = false;
        }

        painter.end();
        surface->Drawing(false);
    }
}
