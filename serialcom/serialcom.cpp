#include "serialcom.h"
#include "../global/externdata.h"

serialCOM::serialCOM(QObject *parent) :
    QObject(parent)
{    
    if(!initSerial())
        qDebug()<<"init";////警告窗口

}
serialCOM::~serialCOM()
{
    MyCom->close();
//    delete readTimer;
}
/*
cmdtype:CMDType   ///command type
value:static int sign_TestCHNum; //通道值 CHNum
Data::SetTime=0;       //5-1000ms
error channel value errorFlag;
float version;                      //版本
float ListSpecNm[3][1024];          //各通道波长列表值
int DarkData[3][1024];              //各通道暗电流AD值
int BaseData[3][1024];              //各通道空白AD值
int SampleData[3][1024];            //各通道样品AD值
*/
bool serialCOM::MultiCMD(int cmdtype, int value)
{
    bool ok=true;
    int tmp;
    Data::errorFlag=0;    
    tmpCMDvalue=cmdtype;
    tmpreadtype=cmdtype%10;
    switch(cmdtype)
    {
    case online:
    case readversion:
    case lightSW:
    case settime:
    case readsigwave:
    case readinit:
        mydelay(NeedDelay*1000);
        ok=signalCMD(cmdtype, value);
        break;
    case readwave:
    case readspec0:
    case readspec1:
    case readspec2:
        tmp=value;
        for(int i=0;i<3;i++)
        {
            if(tmp&0x01)
            {
                mydelay(NeedDelay*1000);
                ok &=signalCMD(cmdtype/10*10, i+1);
            }
            tmp=tmp>>1;
        }        
        break;
    default:
        break;
    }
    return ok;
}
void serialCOM::mydelay(int needtime)
{
    ////wait
    QTime nt;
    nt.start();
    while(nt.elapsed()<needtime)
        QCoreApplication::processEvents();
}

bool serialCOM::initSerial()
{
    /*
    struct PortSettings myComSetting = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};
    myCom =new  Win_QextSerialPort("\\\\.\\COM10",myComSetting,QextSerialBase::EventDriven);
    myCom->open(QIODevice::ReadWrite);
    */
    MyCom=new Posix_QextSerialPort(SerialPort1,QextSerialBase::Polling);      ////linux serial
//    MyCom =new  Win_QextSerialPort("\\\\.\\COM9",QextSerialBase::Polling);     ////win7 serial
    MyCom->open(QIODevice::ReadWrite);
    MyCom->setBaudRate(BAUD115200);
    MyCom->setDataBits(DATA_8);
    MyCom->setParity(PAR_NONE);
    MyCom->setStopBits(STOP_1);
    MyCom->setFlowControl(FLOW_OFF);
    MyCom->setTimeout(100);
    if(MyCom->isOpen())
    {
        readTimer = new QTimer(this);
        connect(this->readTimer,SIGNAL(timeout()),this,SLOT(readMyCom()));
        return true;
    }
    else
    {
        return false;
    }
}

/*  cmdtype is command type;
    when cmdtype is online,readversion,value can be anything.
    when cmdtype is settime,readwave,readspec,value is the channel number.(1,2,3)
    when cmdtype is lightSW,value is the light status.(1 -- on , 0 -- off)
*/
bool serialCOM::signalCMD(int cmdtype, int value)
{
    bool ok;
//    unsigned char *pchar;
    QTime t;
    bool breturn=false;
    int tmptimeval=0;
    int tmpint;
    backStr="";
    QByteArray mysenddata;
    QString writeStr="5AA5";
    intToStr(cmdtype,writeStr);
    switch(cmdtype)
    {
    case online:        
    case readversion:
        writeStr+="0000000000000000";
        tmptimeval=timeNorVal;
        break;
    case settime:
        intToStr(Data::SetTime/256,writeStr);
        intToStr(Data::SetTime%256,writeStr);
        intToStr(value-1,writeStr);
        writeStr+="0000000000";
        tmptimeval=timeNorVal;
        break;
    case readwave:
    case readspec0:
        intToStr(value-1,writeStr);
        writeStr+="00000000000000";
        tmptimeval=timeSpecVal;
        break;
    case lightSW:
        intToStr(value,writeStr);
        writeStr+="00000000000000";
        tmptimeval=timeNorVal;
        break;
    case readsigwave:
        intToStr(value-1,writeStr);
        intToStr(Data::average_time,writeStr);
        tmpint=matchdata(value-1,Data::GetWaveLength);
        qDebug()<<"====tmpint===="<<tmpint;
        intToStr(tmpint/256,writeStr);
        intToStr(tmpint%256,writeStr);
        writeStr+="00000000";       
        tmptimeval=timeNorVal;
        break;
    case readinit:
        intToStr(value,writeStr);
        writeStr+="00000000000000";
        tmptimeval=timeNorVal;
        break;
    default:
        break;
    }
    writeStr+=sumCheckStr(writeStr);    
    cleardata();    
    //write com data
    mysenddata.clear();
    String2Hex(writeStr,mysenddata);
    MyCom->write(mysenddata);
    //
    readtimes=READNUM*Data::average_time;
    readTimer->start(tmptimeval);
    ////wait and dispose return data
    while(readTimer->isActive())
    {
        t.restart();
        while(t.elapsed()<timeSpecVal)
            QCoreApplication::processEvents();
    }
    if(readtimes==0)
    {
        Data::errorFlag |=1<<(value-1);     //use errorFlag
        return false;
    }
    else
    {        
        switch(cmdtype)
        {
        case online:
        case settime:
        case lightSW:
            breturn=bCheckStr(backStr);            
            break;            
        case readversion:
            breturn=bCheckStr(backStr);
            savebakdata(cmdtype,value);
            break;
        case readwave:
        case readspec0:
            breturn=true;
            savebakdata(cmdtype+tmpreadtype, value);
            break;
        case readsigwave:
            breturn=bCheckStr(backStr);
            backStr=backStr.mid(10,4);
            Data::GetADvalue=backStr.left(2).toInt(&ok,16)*256+backStr.right(2).toInt(&ok,16);
            break;
        case readinit:
            breturn=bCheckStr(backStr);
            break;
        default:
            break;
        }
        return breturn;
    }
}

