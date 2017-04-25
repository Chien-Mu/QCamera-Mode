#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QImage>

#include "camera.h"
#include "shared.h"

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
    QImage *on_Capture();
    QImage currentImage;
    QImage *currentImage_ptr;
    void start();
    void stop();

private:
    Ui::MainWindow *ui;
    Camera *camera;
    QPlainTextEdit *formText;
    VideoWidget *videoWidget;

private slots:
    void displayCaptureError(int id,QCameraImageCapture::Error error,const QString &errorString);
};

#endif // MAINWINDOW_H
