#ifndef SERIALCOM_H
#define SERIALCOM_H

#include <QObject>
#include <QtGui/QApplication>
#include <QTimer>
#include "seriallinux/posix_qextserialport.h"      ////linux serial
//#include "serialwin7/win_qextserialport.h"      ////win7 serial
#include "../global/externdata.h"
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QTime>


class serialCOM : public QObject
{
    Q_OBJECT
public:
    explicit serialCOM(QObject *parent = 0);
    ~serialCOM();

public:
    Posix_QextSerialPort* MyCom;   ////linux serial
//    Win_QextSerialPort* MyCom;       ////win7 serial
    bool MultiCMD(int cmdtype,int value);


private:
    int readtimes;
    int tmpCMDvalue;
    int tmpreadtype;    
    QString backStr;
    QByteArray tmpArray;
    QTimer* readTimer;
    bool signalCMD(int cmdtype,int value);

    bool initSerial(void);
    void intToStr(int value,QString &tmpStr);
    QString sumCheckStr(QString &tmpStr);
    bool bCheckStr(QString &tmpStr);

    void savebakdata(int cmdtype,int value);
    void cleardata();    
    int matchdata(int num,float fvalue);
    void mydelay(int needtime);     ///ms
    void Hex2String(QByteArray senddata,QString &str);
    void String2Hex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);

public slots:
    void readMyCom();

};

#endif // SERIALCOM_H
