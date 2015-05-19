#include "dingtest_dlg.h"


DingTest_dlg::DingTest_dlg(QWidget *parent) :
    QDialog(parent,Qt::Dialog|Qt::FramelessWindowHint)
{

    setupUi(this);
    this->Btn_Quit->setFocus();
    if(Data::testType==serialTest)
        if(!serialCOMinit())
            hide();
    connect(this->Btn_Quit,SIGNAL(clicked()),this,SLOT(close()));
//    this->Btn_Del->setVisible(false);
//    this->Btn_Printer->setVisible(false);
    Sum_FinishTest = 0;
    Data::sign_TestCHNum=0;
    init();
}

DingTest_dlg::~DingTest_dlg()
{
    db_dingliang.close();
//    if(Data::testType==serialTest)
//    {
//        delete p_serialCOM3;
////        delete p_sample_infoinput;
//    }
////    delete qTableModel;
}

void DingTest_dlg::init()
{
    this->dateEdit->setDate(QDate::currentDate());
    this->dateEdit->setCalendarPopup(true);
    db_dingliang = QSqlDatabase::addDatabase("QSQLITE");
    db_dingliang.setDatabaseName(DLDATANAME);
    if (!db_dingliang.open())
    {
        QMessageBox::warning(this,tr("����"),tr("open database faile"),tr("ȷ��"));
        return;
    }
    qTableModel = new QSqlTableModel(this);
 //-----¼�������ĿĿ¼������һ������Ŀ�µ���Ʒ����Ŀ¼
    GetList_DTestItems();
//--------------------------------------------
    if(DTestItems_Sum > 0)
    {
        TestItems_Name = this->comboBox_TestItems->currentText();
        GetMeasureInfo();
        if(Data::sign_TestCHNum == 0)
        this->Btn_IsZero->setEnabled(false);
        else
        this->Btn_IsZero->setEnabled(true);
        this->Btn_EditTestItems->setEnabled(true);
        bsetEnable(false);

    }
    else
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_EditTestItems->setEnabled(false);
        bsetEnable(false);
    }
    this->Btn_Del->setEnabled(true);//
    this->Btn_Printer->setEnabled(true);
    QString Strdate = this->dateEdit->date().toString("yyyy-MM-dd");
    display_QTable(Strdate);
    display_QTable(Strdate);
}
void DingTest_dlg::GetList_DTestItems()
{
    QStringList fdlist;
    QSqlTableModel model;
    model.setTable("dinglianglist");
    model.setSort(0,Qt::AscendingOrder);
    model.select();
    DTestItems_Sum = model.rowCount();
    for(int row=0;row <model.rowCount();++row)
    {
        fdlist << model.record(row).value("name").toString();
    }
    this->comboBox_TestItems->clear();
    this->comboBox_TestItems->addItems(fdlist);
    this->comboBox_TestItems->setCurrentIndex(0);
}
void DingTest_dlg::GetMeasureInfo()
{
    int j;
        //������Ŀ�Ѿ�ȷ����ϣ��õ�������Ҫ�Ĳ�����Ϣ
        QSqlRecord record;
        QSqlTableModel model;
        model.setTable("dlinfor");
        model.setSort(0,Qt::AscendingOrder);
        model.setFilter(QObject::tr("dtestitems = '%1'").arg(TestItems_Name));
        model.select();
        if(model.rowCount()==1)
        {
            record =model.record(0);
        }
        else
        {
   //         QMessageBox::warning(this,tr("����"),tr("Sample data has some problems"),tr("ȷ��"));
            return;
        }
        for(j=0;j<3;j++)
        {
            Data::Sign_DoubleWave[j] = record.value("wavemode").toInt();
            Data::WaveLengthMajor[j] = record.value("wavemajor").toFloat();
            Data::WaveLengthMinor[j] = record.value("waveminor").toFloat();
        }
        Num_FormulaC= record.value("formulaC").toFloat();
        Num_FormulaB = record.value("formulaB").toFloat();
        Num_FormulaA = record.value("formulaA").toFloat();
        DSample_Name = record.value("unit").toString();
}
void DingTest_dlg::bsetEnable(bool bset)
{
    this->Btn_Del->setEnabled(bset);//
    this->Btn_Printer->setEnabled(bset);
    this->Btn_Measure->setEnabled(bset);
}
void DingTest_dlg::on_checkBox_CH1_stateChanged(int )
{
    if(this->checkBox_CH1->isChecked())
        Data::sign_TestCHNum |=0x01;
    else
        Data::sign_TestCHNum &=0x06;
    if( Data::sign_TestCHNum == 0)
    {
        this->Btn_IsZero->setEnabled(false);
		this->Btn_Measure->setEnabled(false);
        bsetEnable(false);
    }
    else
    {
        this->Btn_IsZero->setEnabled(true);
		this->Btn_Measure->setEnabled(false);
        bsetEnable(false);
    }
}

