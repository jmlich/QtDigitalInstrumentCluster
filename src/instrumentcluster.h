#ifndef INSTRUMENTCLUSTER_H
#define INSTRUMENTCLUSTER_H

#include <QObject>
#include <QSerialPort>
#include "serialobd.h"

class InstrumentCluster : public QObject
{
    Q_OBJECT
public:

    ///run OBD object in a different thread,
    ///that way there's no blocking operations
    InstrumentCluster(){
        OBD = new SerialOBD;

        ClusterThread = new QThread;

        OBD->moveToThread(ClusterThread);

        connect(this,SIGNAL(start()),OBD,SLOT(ConnectToSerialPort()));

        //connect signals in OBD object to this object, to report to QML
        connect(OBD,SIGNAL(obdRPM(int)),this,SIGNAL(obdRPM(int)));
        connect(OBD,SIGNAL(obdMPH(int)),this,SIGNAL(obdMPH(int)));
        connect(OBD,SIGNAL(obdFuelStatus(int)),this,SIGNAL(obdFuelStatus(int)));
        connect(OBD,SIGNAL(obdCoolantTemp(int)),this,SIGNAL(obdCoolantTemp(int)));
        connect(OBD,SIGNAL(obdTroubleCode(QByteArray)),this,SIGNAL(obdTroubleCode(QByteArray)));

        ClusterThread->start();
    }

    ~InstrumentCluster(){
        ClusterThread->quit();
        ClusterThread->wait();
        delete OBD;
        delete ClusterThread;
    }

signals:
    void start();
    void obdRPM(int rpm);
    void obdMPH(int speed);
    void obdFuelStatus(int fuel);
    void obdCoolantTemp(int coolantTemp);
    void obdThrottlePosition(int throttle);
    void obdTroubleCode(QByteArray troublecode);
public slots:

private:
    SerialOBD* OBD;
    QThread* ClusterThread;

};

#endif // INSTRUMENTCLUSTER_H
