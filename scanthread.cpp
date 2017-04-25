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
        QImage *currentImage = ref->on_Capture();

        //check
        if(currentImage->isNull()){
            msleep(200);
            continue;
        }

        //scanner
        if(!quit){
            SN = scan(currentImage);
            //qDebug() << "Decode: " << SN;
        }
        //msleep(200);
    }
}

QByteArray scanthread::scan(QImage *currentImage){
    QByteArray SN;

    DmtxImage *dmImg;
    DmtxDecode *dmDec;
    DmtxRegion *dmReg;
    DmtxMessage *dmMsg;
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

    // dmtxTimeAdd()再迴圈內定義成找一個條碼 不能超過 timeoutMS 時間
    // dmtxTimeAdd()再迴圈外定義成找 ScanLimit 個條碼 不能超過 timeoutMS 時間
    int timeoutMS = 200;
    int ScanCount=0;
    int ScanLimit=SCANTOTAL;
    INFO info;
    info.total = 0;
    for(int i =0;i<ScanLimit ;i++){
        info.SN[i].text = "";
        info.SN[i].rect.setX(0);
        info.SN[i].rect.setY(0);
        info.SN[i].rect.setWidth(0);
        info.SN[i].rect.setHeight(0);
    }

    for(int i=0;i<ScanLimit;i++){
        //defualt
        timeout = dmtxTimeAdd(dmtxTimeNow(),timeoutMS); // timeout
        dmReg = dmtxRegionFindNext(dmDec,&timeout); //find

        if(dmReg == NULL)
            break;

        dmMsg = dmtxDecodeMatrixRegion(dmDec,dmReg,DmtxUndefined); //decode
        if(dmMsg != NULL){
            SN = (char*)dmMsg->output;
            info.SN[ScanCount].text = SN;

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
            info.SN[ScanCount].rect.setX((int)(p01.X + 0.5));
            info.SN[ScanCount].rect.setY(p_height - 1 - (int)(p01.Y + 0.5));
            info.SN[ScanCount].rect.setWidth((int)(p11.X + 0.5) - info.SN[ScanCount].rect.x());
            info.SN[ScanCount].rect.setHeight((p_height - 1 - (int)(p00.Y + 0.5)) - info.SN[ScanCount].rect.y());
            ScanCount++;

            //release dmMsg
            dmtxMessageDestroy(&dmMsg);
        }
        //default
        dmtxRegionDestroy(&dmReg);
    }
    info.total = ScanCount;
    emit throwInfo(info);
    //qDebug() << "ScanCount" << QString::number(ScanCount);

    dmtxDecodeDestroy(&dmDec);
    dmtxImageDestroy(&dmImg);
    return SN;
}
