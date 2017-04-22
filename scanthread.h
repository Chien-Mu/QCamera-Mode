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
    QRect rects[2];
    int rectCount;

private:
    MainWindow *ref;
    bool quit;
    QByteArray scan(QImage *currentImage);

signals:
    void throwInfo(QRect*,int);
};

#endif // SCANTHREAD_H
