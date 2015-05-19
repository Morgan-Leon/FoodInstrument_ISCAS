#include "externdata.h"

int Data::flagInit=0;                                //init flag 0 or 1(valid)
float Data::fThreshold = 300;                        //max min threshold value
int Data::errorFlag = 0;
int Data::sign_TestCHNum=0;
int Data::SetTime=5;                      //5-1000ms
float Data::version;
float Data::ListSpecNm[3][RESOLUTION];          //各通道包场列表值
int Data::DarkData[3][RESOLUTION];              //各通道暗电流AD值
int Data::BaseData[3][RESOLUTION];              //各通道空白AD值
int Data::SampleData[3][RESOLUTION];            //各通道样品AD值
//permission
int Data::userType=commonUser;        //commonUser,adminiUser
int Data::testType=serialTest;       //serialTest,noserialTest
//average time
int Data::average_time=16;                    //通道光谱数据的每一点平均次数1,2,4,8,16
//food
float Data::GetWaveLength=809.6;                //单点发送波长NM数
int Data::GetADvalue;                    //return AD值
int Data::Sign_DoubleWave[3];           //双波长标志位
float Data::WaveLengthMajor[3];        //主波长NM数
int Data::DarkMajor[3];               //主波长暗电流AD值
int Data::BaseMajor[3];              //主波长空白AD值
int Data::SampleMajor[3];           //主波长样品AD值
float Data::WaveLengthMinor[3];    //参考波长NM数
int Data::DarkMinor[3];           //参考波长暗电流AD值
int Data::BaseMinor[3];          //参考波长空白AD值
int Data::SampleMinor[3];       //参考波长样品AD值
int Data::DLoadsign;                           //定量测量载入原有信息标志

