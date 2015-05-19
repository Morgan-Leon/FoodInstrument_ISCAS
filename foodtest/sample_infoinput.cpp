#include "sample_infoinput.h"

sample_infoinput::sample_infoinput(QWidget *parent) :
    QDialog(parent)
{

    setupUi(this);
    this->Btn_Quit->setFocus();
    SampleInfo.Sample_Num = this->Edit_Numinput->text();
    SampleInfo.Sample_Name = this->Edit_Nameinput->text();
    SampleInfo.Sample_From = this->Edit_Frominput->text();
    SampleInfo.Sample_Unit = this->Edit_Unitinput->text();
    SampleInfo.Test_Name = this->EditTestName->text();
    SampleInfo.Test_Unit = this->Edit_TestUnit->text();
}

sample_infoinput::~sample_infoinput()
{
    ;
}

void sample_infoinput::on_Btn_Save_clicked()
{
    SampleInfo.Sample_Num = this->Edit_Numinput->text();
    SampleInfo.Sample_Name = this->Edit_Nameinput->text();
    SampleInfo.Sample_From = this->Edit_Frominput->text();
    SampleInfo.Sample_Unit = this->Edit_Unitinput->text();
    SampleInfo.Test_Name = this->EditTestName->text();
    SampleInfo.Test_Unit = this->Edit_TestUnit->text();
//    qDebug()<<SampleInfo.Sample_Num;
    hide();
    emit transStrToDlg(SampleInfo);
}

void sample_infoinput::on_Btn_Quit_clicked()
{
    hide();
}
