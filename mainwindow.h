#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

#include <camera.h>

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

private slots:
    void on_Capture();
    void displayCaptureError(int id,QCameraImageCapture::Error error,const QString &errorString);
};

#endif // MAINWINDOW_H
