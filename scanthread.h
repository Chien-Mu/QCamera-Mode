#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <dmtx.h>

#include "mainwindow.h"
#include "shared.h"

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

signals:
    void throwInfo(const INFO value);
};

#endif // SCANTHREAD_H