void DingTest_dlg::on_checkBox_CH2_stateChanged(int )
{
    if(this->checkBox_CH2->isChecked())
        Data::sign_TestCHNum |=0x02;
    else
        Data::sign_TestCHNum &=0x05;
    if( Data::sign_TestCHNum == 0)
    {
        this->Btn_IsZero->setEnabled(false);
		this->Btn_Measure->setEnabled(false);
        bsetEnable(false);
    }
    else
    {
        this->Btn_IsZero->setEnabled(true);
		this->Btn_Measure->setEnabled(false);
        bsetEnable(false);
    }
}

void DingTest_dlg::on_checkBox_CH3_stateChanged(int )
{
    if(this->checkBox_CH3->isChecked())
        Data::sign_TestCHNum |=0x04;
    else
        Data::sign_TestCHNum &=0x03;

    if( Data::sign_TestCHNum == 0)
    {
        this->Btn_IsZero->setEnabled(false);
		this->Btn_Measure->setEnabled(false);
        bsetEnable(false);
    }
    else
    {
        this->Btn_IsZero->setEnabled(true);
		this->Btn_Measure->setEnabled(false);
        bsetEnable(false);
    }

}
void DingTest_dlg::display_QTable(QString Strdate)
{
    qTableModel->setTable("result");
    //    qTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    qTableModel->setSort(0,Qt::AscendingOrder);            //��0�У�AscendingOrder��������,Qt::DescendingOrderΪ��������
    qTableModel->setFilter(QString("time like '%1'").arg(Strdate+"%"));   //QObject::tr("ftestitems = '%1'")
    qTableModel->select();
    qTableModel->setHeaderData(0, Qt::Horizontal, tr("������"));
    qTableModel->setHeaderData(1, Qt::Horizontal, tr("�����Ŀ����"));  //QString::fromLocal8Bit("����")
//    qTableModel->setHeaderData(2, Qt::Horizontal, tr("��Ʒ����"));
    qTableModel->setHeaderData(2, Qt::Horizontal, tr("����ͨ��"));
//    qTableModel->setHeaderData(4, Qt::Horizontal, tr("�ж����"));
    qTableModel->setHeaderData(3, Qt::Horizontal, tr("�������"));
    qTableModel->setHeaderData(4, Qt::Horizontal, tr("�����λ"));
//    qTableModel->setHeaderData(7, Qt::Horizontal, tr("�ο���׼"));
    qTableModel->setHeaderData(5, Qt::Horizontal, tr("����Ƚ��"));
//    qTableModel->setHeaderData(9, Qt::Horizontal, tr("��Ʒ���"));
//    qTableModel->setHeaderData(10, Qt::Horizontal, tr("��Ʒ����"));
//    qTableModel->setHeaderData(11, Qt::Horizontal, tr("����"));
//    qTableModel->setHeaderData(12, Qt::Horizontal, tr("�ͼ쵥λ"));
//    qTableModel->setHeaderData(13, Qt::Horizontal, tr("������Ա"));
//    qTableModel->setHeaderData(14, Qt::Horizontal, tr("���Ե�λ"));
    qTableModel->setHeaderData(6, Qt::Horizontal, tr("����ʱ��"));
    while(qTableModel->canFetchMore())
        qTableModel->fetchMore();
    //set header size
    QHeaderView *header;
    header =this->tableView->horizontalHeader();
    header->resizeSection(0,50);
    header->resizeSection(1,120);
    header->resizeSection(2,120);
//    header->resizeSection(3,80);
    header->resizeSection(3,80);
//    header->resizeSection(5,80);
    header->resizeSection(4,80);
//    header->resizeSection(7,120);
    header->resizeSection(5,80);
//    header->resizeSection(9,120);
 //   header->resizeSection(10,150);
 //   header->resizeSection(11,150);
 //   header->resizeSection(12,150);
//    header->resizeSection(13,80);
//    header->resizeSection(14,150);
    header->resizeSection(6,150);
    this->tableView->hideColumn(qTableModel->columnCount()<0);
    this->tableView->setCurrentIndex(qTableModel->index(qTableModel->rowCount()-1,5));
    //set some authority
    if(Data::userType==commonUser)
    {
        this->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        this->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    this->tableView->setShowGrid(true);
    this->tableView->setModel(qTableModel);
}
void DingTest_dlg::on_Btn_IsZero_clicked()
{
	QTime nntt;
    int j;
    bool ok=true;
    bool b_tmp;
    int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    bsetEnable(false);
    Data::SetTime = 5;
    bsetEnable(false);
    this->Btn_Measure->setEnabled(false);
    this->Btn_IsZero->setEnabled(false);
    QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,tr("�ȴ�"),tr("У���У����Ժ�"),QMessageBox::Ok,this);
    messageBox->show();
    if(Data::testType!=serialTest)
    {
        nntt.start();
        while(nntt.elapsed()<2000)
            QCoreApplication::processEvents();
    }
    if(Data::testType==serialTest)
    {
       b_tmp=p_serialCOM3->MultiCMD(settime,CHEN123);
       if(!b_tmp)
          QMessageBox::warning(this,tr("����"),tr("CCD set failed"),tr("ȷ��"));
    }
    ///��������   
    if(Data::testType==serialTest)       ////serial test mode
    {
        b_tmp = p_serialCOM3->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("����"),tr("read init failed"),tr("ȷ��"));
    }
    for(j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            Data::GetWaveLength = Data::WaveLengthMajor[j];
                        if(Data::testType==serialTest)
                        {
                            b_tmp = p_serialCOM3->MultiCMD(readsigwave, j+1);///////////////////////////////////
                            if(!b_tmp)
                                QMessageBox::warning(this,tr("����"),tr("read wave failed"),tr("ȷ��"));
                            Data::BaseMajor[j] = Data::GetADvalue;
                        }
                        else
                        {
                            Data::BaseMajor[j] = 2000;
                        }

                       if(Data::Sign_DoubleWave[j] == 1)
                        {
                             Data::GetWaveLength = Data::WaveLengthMinor[j];
                            if(Data::testType==serialTest)
                            {
                                b_tmp = p_serialCOM3->MultiCMD(readsigwave, j+1);///////////////////////////////////
                                if(!b_tmp)
                                    QMessageBox::warning(this,tr("����"),tr("read wave failed"),tr("ȷ��"));
                                Data::BaseMinor[j] = Data::GetADvalue;
                            }
                            else
                            {
                                Data::BaseMinor[j] = 2000;
                            }
                        }
        }
        tmpFlag=tmpFlag>>1;
    }
    if(Data::testType==serialTest)
        {
            b_tmp = p_serialCOM3->MultiCMD(lightSW,lightoff);
            if(!b_tmp)
                QMessageBox::warning(this,tr("����"),tr("light off failed"),tr("ȷ��"));
        }
