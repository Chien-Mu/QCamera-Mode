#include "scanthread.h"
#include <QDebug>

scanthread::scanthread(MainWindow *ref)
{
    this->ref = ref;
    this->quit = false;
}

void scanthread::stop(){
    quit = true;
}

void scanthread::run(){
    this->quit = false;
    QByteArray SN;

    while(!quit){
        ImageFrame *currentImage = ref->on_Capture();

        //一樣的圖不出裡
        if(currentImage->isRepeat || currentImage->image.isNull()){
            msleep(100);
            continue;
        }

        //scanner
        if(!quit){
            SN = scan(&currentImage->image);
            qDebug() << "Decode: " << SN;
        }
        msleep(100);
    }
}

QByteArray scanthread::scan(QImage *currentImage){
    QByteArray SN;
    QRect rect;

    DmtxImage *dmImg;// = new DmtxImage;
    DmtxDecode *dmDec;// = new DmtxDecode;
    DmtxRegion *dmReg;// = new DmtxRegion;
    DmtxMessage *dmMsg;// = new DmtxMessage;
    DmtxTime timeout;

    //position
    DmtxVector2 p00, p10, p11, p01;
    p00.X = p00.Y = p10.Y = p01.X = 0.0;
    p10.X = p01.Y = p11.X = p11.Y = 1.0;
    int p_height;

    dmImg = dmtxImageCreate(currentImage->bits(),currentImage->width(),currentImage->height(),DmtxPack8bppK);
    if(dmImg == NULL)
        return "-1";

    //Image 屬性
    dmtxImageSetProp(dmImg, DmtxPropImageFlip, DmtxFlipNone); //不翻轉？

    dmDec = dmtxDecodeCreate(dmImg,1); //shrink;
    if(dmDec == NULL)
        return "-1";

    //Decode 屬性
    //dmtxDecodeSetProp(dmDec, DmtxPropEdgeMin, 70); //minimum-edge
    //dmtxDecodeSetProp(dmDec, DmtxPropEdgeMax, 220); //maximum-edge
    //dmtxDecodeSetProp(dmDec, DmtxPropScanGap, 1); //gap
    //dmtxDecodeSetProp(dmDec, DmtxPropEdgeThresh, 100); //threshold 越高速度越快

    //再迴圈內定義成找一個條碼 不能超過 timeoutMS 時間
    //再迴圈外定義成找 ScanLimit 個條碼 不能超過 timeoutMS 時間
    int timeoutMS = 200;
    int ScanCount=0;
    int ScanLimit=1;
    while(ScanCount < ScanLimit){
        timeout = dmtxTimeAdd(dmtxTimeNow(),timeoutMS); // timeout
        //dmtxDecodeSetProp(dmDec, DmtxPropXmin, (int)(p10.X + 0.5)); //skip
        dmReg = dmtxRegionFindNext(dmDec,&timeout); //find
        if(dmReg == NULL){
            rect.setX(0);
            rect.setY(0);
            rect.setWidth(0);
            rect.setHeight(0);
            emit throwInfo(rect);
            break;
        }

        dmMsg = dmtxDecodeMatrixRegion(dmDec,dmReg,DmtxUndefined); //decode
        if(dmMsg != NULL){
            //char* to QString(雖然型態是uchar，不過原廠是轉char)
            SN = (char*)dmMsg->output;

            //position information
            p00.X = p00.Y = p10.Y = p01.X = 0.0;
            p10.X = p01.Y = p11.X = p11.Y = 1.0;
            p_height = dmtxDecodeGetProp(dmDec, DmtxPropHeight);
            dmtxMatrix3VMultiplyBy(&p00, dmReg->fit2raw);
            dmtxMatrix3VMultiplyBy(&p10, dmReg->fit2raw);
            dmtxMatrix3VMultiplyBy(&p11, dmReg->fit2raw);
            dmtxMatrix3VMultiplyBy(&p01, dmReg->fit2raw);
            //qDebug() << (int)(p00.X + 0.5) << p_height - 1 - (int)(p00.Y + 0.5);
            //qDebug() << (int)(p10.X + 0.5) << p_height - 1 - (int)(p10.Y + 0.5);
            //qDebug() << (int)(p11.X + 0.5) << p_height - 1 - (int)(p11.Y + 0.5);
            //qDebug() << (int)(p01.X + 0.5) << p_height - 1 - (int)(p01.Y + 0.5);
            rect.setX((int)(p01.X + 0.5));
            rect.setY(p_height - 1 - (int)(p01.Y + 0.5));
            rect.setWidth((int)(p11.X + 0.5) - rect.x());
            rect.setHeight((p_height - 1 - (int)(p00.Y + 0.5)) - rect.y());
            emit throwInfo(rect);

            dmtxMessageDestroy(&dmMsg);
        }
        //default
        ScanCount++;
        dmtxRegionDestroy(&dmReg);
    }

    dmtxDecodeDestroy(&dmDec);
    dmtxImageDestroy(&dmImg);

    /*delete dmImg;
    delete dmDec;
    delete dmReg;
    delete dmMsg;*/
    return SN;
}
