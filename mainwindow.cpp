#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QCameraInfo>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //為了讓camera在中心
    QSpacerItem * H_spacer = new QSpacerItem(1000,0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem * H_spacer_2 = new QSpacerItem(1000,0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->gridLayout->addItem(H_spacer,0,0);
    ui->gridLayout->addItem(H_spacer_2,0,2);

    //camera
    camera = new Camera;
    initialization();
    ui->gridLayout->addWidget(camera->getVideoWidget(),0,1);


    //Form Text
    formText = new QPlainTextEdit(this);
    ui->gridLayout->addWidget(formText,1,0,1,3);
}

bool MainWindow::initialization(){
    //列出相機
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    qDebug() << cameras[0].deviceName(); //設備id
    qDebug() << cameras[0].description(); //設備名稱

    //camera set
    camera->setCamera(cameras[0].deviceName().toLocal8Bit());
    connect(camera,SIGNAL(Error(int,QCameraImageCapture::Error,QString)),
            this,SLOT(displayCaptureError(int,QCameraImageCapture::Error,QString)));
    camera->CameraStrat();

    return true;
}

void MainWindow::on_Capture(){
    //camera->getCurrentImage();
}

void MainWindow::displayCaptureError(int id, QCameraImageCapture::Error error, const QString &errorString){
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
}

MainWindow::~MainWindow()
{
    delete ui;
}