//    ok &=b_tmp;
    ///��������
    if(Data::testType==serialTest)       ////serial test mode
    {
        b_tmp = p_serialCOM3->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("����"),tr("read init failed"),tr("ȷ��"));
    }
    tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
    for(j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            Data::GetWaveLength = Data::WaveLengthMajor[j];
                        if(Data::testType==serialTest)
                        {
                            b_tmp = p_serialCOM3->MultiCMD(readsigwave, j+1);///////////////////////////////////
                            if(!b_tmp)
                                QMessageBox::warning(this,tr("����"),tr("read wave failed"),tr("ȷ��"));
                            Data::DarkMajor[j] = Data::GetADvalue;
                        }
                        else
                        {
                            Data::DarkMajor[j] = 0;
                        }

                       if(Data::Sign_DoubleWave[j] == 1)
                        {
                             Data::GetWaveLength = Data::WaveLengthMinor[j];
                            if(Data::testType==serialTest)
                            {
                                b_tmp = p_serialCOM3->MultiCMD(readsigwave, j+1);///////////////////////////////////
                                if(!b_tmp)
                                    QMessageBox::warning(this,tr("����"),tr("read wave failed"),tr("ȷ��"));
                                Data::DarkMinor[j] = Data::GetADvalue;
                            }
                            else
                            {
                                Data::DarkMinor[j] = 0;
                            }
                        }
        }
        tmpFlag=tmpFlag>>1;
    }
    if(Data::testType==serialTest)
        {
            b_tmp = p_serialCOM3->MultiCMD(lightSW,lighton);
            if(!b_tmp)
                QMessageBox::warning(this,tr("����"),tr("light on failed"),tr("ȷ��"));
        }
        ok &=b_tmp;
        bsetEnable(true);
	    this->Btn_Measure->setEnabled(true);
        this->Btn_IsZero->setEnabled(true);
        messageBox->close();
        delete messageBox;
}

