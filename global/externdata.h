#ifndef EXTERNDATA_H
#define EXTERNDATA_H

//Data.h
#ifndef DATA_H
#define DATA_H

#define READNUM 8       ///read serialCOM times at one time  delay 1s 2.5(2)s * Data::average_time

#define SerialPort0 "/dev/ttyusb0"
#define SerialPort1 "/dev/ttySAC1"
#define SerialPort2 "/dev/ttySAC2"
#define SerialPort3 "/dev/ttySAC3"

#define RESOLUTION 2048
#define CurPenWidth 1
#define WaitTime 1  ////second
#define NeedDelay 0 ///second

#define ModifyX 20          ////modify coordinate system X
#define ModifyY 20         ////modify coordinate system Y and margin ///+20
#define ModifyD 70        ////modify display

class Data
{
public:
    static int flagInit;                             //init flag
    static float fThreshold;                        //max min threshold value
    static int errorFlag;                          //ע��ؼ���static
    static int sign_TestCHNum;                    //ͨ��ֵ
    static int SetTime;                          //5-1000ms
    static float version;                       //�汾
    static float ListSpecNm[3][RESOLUTION];          //��ͨ�������б�ֵ
    static int DarkData[3][RESOLUTION];             //��ͨ��������ADֵ
    static int BaseData[3][RESOLUTION];            //��ͨ���հ�ADֵ
    static int SampleData[3][RESOLUTION];         //��ͨ����ƷADֵ
    //permission
    static int userType;
    static int testType;
    //average time
    static int average_time;                    //ͨ���������ݵ�ÿһ��ƽ������1,2,4,8
    //food    
    static float GetWaveLength;                //���㷢�Ͳ���NM��
    static int GetADvalue;                    //return ADֵ
    static int Sign_DoubleWave[3];           //˫������־λ
    static float WaveLengthMajor[3];        //������NM��
    static int DarkMajor[3];               //������������ADֵ
    static int BaseMajor[3];              //�������հ�ADֵ
    static int SampleMajor[3];           //��������ƷADֵ
    static float WaveLengthMinor[3];    //�ο�����NM��
    static int DarkMinor[3];           //�ο�����������ADֵ
    static int BaseMinor[3];          //�ο������հ�ADֵ
    static int SampleMinor[3];       //�ο�������ƷADֵ
    static int DLoadsign;                           //������������ԭ����Ϣ��־
};


enum CMDType                 ///command type
{
    online=90,               ///connect device
    readversion=100,        ///read serial version
    settime=110,           ///set integral time
    readwave=120,         ///read wave length list
    readspec0=130,       ///read spectroscopic dark data
    readspec1=131,      ///read spectroscopic base data
    readspec2=132,     ///read spectroscopic sample data
    lightSW=140,      ///turn on/off the light
    readsigwave=150, ///read signle wave AD
    readinit=160,   ///read init
};
enum LightStatus
{
    lightoff=0,      ///light off
    lighton=1,        ///light on    
};
enum CHNum
{
    CHEN1=1,              ///channel 1
    CHEN2=2,             ///channel 2
    CHEN3=4,            ///channel 3
    CHEN12=3,          ///channel 1 2
    CHEN13=5,         ///channel 1 3
    CHEN23=6,        ///channel 2 3
    CHEN123=7,      ///channel 1 2 3
};
enum readType
{
    darktype=0,
    basetype=1,
    sampletype=2,
};
enum TimerVal
{
    timeNorVal=200,
    timeSpecVal=500,
};

enum DataValNum
{
    dataNorNum=24,
    dataSpecNum=RESOLUTION*4+4,       ///4096
};
enum UserType
{
    commonUser=20,
    adminiUser=21,
};
enum TestType
{
    serialTest=30,
    noserialTest=31,
    showType=32,
};


#endif
#endif // EXTERNDATA_H