void serialCOM::readMyCom()
{
    QString tmpStr="";
    tmpArray = MyCom->readAll();
    Hex2String(tmpArray,tmpStr);
    if(tmpStr=="")
    {
        if(readtimes>0)
            readtimes--;
        else
          readTimer->stop();
    }
    else
    {
        backStr += tmpStr;
        switch(tmpCMDvalue)
        {
        case online:
        case readversion:
        case lightSW:
        case settime:
        case readsigwave:
        case readinit:
            if(backStr.size()==dataNorNum)
                readTimer->stop();
            break;
        case readwave:
        case readspec0:
        case readspec1:
        case readspec2:        
            if(backStr.size()==dataSpecNum)
                readTimer->stop();
            qDebug()<<"backStr.size()="<<backStr.size();
            qDebug()<<"dataNorNum"<<dataSpecNum;
            break;
        default:
            break;
        }
    }
}

void serialCOM::cleardata()
{
    tmpArray = MyCom->readAll();
}

void serialCOM::savebakdata(int cmdtype, int value)
{
    bool ok;
    switch(cmdtype)
    {
    case readversion:
        backStr=backStr.mid(10,6);
        Data::version=backStr.left(2).toInt(&ok,16)+backStr.mid(2,2).toInt(&ok,16)*0.1+backStr.right(2).toInt(&ok,16)*0.01;
        break;
    case readwave:
        backStr=backStr.right(backStr.size()-4);
        for(int i=0;i<backStr.size()/4;i++)
            Data::ListSpecNm[value-1][i]=(float)(backStr.mid(4*i,2).toInt(&ok,16)*256+backStr.mid(4*i+2,2).toInt(&ok,16))/10;
        break;
    case readspec0:
        backStr=backStr.right(backStr.size()-4);
        for(int i=0;i<backStr.size()/4;i++)
            Data::DarkData[value-1][i]=backStr.mid(4*i,2).toInt(&ok,16)*256+backStr.mid(4*i+2,2).toInt(&ok,16);
        break;
    case readspec1:
        backStr=backStr.right(backStr.size()-4);
        for(int i=0;i<backStr.size()/4;i++)
            Data::BaseData[value-1][i]=backStr.mid(4*i,2).toInt(&ok,16)*256+backStr.mid(4*i+2,2).toInt(&ok,16);
        break;
    case readspec2:
        backStr=backStr.right(backStr.size()-4);
        for(int i=0;i<backStr.size()/4;i++)
            Data::SampleData[value-1][i]=backStr.mid(4*i,2).toInt(&ok,16)*256+backStr.mid(4*i+2,2).toInt(&ok,16);
        break;
    default:
        break;
    }
}

void serialCOM::intToStr(int value, QString &tmpStr)
{
    char ms_temStr[8];
    sprintf(ms_temStr,"%02X",value);
    tmpStr+=ms_temStr;
}

QString serialCOM::sumCheckStr(QString &tmpStr)
{
    bool ok;
    QString subStr=tmpStr;
    int tmpInt=0;
    for(int i=0;i<tmpStr.size()/2;i++)
    {
        subStr=tmpStr.right(tmpStr.size()-2*i);
        tmpInt+=subStr.left(2).toInt(&ok,16);
    }
    subStr=QString::number(tmpInt,16).toUpper();
    return subStr.right(2);
}

bool serialCOM::bCheckStr(QString &tmpStr)
{
    QString Str1,Str2;
    Str1=tmpStr.left(tmpStr.size()-2);
    Str2=sumCheckStr(Str1);
    Str1=tmpStr.right(2);
    if(QString::compare(Str2,Str1,Qt::CaseInsensitive)==0)
        return true;
    else
        return false;
}

void serialCOM::String2Hex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toAscii();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toAscii();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char serialCOM::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void serialCOM::Hex2String(QByteArray senddata,QString &str)
{
    QDataStream out(&senddata,QIODevice::ReadWrite);
    while(!out.atEnd())
    {
      qint8 outChar = 0;
      out>>outChar;
      QString strtmp = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
      str += strtmp.toUpper();
     }
}

int serialCOM::matchdata(int num, float fvalue)
{
    int low,high,mid;
    low=0;
    high=RESOLUTION;
    while(low <= high)
    {
        mid = (low + high)/2;
        if(fvalue == Data::ListSpecNm[num][mid])
        {
            break;
        }
        if(fvalue > Data::ListSpecNm[num][mid])
        {
            low = mid + 1;
        }
        if(fvalue < Data::ListSpecNm[num][mid])
        {
            high = mid - 1;
        }
    }
    if(low <= high)
        return mid;
    else
    {
        if(abs(Data::ListSpecNm[num][low]-fvalue)> abs(Data::ListSpecNm[num][high]-fvalue))
        {
            if (high>0)
                return high;
            else
                return 0;
        }
        else
        {
            if(low<RESOLUTION)
                return low;
            else
                return RESOLUTION;
        }
    }
}