void DingTest_dlg::on_Btn_Measure_clicked()
{
	QTime nntt;
    int j;
    srand((unsigned)time(0));
    QSqlQuery query;
	QString Strdate;
    bool ok=true;
    bool b_tmp;
    int tmpFlag=Data::sign_TestCHNum;//^Data::errorFlag;
//    int row = this->Table_FoodTest->currentRow ();
	bsetEnable(false);
    this->Btn_Measure->setEnabled(false);
    this->Btn_IsZero->setEnabled(false);
    QMessageBox *messageBox=new QMessageBox(QMessageBox::Information,tr("�ȴ�"),tr("�����У����Ժ�"),QMessageBox::Ok,this);
    messageBox->show();
    if(Data::testType!=serialTest)
    {
        nntt.start();
        while(nntt.elapsed()<2000)
            QCoreApplication::processEvents();
    }

//    this->Table_FoodTest->scrollToItem( this->Table_FoodTest->item(Sum_FinishTest,0),QAbstractItemView::PositionAtBottom);
    ///��������
    if(Data::testType==serialTest)       ////serial test mode
    {
        b_tmp = p_serialCOM3->MultiCMD(readinit, CHEN123);
        if(!b_tmp)
            QMessageBox::warning(this,tr("����"),tr("read init failed"),tr("ȷ��"));
    }
    for(j=0;j<3;j++)
    {
        if(tmpFlag&0x01)
        {
            Data::GetWaveLength = Data::WaveLengthMajor[j];
                       if(Data::testType==serialTest)
                       {
                           b_tmp = p_serialCOM3->MultiCMD(readsigwave, j+1);///////////////////////////////////
                           if(!b_tmp)
                               QMessageBox::warning(this,tr("����"),tr("read wave failed"),tr("ȷ��"));
                           Data::SampleMajor[j] = Data::GetADvalue;
                       }
                       else
                       {
                           Data::SampleMajor[j] = 500;
                       }
                        AbsMajor[j] =  -log10((float)(Data::SampleMajor[j] - Data::DarkMajor[j])/(float)(Data::BaseMajor[j] - Data::DarkMajor[j]));
                        AbsSampleTest[j] = AbsMajor[j];
                        if(Data::Sign_DoubleWave[j] == 1)
                        {
                            Data::GetWaveLength = Data::WaveLengthMinor[j];
                            if(Data::testType==serialTest)
                            {
                                b_tmp = p_serialCOM3->MultiCMD(readsigwave, j+1);///////////////////////////////////
                                if(!b_tmp)
                                    QMessageBox::warning(this,tr("����"),tr("read wave failed"),tr("ȷ��"));
                                Data::SampleMinor[j] = Data::GetADvalue;
                            }
                            else
                            {
                                Data::SampleMinor[j] = 1000;
                            }                
                AbsMinor[j] =  -log10((float)(Data::SampleMinor[j] - Data::DarkMinor[j])/(float)(Data::BaseMinor[j] - Data::DarkMinor[j]));
                AbsSampleTest[j] = AbsMajor[j] - AbsMinor[j];
            }
            if(AbsSampleTest[j] > 10)
            {
                AbsSampleTest[j] = 10;
            }
            else if(AbsSampleTest[j] < -10)
            {
                AbsSampleTest[j] = -10;
            }
            Num_content[j] = (AbsSampleTest[j]*AbsSampleTest[j]*Num_FormulaC + AbsSampleTest[j]*Num_FormulaB + Num_FormulaA);       //������
//            this->Table_FoodTest->setItem(Sum_FinishTest,0,new QTableWidgetItem(TestItems_Name));
//            this->Table_FoodTest->setItem(Sum_FinishTest,1,new QTableWidgetItem(QString::number(j+1)));
//            this->Table_FoodTest->setItem(Sum_FinishTest,2,new QTableWidgetItem(QString::number(AbsSampleTest[j],'f',3)));
//            this->Table_FoodTest->setItem(Sum_FinishTest,3,new QTableWidgetItem(QString::number(Num_content[j],'f',1)));
//            this->Table_FoodTest->setItem(Sum_FinishTest,4,new QTableWidgetItem(DSample_Name));

//            QTableWidgetItem *item3 =this->Table_FoodTest->item(Sum_FinishTest,0);
//            this->Table_FoodTest->setCurrentItem(item3);
            query.exec("INSERT INTO result VALUES (NULL,'"+TestItems_Name+"', "
                                   " '"+QString::number(j+1)+"', "
                                   " '"+QString::number(Num_content[j],'f',1)+"', "
                                   " '"+DSample_Name+"', "
                                   " '"+QString::number(AbsSampleTest[j],'f',3)+"', "
                                   " '"+(QDateTime::currentDateTime()).toString("yyyy-MM-dd hh:mm:ss")+"')");

//            Sum_FinishTest++;

        }
        tmpFlag=tmpFlag>>1;
		Strdate = this->dateEdit->date().toString("yyyy-MM-dd");
        display_QTable(Strdate);
    }
    messageBox->close();
    delete messageBox;
    bsetEnable(true);
    this->Btn_Measure->setEnabled(true);
    this->Btn_IsZero->setEnabled(true);
}

