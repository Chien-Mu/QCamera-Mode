#include "videowidget.h"
#include <QDebug>

VideoWidget::VideoWidget(QSize imageSize, QWidget *parent) : QWidget(parent)
{
    this->imageSize = imageSize; //設定圖像大小
    QSize widgetSize(PAINT_WIDTH,PAINT_HEIGTH); //設定畫版大小(非圖像大小)

    this->Wratio = (float)widgetSize.width()/(float)imageSize.width();
    this->Hratio = (float)widgetSize.height()/(float)imageSize.height();

    this->isPush = true;
    this->isdraw = false;

    surface = new MyVideoSurface(this,widgetSize);
    this->setFixedSize(widgetSize); //如果沒設畫板大小，可能顯示會有問題。
    //-----------以上為不可變動區-------------

    rect_null.setX(0);
    rect_null.setY(0);
    rect_null.setWidth(0);
    rect_null.setHeight(0);
    for(int i=0;i<SCANTOTAL;i++)
        this->rects.push_back(rect_null);
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

    for(int i=0;i<SCANTOTAL && !isdraw;i++){ //for 裡面也要加 isdraw 判斷！
        if(i<info.total || !info.SN[i].rect.isNull() || !info.SN[i].rect.isEmpty()){
            this->rects[i] = QRect(info.SN[i].rect.x()*Wratio,
                                   info.SN[i].rect.y()*Hratio,
                                   info.SN[i].rect.width()*Wratio,
                                   info.SN[i].rect.height()*Hratio);
        }else{
            this->rects[i] = rect_null;
        }
    }

    isPush = false;
}

// paintEvent() 與 draw() 是不同執行緒在使用的
void VideoWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (surface->isActive()) {
        surface->Drawing(true);
        QPainter painter; //把widget 當畫板
        painter.begin(this); //實驗這樣把 .begin() .end() 寫出來效率會更快
        surface->paintImage(&painter); //從記憶體取得圖

        //draw
        pen.setBrush(Qt::black);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawLine(0,240,640,240);
        painter.drawLine(320,0,320,480);

        pen.setBrush(Qt::red);
        pen.setWidth(4);
        painter.setPen(pen);
        if(!isPush){
        	/* 加 isdraw 的重要性超大
        	   一定有機率會跑進來後，draw()才執行，所以裡面還要再加保護
        	   避免 draw() 改值
        	   當初因為這原因 搞了很久..*/
            isdraw = true;
            painter.drawRects(rects); //在畫上矩形
            isdraw = false;
        }

        painter.end();
        surface->Drawing(false);
    }
}
