#include "serialprint.h"
#include "../global/externdata.h"

serialPrint::serialPrint(QObject *parent) :
    QObject(parent)
{
    if(!initSerial())
        qDebug()<<"init";////¾¯¸æ´°¿Ú

}
serialPrint::~serialPrint()
{
    MyCom->close();
}

void serialPrint::mydelay(int needtime)
{
    ////wait
    QTime nt;
    nt.start();
    while(nt.elapsed()<needtime)
        QCoreApplication::processEvents();
}

bool serialPrint::initSerial()
{
    /*
    struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
    myCom =new  Win_QextSerialPort("\\\\.\\COM10",myComSetting,QextSerialBase::EventDriven);
    myCom->open(QIODevice::ReadWrite);
    */
    MyCom=new Posix_QextSerialPort(SerialPort0,QextSerialBase::Polling);      ////linux serial
//    MyCom =new  Win_QextSerialPort("\\\\.\\COM9",QextSerialBase::Polling);     ////win7 serial
    MyCom->open(QIODevice::ReadWrite);
    MyCom->setBaudRate(BAUD9600);
    MyCom->setDataBits(DATA_8);
    MyCom->setParity(PAR_NONE);
    MyCom->setStopBits(STOP_1);
    MyCom->setFlowControl(FLOW_OFF);
    MyCom->setTimeout(100);
    if(MyCom->isOpen())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool serialPrint::Printchar(char Pdata1)
{
    tmpArray = (QByteArray)(&Pdata1);
    MyCom->write(tmpArray);
    qDebug()<<tmpArray;
    return true;
}

bool serialPrint::PrintString(QString Pdata2)
{
    MyCom->write(Pdata2.toAscii());
    return true;
}

void serialPrint::cleardata()
{
    tmpArray = MyCom->readAll();
}
