#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <dmtx.h>

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
    QByteArray scan(QImage currentImage);
    QImage currentImage;
    int id;

public slots:
    void receiveIamge(QImage currentImage, int id);

signals:
    void scan_ok();
};

#endif // SCANTHREAD_H