void DingTest_dlg::on_comboBox_TestItems_currentIndexChanged(QString )
{
    TestItems_Name = this->comboBox_TestItems->currentText();
    GetMeasureInfo();//���õ���ǰ�����Ŀ������Ҫ�Ĳ�����Ϣ
	if( Data::sign_TestCHNum == 0)
    {
        this->Btn_IsZero->setEnabled(false);
        this->Btn_Measure->setEnabled(false);
    bsetEnable(false);
	}
    else
    {
        this->Btn_IsZero->setEnabled(true);
        this->Btn_Measure->setEnabled(false);
        bsetEnable(false);
    }
}

void DingTest_dlg::on_Btn_NewTestItems_clicked()
{
    QSqlQuery query;
    this->Btn_NewTestItems->setEnabled(false);
    this->Btn_EditTestItems->setEnabled(false);
    int tmp;
    tmp = Data::sign_TestCHNum;
    Data::DLoadsign = 0;
    if(Data::testType==serialTest)
    {
        delete p_serialCOM3;
        //        delete p_sample_infoinput;
    }
    p_quantifyinfoinput = new quantifyinfoinput_dlg;
    p_quantifyinfoinput->show();
    p_quantifyinfoinput->exec();
    if(Data::testType==serialTest)
    {
        p_serialCOM3 =new serialCOM;
        //        delete p_sample_infoinput;
    }
    if(p_quantifyinfoinput->Sign_Save)
    {
        TestItems_Name = p_quantifyinfoinput->QuantifyTestName ;
        DSample_Name = p_quantifyinfoinput->DUint_Name;
        Num_FormulaA = p_quantifyinfoinput->Num_FormulaA;         //ϵ��A
        Num_FormulaB = p_quantifyinfoinput->Num_FormulaB;         //ϵ��B
        Num_FormulaC = p_quantifyinfoinput->Num_FormulaC;         //ϵ��C
        query.exec("INSERT INTO dinglianglist VALUES (NULL,'"+TestItems_Name+"')");
        query.exec("INSERT INTO dlinfor VALUES (NULL,'"+TestItems_Name+"', "
                   " '"+QString::number(Data::Sign_DoubleWave[0])+"', "
                   " '"+QString::number(Data::WaveLengthMajor[0],'f',1)+"', "
                   " '"+QString::number(Data::WaveLengthMinor[0],'f',1)+"', "
                   " '"+QString::number(Num_FormulaC,'f',3)+"', "
                   " '"+QString::number(Num_FormulaB,'f',3)+"', "
                   " '"+QString::number(Num_FormulaA,'f',3)+"', "
                   " '"+DSample_Name+"')");
        init();

    }
    Data::sign_TestCHNum = tmp;
    this->Btn_NewTestItems->setEnabled(true);
    this->Btn_EditTestItems->setEnabled(true);
}

