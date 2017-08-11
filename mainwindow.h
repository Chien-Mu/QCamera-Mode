#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/* 此地方預設使用標記 3*/

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QImage>
#include <QGraphicsView> //2
#include <QGraphicsScene> //2

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
    QImage &on_Capture();
    void start();
    void stop();

//protected:
    //virtual void keyPressEvent(QKeyEvent *event); //2

private:
    Ui::MainWindow *ui;
    QPlainTextEdit *formText;

    //camera
    Camera *camera;


private slots:
    void displayCaptureError(int id,QCameraImageCapture::Error error,const QString &errorString);
    //void on_imageCaptured(int id,const QImage &preview); //4
    //void on_imageSaved(int id,const QString &fileName); //4
};

#endif // MAINWINDOW_H
