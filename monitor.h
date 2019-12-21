#ifndef MONITOR_H
#define MONITOR_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QTimer>
#include <QBuffer>

#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include "ThreeDog/tdwidget.h"
class Monitor : public TDWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = 0);
    ~Monitor();

public slots:
    void newConnections();          //閺傛澘顓归幋椋庮伂鏉╃偞甯村Σ钘夊毐閺?
    void readyReadSlot();           //鐠囪褰囨穱鈩冧紖濡茶棄鍤遍弫?
    void tackImage(int ,QImage);    //鐠佸墽鐤嗛崶鍓у
    void timerSlot();               //鐎规碍妞傞崳銊π崙鑺ユ殶

private:
    QTcpSocket  *socket;            //
    QTcpServer  *server;            //閺堝秴濮熼崳銊ヮ殰閹恒儱鐡?

    QCamera     *camera;            //閹藉嫬鍎氭径鏉戭嚠鐠?
    QCameraViewfinder   *viewFinder;    //閺勫墽銇氶幑鏇″箯鐟欏棝顣剁€电钖?
    QCameraImageCapture *imageCapture;  //閹搭亜娴樼€电钖?

    QTimer      *timer;
};


#endif // MONITOR_H
