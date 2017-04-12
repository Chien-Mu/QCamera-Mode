#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <dmtx.h>

#include "mainwindow.h"

class scanthread : public QThread
{
    Q_OBJECT
public:
    scanthread(MainWindow *ref);
    //void setScan();
    void run();
    void stop();

private:
    MainWindow *ref;
    bool quit;
    QByteArray scan(QImage *currentImage);
    //DmtxImage *dmImg;
    //DmtxDecode *dmDec;
    //DmtxRegion *dmReg;
    //DmtxMessage *dmMsg;
    //QRect rect;

signals:
    void throwInfo(QRect rect);
};

#endif // SCANTHREAD_H
