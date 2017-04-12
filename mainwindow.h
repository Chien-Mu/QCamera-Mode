#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QImage>

#include <camera.h>
#include "scanthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool initialization();

private:
    Ui::MainWindow *ui;
    Camera *camera;
    QPlainTextEdit *formText;
    scanthread *scanner;
    ImageFrame currentImage;
    int TmpID;

private slots:
    void on_Capture();
    void displayCaptureError(int id,QCameraImageCapture::Error error,const QString &errorString);

signals:
    void throwImage(QImage value,int id);
};

#endif // MAINWINDOW_H
