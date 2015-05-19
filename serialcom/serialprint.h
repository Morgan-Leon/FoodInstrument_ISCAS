#ifndef SERIALPRINT_H
#define SERIALPRINT_H

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

class serialPrint : public QObject
{
    Q_OBJECT
public:
    explicit serialPrint(QObject *parent = 0);
    ~serialPrint();

public:
    Posix_QextSerialPort* MyCom;   ////linux serial
//    Win_QextSerialPort* MyCom;       ////win7 serial

    bool Printchar(char Pdata1);             //´òÓ¡ÃüÁîASCIIÂë
    bool PrintString(QString Pdata2);       //´òÓ¡ÄÚÈÝ×Ö·û´®


private:
    QString backStr;
    QByteArray tmpArray;
    bool initSerial(void);
    void cleardata();
    void mydelay(int needtime);     ///ms
};

#endif // SERIALPRINT_H