void DingTest_dlg::on_Btn_EditTestItems_clicked()
{
    int tmp;
    this->Btn_NewTestItems->setEnabled(false);
    this->Btn_EditTestItems->setEnabled(false);
    QString Temp_Name = TestItems_Name;     //�����Ŀ
    tmp = Data::sign_TestCHNum;
    Data::DLoadsign = 1;
    QSqlTableModel model;    
    model.setTable("dlinfor");
    model.setSort(0,Qt::AscendingOrder);
    model.setFilter(QObject::tr("dtestitems = '%1'").arg(TestItems_Name));
    model.select();
    if(Data::testType==serialTest)
    {
        delete p_serialCOM3;
//        delete p_sample_infoinput;
    }
    p_quantifyinfoinput = new quantifyinfoinput_dlg;
    p_quantifyinfoinput->QuantifyTestName = TestItems_Name;
    p_quantifyinfoinput->DUint_Name = DSample_Name;
    p_quantifyinfoinput->Num_FormulaA = Num_FormulaA;         //ϵ��A
    p_quantifyinfoinput->Num_FormulaB = Num_FormulaB;         //ϵ��B
    p_quantifyinfoinput->Num_FormulaC = Num_FormulaC;         //ϵ��C
    p_quantifyinfoinput->init();
    p_quantifyinfoinput->show();
    p_quantifyinfoinput->exec();
    if(Data::testType==serialTest)
    {
        p_serialCOM3 =new serialCOM;
//        delete p_sample_infoinput;
    }
    if(model.rowCount()==1)
    {
        if(p_quantifyinfoinput->Sign_Save == 1)
        {
            TestItems_Name = p_quantifyinfoinput->QuantifyTestName ;
            DSample_Name = p_quantifyinfoinput->DUint_Name;

            Num_FormulaA = p_quantifyinfoinput->Num_FormulaA;         //ϵ��A
            Num_FormulaB = p_quantifyinfoinput->Num_FormulaB;         //ϵ��B
            Num_FormulaC = p_quantifyinfoinput->Num_FormulaC;         //ϵ��C
            model.setData(model.index(0,1),TestItems_Name);
            model.setData(model.index(0,2),QString::number(Data::Sign_DoubleWave[0]));
            model.setData(model.index(0,3),QString::number(Data::WaveLengthMajor[0],'f',1));
            model.setData(model.index(0,4),QString::number(Data::WaveLengthMinor[0],'f',1));
            model.setData(model.index(0,5),QString::number(Num_FormulaC,'f',3));
            model.setData(model.index(0,6),QString::number(Num_FormulaB,'f',3));
            model.setData(model.index(0,7),QString::number(Num_FormulaA,'f',3));
            model.setData(model.index(0,8),DSample_Name);
        }
        else if(p_quantifyinfoinput->Sign_Save == 2)
        {
            model.removeRows(0,1);
        }
        model.submitAll();
    }
    else
    {
        QMessageBox::warning(this,tr("����"),tr("Sample data has some problems"),tr("ȷ��"));
        return;
    }
    model.setTable("dinglianglist");
    model.setSort(0,Qt::AscendingOrder);
    model.setFilter(QObject::tr("name = '%1'").arg(Temp_Name));
    model.select();
    if(model.rowCount()==1)
    {
        if(p_quantifyinfoinput->Sign_Save == 1)
        {
            model.setData(model.index(0,1),TestItems_Name);
        }
        else if(p_quantifyinfoinput->Sign_Save == 2)
        {
            model.removeRows(0,1);
        }
    }
    model.submitAll();
    init();
    Data::sign_TestCHNum = tmp;
    this->Btn_NewTestItems->setEnabled(true);
    this->Btn_EditTestItems->setEnabled(true);
}

