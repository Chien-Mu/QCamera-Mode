#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <dmtx.h>
#include <QMutex>
#include <QWaitCondition>

class scanthread : public QThread
{
    Q_OBJECT
public:
    scanthread();
    //void setScan();
    void run();
    void stop();

private:
    bool quit;
    QByteArray scan(QImage *currentImage);
    QImage *currentImage;
    QMutex mutex;
    QWaitCondition cond;

public slots:
    void receiveIamge(QImage *currentImage);

signals:
    void scan_ok();
};

#endif // SCANTHREAD_H
