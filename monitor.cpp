#include "monitor.h"
//#include "ui_mainwindow.h"
#include "ThreeDog/tdscrollarea.h"
#include "ThreeDog/tdradiobuttongroup.h"
#include "ThreeDog/tdtoolbar.h"
#include "qpushbutton.h"
#include "ThreeDog/tdpushbutton.h"
#include "ThreeDog/tdmenubutton.h"
#include  <QDebug>
#include<qserialport.h>
#include<QSerialPort>

//Widget::Widget(QWidget *parent)
//    : TDWidget(":/img/jiaju/gtting.jpg",parent,true)
//{

//    TDToolbar *kaiguan=new TDToolbar(":/img/device/switch_open.png",":/img/device/switch_close.png",this);
//    kaiguan->move(133,210);
//    jiankong->setCallback(this,my_selector(menu1Clicked));
//}
Monitor::Monitor(QWidget *parent) :
    TDWidget(":/img/jiaju/geting.jpg",parent)
{
    //ui(new Ui::MainWindow)
    this->resize(993,720);
    TDToolbar *kaiguan=new TDToolbar(":/img/device/switch_open.png",":/img/device/switch_close.png",this);
    kaiguan->move(133,210);

    //ui->setupUi(this);
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any,10000);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnections()));


    QList<QCameraInfo> cameraInfo = QCameraInfo::availableCameras();

    foreach(const QCameraInfo &cameras,cameraInfo)
    {
        camera = new QCamera(cameras);
    }
    camera->setCaptureMode(QCamera::CaptureStillImage);
    viewFinder = new QCameraViewfinder(this);
    camera->setViewfinder(viewFinder);
    imageCapture = new QCameraImageCapture(camera);
    QVBoxLayout * v = new QVBoxLayout;
    v->addWidget(viewFinder);
    this->setLayout(v);
    //this->setCentralWidget(viewFinder);

    connect(imageCapture,SIGNAL(imageCaptured(int,QImage)),this,SLOT(tackImage(int,QImage)));
    camera->start();
}

Monitor::~Monitor()
{
}

void Monitor::newConnections()
{
    socket = server->nextPendingConnection();

    if(!socket){
        return;
    }
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
}

void Monitor::readyReadSlot()
{
    qDebug()<<"read:"<<socket->readAll();

    QByteArray buf = "HTTP/1.0 200 Ok\r\n"
                     "Connection:Keep-Alive\r\n"
                     "Server:Network camera\r\n"
                     "Cache-Control:no-cachenno-store,must-revalidate,pre-check=0,"
                     "max-age=0\r\n"
                     "Content-Type:multipart/x-mixed-replace;boundary=BRIUP\r\n";
    socket->write(buf);

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Monitor::timerSlot);
    timer->start(100);
}

void Monitor::tackImage(int, QImage img)
{

    QString str = "\r\n--HAHAHA\r\n"
                  "Content-Type:image/jpeg\n"
                  "Content-Length:%1\n\n";
    str = str.arg(img.byteCount());

    qDebug()<<"str:"<<str;


    QByteArray sendbuf;
    sendbuf.clear();
    sendbuf.append(str);


    QPixmap pix = QPixmap::fromImage(img);
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);
    pix.save(&buf,"JPEG");

    sendbuf.append(buf.data());
    socket->write(sendbuf);
}

void Monitor::timerSlot()
{

    camera->searchAndLock();
    imageCapture->capture(QString("temp_image"));
    camera->unlock();
}