bool DingTest_dlg::serialCOMinit()
{
    bool ok=true;
    bool b_tmp;
    p_serialCOM3 =new serialCOM;
    p_serialPrint3 =new serialPrint;
    PrintInit();//////��ӡ����ʼ��
    ////connect
    b_tmp=p_serialCOM3->MultiCMD(online,CHEN123);
    ok &=b_tmp;
    if(!b_tmp)
    {
        QMessageBox::warning(this,tr("����"),tr("on line failed"),tr("ȷ��"));
        return ok;
    }
    b_tmp=p_serialCOM3->MultiCMD(readwave,CHEN123);
    ok &=b_tmp;
    if(!b_tmp)
    {
        QMessageBox::warning(this,tr("����"),tr("read wave failed"),tr("ȷ��"));
        return ok;
    }
    return ok;
}
void DingTest_dlg::on_Btn_Display_clicked()
{
    QString Strdate = this->dateEdit->date().toString("yyyy-MM-dd");
    display_QTable(Strdate);
}
void DingTest_dlg::on_Btn_Del_clicked()
{
    QItemSelectionModel *selections = tableView->selectionModel();
    QModelIndexList selecteds;
    selecteds = selections->selectedRows();
    int ok = QMessageBox::warning(this,tr("ɾ��ѡ�е���!"),tr("��ȷ��ɾ����ǰѡȡ�е�����?"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::Yes)
    {
        foreach (QModelIndex index, selecteds)
        {
            int curRow = index.row();
            qTableModel->removeRow(curRow);
        }
        qTableModel->submitAll();
    }
    else
    {
        qTableModel->revertAll();
    }
}
void DingTest_dlg::Printchar(char Pdata1)
{    
    qDebug()<<Pdata1 ;
    p_serialPrint3->Printchar(Pdata1);
}
void DingTest_dlg::PrintString(QString Pdata2)
{
    qDebug()<<Pdata2;
    p_serialPrint3->PrintString(Pdata2);
}
void DingTest_dlg::PrintInit(void)
{
    /*
    ��ӡ���ô��ڳ�ʼ��������9600
    */
    Printchar(0x1b);	//   ��ӡ����ʼ��
    Printchar(0x40);

    Printchar(0x1b);     //   ��ӡ�������ӡ
    Printchar(0x63);
    Printchar(0x0);

    Printchar(0x1c);	//�򿪺�����ʾģʽ
    Printchar(0x26);

    Printchar(0x1B);     //����Ŵ�
    Printchar(0x55);
    Printchar(0x02);

}

void DingTest_dlg::on_Btn_Printer_clicked()
{
    QItemSelectionModel *selections = tableView->selectionModel();
    QModelIndexList selecteds;
    selecteds = selections->selectedRows();
    PrintString("  ʳƷ��ȫ���ټ����");
    Printchar(0x0A);
    foreach (QModelIndex index, selecteds)
    {
        int curRow = index.row();
        PrintString("�����Ŀ��"+qTableModel->record(curRow).value("prjname").toString());
        Printchar(0x0A);
 //       PrintString("��Ʒ���ͣ�"+qTableModel->record(curRow).value("sampletype").toString());
  //      Printchar(0x0A);
        PrintString("ͨ����"+qTableModel->record(curRow).value("detectchn").toString());
  //      PrintString("�����"+qTableModel->record(curRow).value("judgeresult").toString());
        Printchar(0x0A);
        PrintString(qTableModel->record(curRow).value("prjresult").toString()+qTableModel->record(curRow).value("unit").toString());
        Printchar(0x0A);
        PrintString("����ʱ�䣺"+qTableModel->record(curRow).value("time").toString());
        Printchar(0x0A);
    }
}
