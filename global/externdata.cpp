#include "externdata.h"

int Data::flagInit=0;                                //init flag 0 or 1(valid)
float Data::fThreshold = 300;                        //max min threshold value
int Data::errorFlag = 0;
int Data::sign_TestCHNum=0;
int Data::SetTime=5;                      //5-1000ms
float Data::version;
float Data::ListSpecNm[3][RESOLUTION];          //��ͨ�������б�ֵ
int Data::DarkData[3][RESOLUTION];              //��ͨ��������ADֵ
int Data::BaseData[3][RESOLUTION];              //��ͨ���հ�ADֵ
int Data::SampleData[3][RESOLUTION];            //��ͨ����ƷADֵ
//permission
int Data::userType=commonUser;        //commonUser,adminiUser
int Data::testType=serialTest;       //serialTest,noserialTest
//average time
int Data::average_time=16;                    //ͨ���������ݵ�ÿһ��ƽ������1,2,4,8,16
//food
float Data::GetWaveLength=809.6;                //���㷢�Ͳ���NM��
int Data::GetADvalue;                    //return ADֵ
int Data::Sign_DoubleWave[3];           //˫������־λ
float Data::WaveLengthMajor[3];        //������NM��
int Data::DarkMajor[3];               //������������ADֵ
int Data::BaseMajor[3];              //�������հ�ADֵ
int Data::SampleMajor[3];           //��������ƷADֵ
float Data::WaveLengthMinor[3];    //�ο�����NM��
int Data::DarkMinor[3];           //�ο�����������ADֵ
int Data::BaseMinor[3];          //�ο������հ�ADֵ
int Data::SampleMinor[3];       //�ο�������ƷADֵ
int Data::DLoadsign;                           //������������ԭ����Ϣ��־

